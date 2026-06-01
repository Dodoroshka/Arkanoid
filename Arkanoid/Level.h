#pragma once
#include <vector>
#include "Block.h"

class Level {
public:
    std::vector<Block> loadLevel(int level);
};