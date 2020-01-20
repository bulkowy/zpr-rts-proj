#ifndef _ENGINE_
#define _ENGINE_

#include "Entity.hpp"
#include "Component.hpp"
#include "ComponentType.hpp"
#include "ComponentStore.hpp"
#include "System.hpp"

#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <memory>   // std::shared_ptr
#include <cassert>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <chrono>   // std::chrono::duration
#include <thread>   // std::sleep_for

namespace ecs {

/**
 * @brief   Zarządza powiązaniami pomiędzy Obiektami (Entity), Komponentami (Component) i Systemami (System).
 */
class Engine {
public:
    Engine();
    virtual ~Engine();

    /**
     * @brief   Stwórz ComponentStore dla danego typu Komponentu.
     *
     * @tparam C    Struktura dziedzicząca po Komponencie, definiująca typ Komponentu.
     */
    template<typename C>
    inline bool createComponentStore() {
        static_assert(std::is_base_of<Component, C>::value, "C must be derived from the Component struct");
        return componentStores_.insert(std::make_pair(C::_type, IComponentStore::Ptr(new ComponentStore<C>()))).second;
    }

    /**
     * @brief   Uzyskaj (dostęp do) Zbioru Komponentów danego Typu.
     *
     *  Rzuca std::runtime_error jeśli Zbiór nie istnieje.
     *
     * @tparam C    Struktura dziedzicząca po Component, definiująca jego Typ
     *
     * @return      Referencję doZbioru Komponentów danego Typu.
     */
    template<typename C>
    inline ComponentStore<C>& getComponentStore() {
        static_assert(std::is_base_of<Component, C>::value, "C must be derived from the Component struct");
        auto iComponentStore = componentStores_.find(C::_type);
        if (componentStores_.end() == iComponentStore) {
            throw std::runtime_error("The ComponentStore does not exist");
        }
        return reinterpret_cast<ComponentStore<C>&>(*(iComponentStore->second));
    }

    /**
     * @brief Dodaj System do Silnika.
     *
     *  Wymaga shared pointera (zamiast unique_ptr) do Systemu, żeby móc wielokrotnie go umieścić w wektorze Systemów w celu wielokrotnego wywołania.
     *
     * @param[in] aSystemPtr    Shared pointer do dodawanego Systemu.
     */
    void addSystem(const System::Ptr& aSystemPtr);

    /**
     * @brief   Stwórz nowy Obiekt (przydziel mu nowy id).
     *
     * @return  Id nowego Obiektu.
     */
    inline Entity createEntity() {
        assert(lastEntity_ < std::numeric_limits<Entity>::max());
        entities_.insert(std::make_pair((lastEntity_ + 1), ComponentTypeSet()));
        return (++lastEntity_);
    }

    /**
     * @brief Dodaj (przenieś) Komponent (tego samego typu co ComponentStore) powiązany z Obiektem.
     *
     *  Rzuca std::runtime_error jeśli Obiekt lub Zbiór Komponentów nie istnieje.
     *
     *  Przenieś nowy Komponent do Zbioru, wiążąc go z Obiektem.
     * Używanie 'rvalue' (używanie "move semantic" C++11) wymaga:
     * - wywołania add() z 'std::move(component)', na przykład:
     *   ComponentTest1 component1(123);
     *   store.add(entity1, std::move(component1));
     * - wywołania add() z nowym tymczasowym 'ComponentXxx(args)', na przykład:
     *   store.add(entity1, ComponentTest1(123);
     *
     * @tparam C    Struktura dziedzicząca po Component, definiująca jego Typ.
     *
     * @param[in] aEntity       Id Obiektu
     * @param[in] aComponent    'rvalue' nowego Komponentu do wstawienia
     *
     * @return true jeśli wstawiono poprawnie
     */
    template<typename C>
    inline bool addComponent(const Entity aEntity, C&& aComponent) {
        static_assert(std::is_base_of<Component, C>::value, "C must be derived from the Component struct");
        
        auto entity = entities_.find(aEntity);
        if (entities_.end() == entity) {
            throw std::runtime_error("The Entity does not exist");
        }
        // Dodaj ComponentType do Obiektu
        (*entity).second.insert(C::_type);
        // Dodaj Komponent do Zbioru
        return getComponentStore<C>().add(aEntity, std::move(aComponent));
    }

    /**
     * @brief   Zarejestruj Obiekt do wszystkich adekwatnych Systemów.
     *
     * Obiekt musi posiadać wszystkie Komponenty w momencie bycia rejestrowanym do Systemu.
     *
     * @param[in] aEntity   Id Obiektu
     *
     * @return  Liczba Systemów objętych zmianą.
     */
    size_t registerEntity(const Entity aEntity);

    /**
     * @brief   Wyrejestruj Obiekt ze wszytkich Systemów.
     *
     * @param[in] aEntity   Id Obiektu
     *
     * @return  Liczba Systemów objętych zmianą.
     */
    void unregisterEntity(const Entity aEntity);

    /**
     * @brief   Wykonaj obliczenia (update) dla każdego Obiektu wszystkich Systemów.
     *
     * @param[in] frameTime Czas który upłynął od ostatniego wywołania, w milisekundach.
     *
     * @return  Liczbę przetworzonych Obiektów (dany obiekt może być przetworzony wielokrotnie przez wiele systemów).
     */
    virtual void update(unsigned int frameTime);
    
    /**
     * @brief  Uzyskaj zbiór wszystkich zarejestrowanych Obiektów.
     *
     * @return  Zbiór wszystkich zarejestrowanych Obiektów;
     */
    std::set<Entity> getEntitySet();

    /**
     * @brief  Uzyskaj zbiór wszystkich Komponentów.
     *
     * @return  Zbiór wszystkich Komponentów.
     */
    inline std::map<ComponentType, IComponentStore::Ptr> getComponentStores() { return componentStores_; }

private:
    /// Id ostatniego utworzonego obiektu (zaczyna się od 0)
    Entity                                          lastEntity_;

    /**
     * @brief Hashmapa wszystkich zarejestrowanych Obiektów, określająca typy posiadanych Komponentów.
     *
     *  Wiąże id każdego Obiektu z Typami posiadanych Komponentów.
     */
    std::unordered_map<Entity, ComponentTypeSet>    entities_;

    /**
     * @brief Mapa wszystkich Zbiorów Komponentów w zależności od Typu.
     *
     *  Zbiór wszystkich Komponentów wszystkich Obiektów, pogrupowany Typami.
     */
    std::map<ComponentType, IComponentStore::Ptr>   componentStores_;

    /**
     * @brief Lista wszystkich systemów, w kolejności dodawania.
     *
     * Jeśli pointer do danego Systemu występuje w liście dwa razy, System zostanie dwa razy wywołany w każdym kroku silnika (w kolejności dodawanai do listy).
     */
    std::vector<System::Ptr>                        systems_;
};

} // namespace ecs

#endif