/**
 * Zbiorczy nagłówek dla wszystkich typów Komponentów.
 */

#ifndef _COMPONENTS_
#define _COMPONENTS_

#include "Health.hpp"
#include "Move.hpp"
#include "Position.hpp"
#include "Renderable.hpp"

const ecs::ComponentType Health::_type = 1;
const ecs::ComponentType Move::_type = 2;
const ecs::ComponentType Position::_type = 3;
const ecs::ComponentType Renderable::_type = 4;

#endif