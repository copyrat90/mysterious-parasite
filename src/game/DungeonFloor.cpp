#include "game/DungeonFloor.hpp"

#include "bn_assert.h"

#include "iso_bn_random.h"

namespace mp::game
{

namespace
{
constexpr s32 FLOOR_WIDTH = consts::DUNGEON_FLOOR_SIZE.width();
constexpr s32 FLOOR_HEIGHT = consts::DUNGEON_FLOOR_SIZE.height();
} // namespace

DungeonFloor::DungeonFloor() : _seeds({0, 0, 0})
{
}

DungeonFloor::Type DungeonFloor::getTile(s32 x, s32 y)
{
    BN_ASSERT(0 <= x && x < FLOOR_WIDTH, "Tile OOB for x(", x, ")");
    BN_ASSERT(0 <= y && y < FLOOR_HEIGHT, "Tile OOB for y(", y, ")");

    return _walls[y][x];
}

void DungeonFloor::generate(iso_bn::random& rng)
{
    // Save current seed to generate this floor identically for the loaded game.
    _seeds = {rng.seed_x(), rng.seed_y(), rng.seed_z()};

    _generateByRoomAddition(rng);
}

void DungeonFloor::generate(u32 seed_x, u32 seed_y, u32 seed_z)
{
    iso_bn::random rng;
    rng.set_seed(seed_x, seed_y, seed_z);

    generate(rng);
}

/**
 * @brief Generate random dungeon floor by Room Addition Algorithm.
 * See https://github.com/AtTheMatinee/dungeon-generation for details.
 * See `licenses/dungeon-generation.txt` file for the license info.
 */
void DungeonFloor::_generateByRoomAddition(iso_bn::random& rng)
{
    // test
    for (s32 y = 0; y < ROWS; ++y)
        for (s32 x = 0; x < COLUMNS; ++x)
            _walls[y][x] = rng.get_int(5) ? Type::FLOOR : Type::WALL;
    // test
}

} // namespace mp::game
