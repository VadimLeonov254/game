#include<iostream>
#include "raylib.h"
#include "raymath.h"
#include "climbing.h"
#include "player.h"
#include "hold.h"
#include<random>
#include "json.hpp"
#include "filesystem"
#include<fstream>

using json = nlohmann::json;
namespace fs = std::filesystem;

Cube Climbing::generateWallFromFile(std::string map_file){
    
    map_file = "maps/" + map_file; 
    std::ifstream file(map_file);

    if(!file.is_open()){
        std::cerr << "Could not open the file" << '\n';
    }    

    
    Cube wall;

    wall.color = GRAY;

    json data = json::parse(file);
    try{
        wall.position = (Vector3){data["wall"]["wallX"], data["wall"]["wallY"], data["wall"]["wallZ"]};
        wall.width = data["wall"]["width"];
        wall.height = data["wall"]["height"];
        wall.length = data["wall"]["length"];
        
        wall.rappPosition = (Vector3){data["rapp"]["rappX"], data["rapp"]["rappY"], data["rapp"]["rappZ"]};

        wall.generateRoutesFromFile(map_file);

    }catch(const json::parse_error& e){
        std::cerr << "parse error" << e.what() << '\n';
    }
    
    return wall;

}

Cube Climbing::generateWall(){
        
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(10.0f, 20.0f);
    
    std::uniform_real_distribution<float> dim(25.0f, 100.0f);

    float h = std::fabs(dim(gen));
    float l = std::fabs(dim(gen));
    
    std::uniform_int_distribution<> r(1, l/4);
    std::uniform_real_distribution<float> distw(5.5f, 12.0f);

    int num = r(gen);
    //std::cout << num << '\n';
    Cube wall;
    
    wall.position = (Vector3){-dist(gen), h/2, dist(gen)/10};
    wall.width = 1.0f + distw(gen);
    wall.height = h;
    wall.length = l;
    wall.color = GRAY;
    
    Mesh wallMesh = GenMeshCube(wall.width, h, l);
    
    Model modelTemp = LoadModelFromMesh(wallMesh);

    modelTemp.materials[0].maps[MATERIAL_MAP_ROUGHNESS].value = 0.5f;

    wall.wallModel = modelTemp;


    wall.rappPosition = (Vector3){wall.position.x + wall.width/2.0f, wall.height, wall.position.z};

    for(int i = 0; i < num; i++){
        wall.generateRoute();
    }
    
    isGenerated = true;

    //std::cout << "wall position: " << wall.position.x << '\n' << wall.position.y << '\n' << wall.position.z << '\n';
    return wall;
    

}

void Climbing::update(Player& player, std::vector<Chunk>& chunks){
        Vector2 mousePos = (Vector2){GetScreenWidth()/2.0f, GetScreenHeight()/2.0f};
        
        Ray ray = GetScreenToWorldRay(mousePos, player.camera);
        
        int chosenHold = -1; 
        
        float closestRay = INFINITY;
       
        int lastChunk = player.currChunk;
        
        //std::cout << chunks.size() << '\n';

        player.currChunk = static_cast<int>(player.position.y / 10.0f);

        if(player.currChunk < 0)
            player.currChunk = 0;

        if(player.currChunk >= chunks.size())
            player.currChunk = chunks.size() - 1;

        if(player.position.y < 15.0f){
            chunks[0].isActive = true;
        }else{
            chunks[0].isActive = false;
        }
       
        chunks[player.currChunk].isActive = true;

        bool foundHold = false;
        Vector3 holdPointShoulders = {0};
        Vector3 holdPointHips = {0};

        Vector3 shouldersPoint = {player.position.x, player.position.y - 0.3f, player.position.z};
        Vector3 hipsPoint = {player.position.x, player.position.y - 1.1f, player.position.z}; 
        
        bool checkLegs = player.onHoldRL || player.onHoldLL;
        if(player.check == 0 || lastChunk != player.currChunk){
            holds.insert(holds.end(), chunks[player.currChunk].holds_chunk.begin(), chunks[player.currChunk].holds_chunk.end());
            player.check = 1;    
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
