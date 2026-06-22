#pragma once
#ifndef CLIMBING_H
#define CLIMBING_H
#include "raylib.h"
#include "hold.h"
#include "player.h"
#include "cube.h"
#include<vector>

class Cube;

class Climbing{
public:
    
    bool foundHold;
    void update(Player& player, std::vector<Chunk>& chunk);
    void drawLimbs(Player& player);

    Cube generateWall();
};

#endif
