#pragma once
#ifndef CLIMBING_H
#define CLIMBING_H
#include "raylib.h"
#include "hold.h"
#include "player.h"
#include "cube.h"
#include<vector>
#include<string>
#include "json.hpp"
#include "filesystem"
#include<fstream>


class Cube;

class Hold;

class Climbing{
public:
    
    bool foundHold;
    void update(Player& player, std::vector<Chunk>& chunk);
    void drawLimbs(Player& player);
    
    bool isGenerated = false;

    std::vector<Hold> holds;

    Cube generateWall();
    Cube generateWallFromFile(std::string map_file);
};

#endif
