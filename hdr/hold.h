#pragma once
#ifndef HOLD_H
#define HOLD_H
#include "raylib.h"
#include "cube.h"
#include "route.h"
#include<memory>

class Route;

class Cube;

class Hold{
    
public:
    Vector3 offset;

    float width = 0.05f;
    float height = 0.2f;
    float length = 0.2f;
    
    Color color;
    bool isVisible;
    
    std::shared_ptr<Route> parentRoute;

    Vector3 position;
    
    Hold(Vector3 wallPos, Vector3 offset, Color color);

    Vector3 getWorldPosition();
    void draw();
    BoundingBox getBoundingBox();
};

#endif
