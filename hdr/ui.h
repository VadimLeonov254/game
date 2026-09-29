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

    Font font = LoadFontEx("res/Naked Power Bd.otf", 100, NULL, 0);

    Vector2 lastPos;
    Vector2 nextPos;

    std::vector<Hold> holds_map;
    
    bool isRouteHovered = false;

    int tot = 0;
    
    int lol = 0;

    bool mapIsClosed = true;

    void drawCrosshair(Player& player);
    
    void drawDebug(Player& player, Cube& wall, Chunk& ch);
    
    void drawMap(Cube& wall, Player& player);

};

#endif
