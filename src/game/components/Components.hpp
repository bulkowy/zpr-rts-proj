/**
 * Zbiorczy nagłówek dla wszystkich typów Komponentów.
 */

#ifndef _COMPONENTS_
#define _COMPONENTS_

#include "Health.hpp"
#include "Move.hpp"
#include "Position.hpp"
#include "Renderable.hpp"
#include "Selectable.hpp"

const ecs::ComponentType Health::_type = 1;
const ecs::ComponentType Move::_type = 2;
const ecs::ComponentType Position::_type = 3;
const ecs::ComponentType Renderable::_type = 4;
const ecs::ComponentType Selectable::_type = 5;

#endif