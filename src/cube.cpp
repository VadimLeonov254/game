#include "cube.h"
#include "hold.h"
#include "chunk.h"
#include<iostream>
#include<random>
#include<vector>
#include<algorithm>
#include "raymath.h"
#include<utility>
#include<memory>
#include "json.hpp"
#include "filesystem"
#include<fstream>

using json = nlohmann::json;
namespace fs = std::filesystem;

void Cube::draw(){

    //BeginShaderMode(shader);
        DrawModel(wallModel, position, 1.0f, WHITE);
    //EndShaderMode();
}

void Cube::generateRoutesFromFile(std::string map_file){
    

    std::ifstream file(map_file);

    if(!file.is_open()){
        std::cerr << "Couldn't open file" << '\n';
    }
    
    std::vector<Hold> holds;
    

    json data = json::parse(file);

    try{
    
        for(const auto& routefile : data["routes"]){
            if(holds.size() > 0){
                holds.clear();
            }
            for(const auto& hold : routefile["holds"]){
                Hold h = Hold(position, (Vector3){(float)hold["holdX"], (float)hold["holdY"] - position.y, (float)hold["holdZ"] - position.z}, LIGHTGRAY);
                holds.push_back(h);
            }

            Route route(holds);

            routes.push_back(route);
    
            for(int i = 0; i < holds.size(); i++){
                holds[i].parentRoute = std::make_shared<Route>(route);
            }
        }
        
        for(const auto& hold : data["AdditionalHolds"]){
            if(additionalHolds.size() > 0){
                additionalHolds.clear();
            }
            
            Hold h = Hold(position, (Vector3){(float)hold["holdX"], (float)hold["holdY"] - position.y, (float)hold["holdZ"] - position.z}, LIGHTGRAY);
            additionalHolds.push_back(h);

        }

    }catch(const json::parse_error& e){
        std::cerr << "Could not parse the file" << e.what() << '\n';
    }

}

void Cube::generateRoute(){
    
    int outOfBound = 0;

    float maxDist = 0.0f;

    std::vector<Hold> holds;

    std::random_device rd;
    std::mt19937 gen(rd());

    float margin = 0.5f;

    std::uniform_real_distribution<float> distZ(-1.3f, 1.3f);
    std::uniform_real_distribution<float> distY(0.5f, 1.3f);
   
    std::uniform_real_distribution<float> distAddY(-0.5f, 1.3f);

    std::uniform_real_distribution<float> distR(-1.3f, 1.3f);
    
    std::uniform_real_distribution<float> distW(-length/2.0f, length/2.0f);

    std::uniform_int_distribution<int> distS(1,2);
    

    Vector3 lastPos = {position.x - width/2, -height/2, position.z + distW(gen)};
    
    while(lastPos.y < height/2){        
        Vector3 newPos = {position.x - width/2, lastPos.y + distY(gen), lastPos.z + distZ(gen)};
        
        float disp = distR(gen);
        int diss = distS(gen);

        if(maxDist < Vector3Distance(newPos, lastPos)){
            maxDist = Vector3Distance(newPos, lastPos);
        }

        lastPos = newPos;
        
        Color col;        

        if(lastPos.y + 0.2f <= height/2){

            if(lastPos.z >= length/2 - 0.1f){
                lastPos.z -= 2*(lastPos.z - length/2) + 0.2f;
            }
            
            if(lastPos.z <= -length/2 + 0.1f){
                lastPos.z += 2*(-length/2 - lastPos.z) + 0.2f;
            }

            Hold h = Hold(position, lastPos, LIGHTGRAY);
            holds.push_back(h);
        }

        Vector3 addPos = {lastPos.x, lastPos.y + distAddY(gen), lastPos.z + distZ(gen)};

        if(addPos.y + 0.2f <= height/2){

            if(addPos.z >= length/2 - 0.1f){
                addPos.z -= 2*(addPos.z - length/2) + 0.2f;
            }
            
            if(addPos.z <= -length/2 + 0.1f){
                addPos.z += 2*(-length/2 - addPos.z) + 0.2f;
            }

            Hold h = Hold(position, addPos, LIGHTGRAY);
            additionalHolds.push_back(h);
        }
        

    }
    
    //std::cout << "all good" << '\n';

    Route route(holds);
   
    route.grade = maxDist;
    
    std::cout << "weeell " << route.grade << '\n';

    std::cout << outOfBound << '\n';

    routes.push_back(route);
    
    for(int i = 0; i < holds.size(); i++){
        holds[i].parentRoute = std::make_shared<Route>(route);
    }
    
    //std::cout << "cooked" << '\n';
        
}

bool Cube::setChunks(){
    chunks.clear();
    for(int k = 1; k < 1+height/10; k++){
        std::vector<Hold> temp;
        for(int i = 0; i < routes.size(); i++){
            for(int j = 0; j < routes[i].holds_route.size(); j++){
                if(routes[i].holds_route[j].position.y >= 10.0f*(k-1) && routes[i].holds_route[j].position.y < 10.0f*k){
                    temp.push_back(routes[i].holds_route[j]);
                }
            }
        }
        for(int i = 0; i < additionalHolds.size(); i++){
            if(additionalHolds[i].position.y >= 10.0f*(k-1) && additionalHolds[i].position.y < 10.0f*k){
                temp.push_back(additionalHolds[i]);
            }
        }
        Chunk chunk;
        chunk.holds_chunk = std::move(temp);
        chunks.push_back(chunk);
    }
    return true;
}

BoundingBox Cube::getBoundingBox(){
    return {
        { position.x - width/2, position.y - height/2, position.z - length/2 },
        { position.x + width/2, position.y + height/2, position.z + length/2 }
    };
}
