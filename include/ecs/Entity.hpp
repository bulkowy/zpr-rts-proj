#ifndef _ENTITY_
#define _ENTITY_

namespace ecs {

/**
 * @brief   Obiekt (Entity) to dodatnie id, będące jednoznaczym identyfikatorem danego obiektu w grze. Dane Obiektu są reprezentowane przez Komponenty "Components", logika obiektów wykonywana przez Systemy.
 * 
 *  Obiekt (Entity) identyfikuje obiekt, ale nie posiada żadnych danych sam z siebie, ani żadnej logiki.
 * Jest określana jako zbiór Komponentów (Components), przetwarzana i uaktualniana przez Systemy (Systems).
 */
typedef unsigned int Entity;

/**
 * @brief   Jako że Obiekty to dodatnie id, id = 0 jest znacznikiem niepoprawnego obiektu.
 * 
 */
static const Entity _invalidEntity = 0;

} // namespace ecs

#endif