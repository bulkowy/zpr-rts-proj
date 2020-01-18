#ifndef _COMPONENTSTORE_
#define _COMPONENTSTORE_

#include "Entity.hpp"
#include "Component.hpp"

#include <unordered_map>
#include <memory>

namespace ecs {

/**
 * @brief   Abstrakcyjna klasa bazowa dla wszystkich szablonowanych Zbiorów Komponentów (ComponentStore)
 */
class IComponentStore {
public:
    /**
     * @brief Shared pointer do ComponentStore
     */
    typedef std::shared_ptr<IComponentStore> Ptr;
};

/**
 * @brief   ComponentStore przechowuje dane wszystkich Komponentów danego typu dla wszystkich Obiektów.
 *
 * @tparam C    Strutura dziedzicząca po klasie Component, określająca dany typ Komponentu.
 */
template<typename C>
class ComponentStore : public IComponentStore {
    static_assert(std::is_base_of<Component, C>::value, "C must be derived from the Component struct");
    static_assert(C::_type != _invalidComponentType, "C must define a valid non-zero _type");

public:
    ComponentStore() {
    }
    ~ComponentStore() {
    }

    /**
     * @brief Dodaj (przenieś) Komponent (tego samego typu co ComponentStore) powiązany z Obiektem.
     *
     *  Przenieś nowy Komponent do Zbioru, wiążąc go z Obiektem.
     * Używanie 'rvalue' (używanie "move semantic" C++11) wymaga:
     * - wywołania add() z 'std::move(component)', na przykład:
     *   ComponentTest1 component1(123);
     *   store.add(entity1, std::move(component1));
     * - wywołania add() z nowym tymczasowym 'ComponentXxx(args)', na przykład:
     *   store.add(entity1, ComponentTest1(123);
     *
     * @param[in] aEntity       Id Obiektu, którego Komponent chcemy dodać.
     * @param[in] aComponent    'rvalue' nowego Komponentu do dodania.
     *
     * @return true jeśli dodano poprawnie
     */
    inline bool add(const Entity aEntity, C&& aComponent) {
        return _store.insert(std::make_pair(aEntity, std::move(aComponent))).second;
    }

    /**
     * @brief Usuń (zniszcz) Komponent przynależny do sprecyzowanego Obiektu.
     *
     * @param[in] aEntity   Id Obiektu
     *
     * @return true jeśli poprawnie znaleziono Obiekt i usunięto Komponent
     */
    inline bool remove(Entity aEntity) {
        return (0 < _store.erase(aEntity));
    }

    /**
     * @brief Sprawdź czy zbiór zawiera komponent sprecyzowango Obiektu
     *
     * @param[in] aEntity   Id Obiektu
     *
     * @return true jeśli znaleziono
     */
    inline bool has(Entity aEntity) const {
        return (_store.end() != _store.find(aEntity));
    }

    /**
     * @brief Uzyskaj dostęp do komponentu sprecyzowanego Obiektu.
     *
     *  Rzuca std::out_of_range exception jeśli Obiekt i jego Komponent nie zostały znalezione.
     *
     * @param[in] aEntity   Id Obiektu 
     *
     * @return Referencję do Komponentu (lub rzuca wyjątek).
     */
    inline C& get(Entity aEntity) {
        return _store.at(aEntity);
    }

    /**
     * @brief Wyjmij (move out) Komponent przynależny do sprecyzowanego Obiektu.
     *
     *  Rzuca std::out_of_range exception jeśli Obiekt i jego Komponent nie zostały znalezione.
     *
     * @param[in] aEntity   Id Obiektu 
     *
     * @return Komponent przynależny do sprecyzowanego Obiektu (lub rzuca wyjątek).
     */
    inline C extract(Entity aEntity) {
        C component = std::move(_store.at(aEntity));
        _store.erase(aEntity);
        return component;
    }

    /**
     * @brief Uzyskaj dostęp do zbioru Komponentów.
     *
     * @return Referencja do zbioru Komponentów.
     */
    inline const std::unordered_map<Entity, C>& getComponents() {
        return _store;
    }

private:
    std::unordered_map<Entity, C>   _store;             ///< Hash mapa przechowywanych Komponentów
    static const ComponentType      _type = C::_type; ///< Typ przechowywanych Komponentów
};

} // namespace ecs

#endif