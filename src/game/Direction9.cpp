#include "game/Direction9.hpp"

#include "bn_keypad.h"

namespace mp::game
{

static Direction9 _getDirectionFromKeyInput(bool (*keyInput)(bn::keypad::key_type key));

Direction9 getDirectionFromKeyPress()
{
    return _getDirectionFromKeyInput(bn::keypad::pressed);
}

Direction9 getDirectionFromKeyHold()
{
    return _getDirectionFromKeyInput(bn::keypad::held);
}

Direction9 getDirectionFromKeyRelease()
{
    return _getDirectionFromKeyInput(bn::keypad::released);
}

static Direction9 _getDirectionFromKeyInput(bool (*keyInput)(bn::keypad::key_type key))
{
    using KeyType = bn::keypad::key_type;

    BoardPos pos = {0, 0};
    if (keyInput(KeyType::UP))
        pos += convertDir9ToPos(Direction9::UP);
    if (keyInput(KeyType::DOWN))
        pos += convertDir9ToPos(Direction9::DOWN);
    if (keyInput(KeyType::LEFT))
        pos += convertDir9ToPos(Direction9::LEFT);
    if (keyInput(KeyType::RIGHT))
        pos += convertDir9ToPos(Direction9::RIGHT);

    return convertPosToDir9(pos);
}

} // namespace mp::game
