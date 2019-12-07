/*
 * Copyright (C) 2008-2019 TrinityCore <https://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Random_moshou.h"
#include "SFMT.h"
#include <random>

static SFMTRand sfmtRand;
static SFMTEngine engine;

static SFMTRand* GetRng()
{
	return &sfmtRand;
}

int32_t irand(int32_t min, int32_t max)
{
    return int32_t(GetRng()->IRandom(min, max));
}

uint32_t urand(uint32_t min, uint32_t max)
{
    return GetRng()->URandom(min, max);
}

float frand(float min, float max)
{
    return float(GetRng()->Random() * (max - min) + min);
}

uint32_t rand32()
{
    return GetRng()->BRandom();
}

double rand_norm()
{
    return GetRng()->Random();
}

double rand_chance()
{
    return GetRng()->Random() * 100.0;
}

uint32_t urandweighted(size_t count, double const* chances)
{
    std::discrete_distribution<uint32_t> dd(chances, chances + count);
    return dd(SFMTEngine::Instance());
}

SFMTEngine& SFMTEngine::Instance()
{
    return engine;
}
