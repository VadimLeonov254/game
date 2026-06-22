#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include "raylib.h"
#include "cube.h"

class Cube;

class Player{
public:
    
    int currChunk;

    float yaw = 0.0f;
    float pitch = 0.0f;
    
    float currHeight = position.y - 1.8f;

    bool onHoldLA = false;
    bool onHoldRA = false;
    bool onHoldRL = false;
    bool onHoldLL = false;

    bool stretchedLeft = false;
    bool stretchedRight = false;

    bool selectingRA = false;
    bool selectingLA = false;
    bool selectingRL = false;
    bool selectingLL = false;

    bool isTopped = true;

    Vector3 grabPointLA;
    Vector3 grabPointRA; 
    Vector3 grabPointRL;
    Vector3 grabPointLL;

    Vector3 forward; 
    Vector3 position;
    Vector3 prevPos;
    Vector3 nextPos;
    Vector3 velocity {0.0f,0.0f,0.0f};
    float limbLength = 0.9f;
    
    Vector3 shoulders;
    Vector3 hips;

    Camera3D camera = {0};
    bool isGrounded;
    Player();
    void update(float dt);
    void applyMovement(float dt);
    void applyCollision(float dt, Cube& wall);
    BoundingBox MakeBox(Vector3 pos, float halfSize);
};

#endif
