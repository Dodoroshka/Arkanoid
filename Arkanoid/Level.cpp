#include "Level.h"

std::vector<Block> Level::loadLevel(int level) {
    std::vector<Block> blocks;
    float startX = 50.0f, startY = 80.0f;
    float bw = 70.0f, bh = 25.0f, spacing = 5.0f;

    auto build = [&](const std::vector<std::vector<int>>& grid) {
        for (int row = 0; row < (int)grid.size(); ++row)
            for (int col = 0; col < (int)grid[row].size(); ++col)
                if (grid[row][col] > 0)
                    blocks.push_back(Block(
                        startX + col * (bw + spacing),
                        startY + row * (bh + spacing),
                        grid[row][col]
                    ));
        };

    if (level == 1) {
        build({
            {2,2,2,2,3,2,3,3,3,3},
            {2,4,2,3,2,3,2,3,4,3},
            {2,2,2,1,1,1,1,3,3,3},
            {4,1,4,1,4,1,4,1,4,1},
            {1,3,1,3,1,3,1,3,1,3}
            });
    }
    else if (level == 2) {
        build({
            {0,0,0,0,0,0,0,0,0,0},
            {4,4,4,3,0,0,3,4,4,4},
            {1,4,2,3,0,0,3,2,4,1},
            {4,1,3,0,0,0,0,3,1,4},
            {2,3,0,0,0,0,0,0,3,2},
            {3,0,0,0,0,0,0,0,0,3}
            });
    }
    else if (level == 3) {
        build({
            {0,2,3,4,0,0,4,3,2,0},
            {0,2,3,4,4,4,4,3,2,0},
            {0,1,3,3,4,4,3,3,1,0},
            {0,0,1,3,3,3,3,1,0,0},
            {0,0,0,1,1,1,1,0,0,0}
            });
    }
    else if (level == 4) {
        build({
            {4,0,0,0,0,0,0,0,0,0},
            {4,3,2,0,0,0,0,0,0,0},
            {4,3,2,1,4,0,0,0,0,0},
            {4,3,2,1,4,3,2,0,0,0},
            {4,3,2,1,4,3,2,1,4,0},
            {5,5,5,5,5,5,5,5,5,4}
            });
    }
    else if (level == 5) {
        build({
            {2,0,3,0,2,2,0,3,0,2},
            {0,3,0,4,4,4,4,0,3,0},
            {3,0,4,5,1,1,5,4,0,3},
            {0,4,5,1,0,0,1,5,4,0},
            {4,5,1,0,0,0,0,1,5,4},
            {5,1,0,0,0,0,0,0,1,5}
            });
    }
    return blocks;
}