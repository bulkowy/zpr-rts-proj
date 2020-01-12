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
        static_assert(C::_type != _invalidComponentType, "C must define a valid non-zero _type");
        return _componentStores.insert(std::make_pair(C::_type, IComponentStore::Ptr(new ComponentStore<C>()))).second;
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
        static_assert(C::_type != _invalidComponentType, "C must define a valid non-zero _type");
        auto iComponentStore = _componentStores.find(C::_type);
        if (_componentStores.end() == iComponentStore) {
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
        assert(_lastEntity < std::numeric_limits<Entity>::max());
        _entities.insert(std::make_pair((_lastEntity + 1), ComponentTypeSet()));
        return (++_lastEntity);
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
        static_assert(C::_type != _invalidComponentType, "C must define a valid non-zero _type");
        
        auto entity = _entities.find(aEntity);
        if (_entities.end() == entity) {
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
     * @param[in] aElapsedTime Czas który upłynął od ostatniego wywołania, w sekundach.
     *
     * @return  Liczbę przetworzonych Obiektów (dany obiekt może być przetworzony wielokrotnie przez wiele systemów).
     */
    void update(float aElapsedTime);

private:
    /// Id ostatniego utworzonego obiektu (zaczyna się od 0)
    Entity                                          _lastEntity;

    /**
     * @brief Hashmapa wszystkich zarejestrowanych Obiektów, określająca typy posiadanych Komponentów.
     *
     *  Wiąże id każdego Obiektu z Typami posiadanych Komponentów.
     */
    std::unordered_map<Entity, ComponentTypeSet>    _entities;

    /**
     * @brief Mapa wszystkich Zbiorów Komponentów w zależności od Typu.
     *
     *  Zbiór wszystkich Komponentów wszystkich Obiektów, pogrupowany Typami.
     */
    std::map<ComponentType, IComponentStore::Ptr>   _componentStores;

    /**
     * @brief Lsita wszystkich systemów, w kolejności dodawania.
     *
     * Jeśli pointer do danego Systemu występuje w liście dwa razy, System zostanie dwa razy wywołany w każdym kroku silnika (w kolejności dodawanai do listy).
     */
    std::vector<System::Ptr>                        _systems;
};

// ----- implementacje ------

Engine::Engine() :
    _lastEntity(_invalidEntity),
    _entities(),
    _componentStores(),
    _systems() {
}

Engine::~Engine() {
}

// Dodaj System.
void Engine::addSystem(const System::Ptr& aSystemPtr) {
    // Sprawdź czy są określone wymagane komponenty
    if ((!aSystemPtr) || (aSystemPtr->getRequiredComponents().empty())) {
        throw std::runtime_error("System shall specified required Components");
    }
    _systems.push_back(aSystemPtr);
}

// Zarejestruj Obiekt do wszystkich Systemów.
size_t Engine::registerEntity(const Entity aEntity) {
    size_t affectedSystemsCount = 0;

    auto entity = _entities.find(aEntity);
    if (_entities.end() == entity) {
        throw std::runtime_error("The Entity does not exist");
    }
    auto entityComponents = (*entity).second;

    // Sprawdź, które Systemy będą zainteresowane tym Obiektem
    for (auto system  = _systems.begin();
              system != _systems.end();
            ++system) {
        auto systemRequiredComponents = (*system)->getRequiredComponents();
        for (std::size_t family = 0;
                family < systemRequiredComponents.size();
                family++)
        {
            // Sprawdź czy Obiekt zawiera wszystkie wymagane Komponenty
            if (std::includes(entityComponents.begin(), entityComponents.end(),
                            systemRequiredComponents[family].begin(), systemRequiredComponents[family].end())) {
                // Zarejestruj Obiekt
                (*system)->registerEntity(aEntity, family);
                ++affectedSystemsCount;
            }
        }            
        
    }

    return affectedSystemsCount;
}

// Wyrejestruj Obiekt ze wszystkich Systemów.
void Engine::unregisterEntity(const Entity aEntity) {
    auto entity = _entities.find(aEntity);
    if (_entities.end() == entity) {
        throw std::runtime_error("The Entity does not exist");
    }

    for (auto system  = _systems.begin();
              system != _systems.end();
            ++system) {
        (*system)->unregisterEntity(aEntity);
    }

    return;
}

// Wykonaj obliczenia dla wszystkich Obiektów wszystkich Systemów.
void Engine::update(float aElapsedTime) {

    for (auto system  = _systems.begin();
              system != _systems.end();
            ++system) {
        (*system)->update(aElapsedTime);
    }

    return;
}


} // namespace ecs

#endif