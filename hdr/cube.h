#pragma once
#ifndef CUBE_H
#define CUBE_H
#include "raylib.h"
#include "hold.h"
#include "route.h"
#include<vector>
#include "chunk.h"
#include "json.hpp"
#include "filesystem"
#include<fstream>

class Hold;
class Route;

class Cube{
public:
    Vector3 position;
    
    Vector3 rappPosition;

    Model wallModel;

    float width, height, length;
    Color color;

    std::vector<Route> routes;
    
    std::vector<Chunk> chunks;

    void generateRoute();
    
    void generateRoutesFromFile(std::string map_file);

    void draw();
    BoundingBox getBoundingBox();

    bool setChunks();
    

};

#endif
