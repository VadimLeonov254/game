#pragma once
#ifndef UI_H
#define UI_H
#include "raylib.h"
#include "player.h"
#include "cube.h"
#include "chunk.h"
#include "hold.h"
#include<vector>

class Player;
class Cube;
class Chunk;
class Hold;

class Ui{
public:
    
    Vector2 lastPos;
    Vector2 nextPos;

    std::vector<Hold> holds_map;

    int tot = 0;
    
    bool mapIsClosed = true;

    void drawCrosshair(Player& player);
    
    void drawDebug(Player& player, Cube& wall, Chunk& ch);
    
    void drawMap(Cube& wall);

};

#endif
