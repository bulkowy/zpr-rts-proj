#ifndef _SYSTEM_
#define _SYSTEM_

#include "ComponentType.hpp"
#include "Entity.hpp"

#include <set>
#include <vector>
#include <memory>

namespace ecs {

class Engine;

/**
 * @brief System przetwarza dane każdego Obiektu (Entity), który posiada wszystkie wymagane Komponenty (Components).
 *
 *  System to program wykonujący obliczenia związane z logiką i uaktualniający dane na każdym Obiekcie (Entity), który posiada wszystkie wymagane Komponenty (Components).
 * Systemy wykonują obliczenia w każdej klatce na każdym odpowiadającym Obiekcie.
 *
 *  To jest klasa bazowa, która potrzebuje utworzenia podklasy.
 */
class System {
public:
    /// Shared pointer do systemu jest potrzebny do wielokrotnego umieszczenia Systemu w wektorze Systemów, w celu wielokrotnego wykonania w trakcie jednego kroku gry.
    typedef std::shared_ptr<System> Ptr;

    /**
     * @brief Konstruktor.
     *
     * @param[in] engine  Odniesienie do Silnika gry (Engine) potrzebne, żeby uzyskać dostęp do Komponentów Obiektów.
     */
    explicit System(Engine& aEngine) :
        _engine(aEngine) {
    }

    /**
     * @brief Destruktor.
     */
    virtual ~System() {};

    /**
     * @brief Uzyskaj typy wszystkich Komponentów wymaganych przez System.
     */
    inline const std::vector<ComponentTypeSet>& getRequiredComponents() const {
        return _requiredComponents;
    }

    /**
     * @brief Zarejestruj adekwatny Obiekt, posiadający wszystkie wymagane Komponenty.
     *
     * @param[in] entity   Adekwatny Obiekt
     *
     * @return true jeżeli dodano Obiekt poprawnie
     */
    inline bool registerEntity(Entity entity, int family) {
        return _entityFamilies[family].insert(entity).second;
    }

    /**
     * @brief Wyrejestruj Obiekt.
     *
     * @param[in] entity   Obiekt do wyrejestrowania
     */
    inline void unregisterEntity(Entity entity) {
        for (auto i = _entityFamilies.begin(); i < _entityFamilies.end(); i++) {
            i->erase(entity);
        }
    }

    /**
     * @brief Sprawdź czy System zarejestrował ten Obiekt.
     *
     * @param[in] entity   Id Obiektu do znalezienia
     *
     * @return true jeśli znaleziono poszukiwany Obiekt.
     */
    inline bool hasEntity(Entity entity) const {
        bool found;
        for (auto i = _entityFamilies.begin(); i < _entityFamilies.end(); i++)
        {
            found &= (i->end() != i->find(entity));
            if(found) return found;
        }
        return found;
    }

    /**
     * @brief Sprawdź czy System zarejestrował ten Obiekt w określonej rodzinie Obiektów.
     *
     * @param[in] entity   Id Obiektu do znalezienia
     * @param[in] family    Numer rodziny Obiektów
     *
     * @return true jeśli znaleziono poszukiwany Obiekt.
     */
    inline bool hasEntity(Entity entity, int family) const {
        return (_entityFamilies[family].end() != _entityFamilies[family].find(entity));
    }

    /**
     * @brief Funkcja wykonująca obliczenia na wszystkich adekwatnych Obiektach.
     *
     * @param[in] frameTime  Czas który upłynął od ostatniego wywołania funkcji, w sekundach.
     *
     * @return Liczba uaktualnionych Obiektów.
     */
    virtual void update(int64_t frameTime) = 0;

protected:
    /**
     * @brief Określ zbiór zbiorów typów Komponentów wymaganych przez System.
     *
     * @param[in] aRequiredComponents   Zbiór Komponentów wymaganych przez System.
     */
    inline void addRequiredComponentSet(ComponentTypeSet&& aRequiredComponents) {
        _requiredComponents.push_back(std::move(aRequiredComponents));
        _entityFamilies.push_back(std::set<Entity>());
    }

    /**
     * @brief Odniesienie do Silnika gry (Engine), potrzebne, żeby uzyskać dostęp do Komponentów Obiektów.
     */
    Engine&            _engine;

    /**
     * @brief Zbiór "rodzin" adekwatnych Obiektów (posiadających wszystkie Komponenty wymagane przez System).
     */
    std::vector<std::set<Entity>>    _entityFamilies;


private:
    /**
     * @brief Zbiór zbiorów typów Komponentów wymaganych przez System (uogólnienie dla różnych systemów, które mogą wykonywać operacje przyjmujące jako argumenty Obiekty z różnymi Komponentami)
     */
    std::vector<ComponentTypeSet>    _requiredComponents;

};

} // namespace ecs

#endif