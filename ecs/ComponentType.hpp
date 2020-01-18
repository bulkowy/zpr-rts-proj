#ifndef _COMPONENTTYPE_
#define _COMPONENTTYPE_

#include <set>

namespace ecs {

/**
 * @brief   Typ komponentu (ComponentType) - dodatnie id wyróżniające typ komponentu.
 */
typedef unsigned int ComponentType;

/**
 * @brief   Ponieważ ComponentType jest zawsze dodatnim id, id = 0 jest znacznikiem niepoprawnego komponentu.
 */
static const ComponentType _invalidComponentType = 0;

/**
 * @brief   Sorted List (set) of ComponentType (of an Entity, or required by a System)
 * @brief   Zbiór identyfikatorów ComponentType (określa komponenty zawierane przez obiekt (Entity) lub wymagane przez System)
 */
typedef std::set<ComponentType> ComponentTypeSet;

} // namespace ecs

#endif