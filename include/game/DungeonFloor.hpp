#pragma once

#include "bn_array.h"

#include "constants.hpp"

namespace iso_bn
{
class random;
}

namespace mp::game
{

class DungeonFloor final
{
public:
    enum class Type : u8
    {
        FLOOR = 0,
        WALL = 1
    };

    static constexpr s32 ROWS = consts::DUNGEON_FLOOR_SIZE.height();
    static constexpr s32 COLUMNS = consts::DUNGEON_FLOOR_SIZE.width();

private:
    bn::array<u32, 3> _seeds;
    bn::array<bn::array<Type, COLUMNS>, ROWS> _walls;

public:
    DungeonFloor();

    Type getTile(s32 x, s32 y);

    /**
     * @brief Generate random dungeon floor.
     */
    void generate(iso_bn::random& rng);

    /**
     * @brief Generate random dungeon floor with custom seeds.
     * This is meant to be used for the loaded game.
     *
     * @param seed_x `iso_bn::random` internal seed x.
     * @param seed_y `iso_bn::random` internal seed y.
     * @param seed_z `iso_bn::random` internal seed z.
     */
    void generate(u32 seed_x, u32 seed_y, u32 seed_z);

    auto getSeeds() const
    {
        return _seeds;
    }

private:
    /**
     * @brief Generate random dungeon floor by Room Addition Algorithm.
     * See https://github.com/AtTheMatinee/dungeon-generation for details.
     * See `licenses/dungeon-generation.txt` file for the license info.
     */
    void _generateByRoomAddition(iso_bn::random& rng);
};

} // namespace mp::game
