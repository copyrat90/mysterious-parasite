#include "game/mob/MonsterAction.hpp"

#include "bn_assert.h"

namespace mp::game::mob
{

MonsterAction::MonsterAction(Direction direction, Type type, s32 actionIdx)
    : _direction(direction), _type(type), _actionIdx(actionIdx)
{
    BN_ASSERT(type == Type::ABILITY || type == Type::ITEM, "actionIndex(", actionIdx, ") provided with type(",
              (s32)type, ")");
}

MonsterAction::MonsterAction(Direction direction, Type type) : _direction(direction), _type(type)
{
    BN_ASSERT(type != Type::ABILITY && type != Type::ITEM, "actionIndex not provided with type(", (s32)type, ")");
}

auto MonsterAction::getType() const -> Type
{
    return _type;
}

s32 MonsterAction::getActionIdx() const
{
    BN_ASSERT(_type == Type::ABILITY || _type == Type::ITEM, "_type(", (s32)_type, ") doesn't have actionIndex");
    return _actionIdx;
}

auto MonsterAction::getDirection() const -> Direction
{
    return _direction;
}

BoardPos MonsterAction::getDirectionPos() const
{
    switch (_direction)
    {
    case Direction::UP:
        return {0, -1};
    case Direction::UP_RIGHT:
        return {1, -1};
    case Direction::RIGHT:
        return {1, 0};
    case Direction::DOWN_RIGHT:
        return {1, 1};
    case Direction::DOWN:
        return {0, 1};
    case Direction::DOWN_LEFT:
        return {-1, 1};
    case Direction::LEFT:
        return {-1, 0};
    case Direction::UP_LEFT:
        return {-1, -1};
    }
    BN_ERROR("Invalid _direction(", (s32)_direction, ")");
    return {0, 0};
}

} // namespace mp::game::mob
