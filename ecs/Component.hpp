#ifndef _COMPONENT_
#define _COMPONENT_

#include "ComponentType.hpp"

namespace ecs {

/**
 * @brief   Komponent (Component) jest reprezentacją danych jednego z aspektów obiektu (Entity).
 *
 *  Komponent jest strukturą danych, która przechowuje informacje o pewnym aspekcie danego obiektu (Entity).
 * Stan danego obiektu (Entity) jest w całości opisywany przez kilka komponentów.
 *
 *  Każda klasa określająca komponent musi dziedziczyć po tej strukturze i zdefiniować swój własny, unikalny #ComponentType.
 */
struct Component {
};

} // namespace ecs

#endif