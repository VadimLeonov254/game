#include<iostream>
#include "raylib.h"
#include "raymath.h"
#include "climbing.h"
#include "player.h"
#include "hold.h"
#include<random>

Cube Climbing::generateWall(){
        
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-10.0f, 100.0f);
    

    float h = std::fabs(dist(gen));
    float l = std::fabs(dist(gen));
    
    std::uniform_int_distribution<> r(1, l/4);
    
    int num = r(gen);
    Cube wall({dist(gen), h/2, dist(gen)}, 1, h, l, GRAY);
    
    for(int i = 0; i < num; i++){
        wall.generateRoute();
    }
    
    return wall;

}

void Climbing::update(Player& player, std::vector<Chunk>& chunks){
        Vector2 mousePos = GetMousePosition();
        
        Ray ray = GetScreenToWorldRay(GetMousePosition(), player.camera);
        
        int chosenHold = -1; 
        
        float closestRay = INFINITY;
       
        int lastChunk= player.currChunk;

        player.currChunk = player.position.y/10;

        chunks[player.currChunk].isActive = true; 

        bool foundHold = false;
        Vector3 holdPointShoulders = {0};
        Vector3 holdPointHips = {0};

        Vector3 shouldersPoint = {player.position.x, player.position.y - 0.3f, player.position.z};
        Vector3 hipsPoint = {player.position.x, player.position.y - 1.1f, player.position.z}; 
        
        bool checkLegs = player.onHoldRL || player.onHoldLL;
       
                std::vector<Hold> holds = chunks[player.currChunk].holds_chunk;
                if(player.currChunk != 0){
                    holds.insert(holds.end(), chunks[player.currChunk].holds_chunk.begin(), chunks[player.currChunk].holds_chunk.end());
                }
                for(int i = 0; i < holds.size(); i++){
                    float distP = Vector3Distance(player.position, holds[i].position);
                    if(distP > 1.5f){
                        continue;
                    }
                    if(player.selectingRA || player.selectingLA){
                        float distShoulders = Vector3Distance(shouldersPoint, holds[i].position);
                        RayCollision collision = GetRayCollisionBox(ray, holds[i].getBoundingBox());
                        if(collision.hit && distShoulders <= player.limbLength){
                            if(collision.distance < closestRay){
                                closestRay = collision.distance;
                                holdPointShoulders = collision.point; 
                                foundHold = true;
                            }
                        }
                    }

                    if(player.selectingRL || player.selectingLL){
                        float distHips = Vector3Distance(hipsPoint, holds[i].position);
                        RayCollision collision = GetRayCollisionBox(ray, holds[i].getBoundingBox());
                        if(collision.hit && distHips <= player.limbLength){
                            if(collision.distance < closestRay){
                                closestRay = collision.distance;
                                holdPointHips = collision.point; 
                                foundHold = true;
                            }
                        }
                    }
                }

                if(IsKeyPressed(KEY_E)){
                    if(!player.onHoldRA){
                        player.selectingRA = !player.selectingRA;
                        if(player.selectingRA){
                            SetMousePosition(GetScreenWidth()/2, GetScreenHeight()/2); 
                        }
                    }else{
                        player.onHoldRA = false;
                    }
                }

                if(IsKeyPressed(KEY_Q)){
                    if(!player.onHoldLA){
                        player.selectingLA = !player.selectingLA;
                        if(player.selectingLA){
                            SetMousePosition(GetScreenWidth()/2, GetScreenHeight()/2); 
                        }
                    }else{
                        player.onHoldLA = false;
                    }
                }

                if(IsKeyPressed(KEY_C)){
                    if(!player.onHoldRL){
                        player.selectingRL = !player.selectingRL;
                        if(player.selectingRL){
                            SetMousePosition(GetScreenWidth()/2, GetScreenHeight()/2); 
                        }
                    }else{
                        player.onHoldRL = false;
                    }
                }   

                if(IsKeyPressed(KEY_Z)){
                    if(!player.onHoldLL){
                        player.selectingLL = !player.selectingLL;
                        if(player.selectingLL){
                            SetMousePosition(GetScreenWidth()/2, GetScreenHeight()/2); 
                        }
                    }else{
                        player.onHoldLL = false;
                    }
                }
 

                if(foundHold && player.selectingRA && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    player.onHoldRA = true;
                    player.grabPointRA = holdPointShoulders;
                    player.selectingRA = false;
                    player.velocity = {0,0,0};
                }

                if(foundHold && player.selectingLA && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    player.onHoldLA = true;
                    player.grabPointLA = holdPointShoulders;
                    player.selectingLA = false;
                    player.velocity = {0,0,0};
                }

                if(foundHold && player.selectingRL && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    player.onHoldRL = true;
                    player.grabPointRL = holdPointHips;
                    player.selectingRL = false;
                    player.velocity = {0,0,0};
                }

                if(foundHold && player.selectingLL && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    player.onHoldLL = true;
                    player.grabPointLL = holdPointHips;
                    player.selectingLL = false;
                    player.velocity = {0,0,0};
                }
}

void Climbing::drawLimbs(Player& player){
    

    if(player.onHoldRL){
        DrawCylinderEx((Vector3){player.position.x, player.position.y - 0.9f, player.position.z - 0.1f}, player.grabPointRL, 0.03f, 0.04f, 8, Fade(BLACK, 0.5f));
        DrawCube(player.grabPointRL, 0.1f, 0.1f, 0.1f, BLACK);
    }

    if(player.onHoldLL){
        DrawCylinderEx((Vector3){player.position.x, player.position.y - 0.9f, player.position.z + 0.1f}, player.grabPointLL, 0.03f, 0.04f, 8, Fade(BLACK, 0.5f));
        DrawCube(player.grabPointLL, 0.1f, 0.1f, 0.1f, BLACK);
    }

    if(player.onHoldRA){
        DrawCylinderEx((Vector3){player.position.x, player.position.y - 0.1f, player.position.z - 0.15f}, player.grabPointRA, 0.03f, 0.04f, 8, Fade(BLACK, 0.5f));
        DrawCube(player.grabPointRA, 0.1f, 0.1f, 0.1f, BLACK);
    }

    if(player.onHoldLA){
        DrawCylinderEx((Vector3){player.position.x, player.position.y - 0.1f, player.position.z + 0.1f}, player.grabPointLA, 0.03f, 0.04f, 8, Fade(BLACK, 0.5f));
        DrawCube(player.grabPointLA, 0.1f, 0.1f, 0.1f, BLACK);
    }
}
