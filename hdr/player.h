#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include "raylib.h"
#include "cube.h"
#include<vector>

class Cube;

class Player{
public:
    
    int currChunk;
    
    int savedMaps = 0;

    float yaw = 0.0f;
    float pitch = 0.0f;
    
    int check = 0;

    float currHeight = position.y - 1.8f;

    bool onHoldLA = false;
    bool onHoldRA = false;
    bool onHoldLL = false;
    bool onHoldRL = false;
    
    float staminaLA = 1.0f;
    float staminaRA = 1.0f;
    float staminaLL = 1.0f;
    float staminaRL = 1.0f;

    bool stretchedLeft = false;
    bool stretchedRight = false;

    bool selectingRA = false;
    bool selectingLA = false;
    bool selectingRL = false;
    bool selectingLL = false;

    bool isTopped = true;
    
    bool createMap = false;
    bool loadMap = false;

    bool isRappelling = false;

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
    void applyMovementRappelling(float dt);
    void applyCollision(float dt, Cube& wall);
    BoundingBox MakeBox(Vector3 pos, float halfSize);

    
    void takeToTheTop(Cube& wall);
    void rappDown(Cube& wall);      
    
    void savePlayerToFile(std::string map_file);
    void loadPlayerFromFile(std::string map_file); 

};

#endif
