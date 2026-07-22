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
    DrawCube(position, width, height, length, color);
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
                holds.push_back(Hold(position, (Vector3){(float)hold["holdX"], (float)hold["holdY"] - position.y, (float)hold["holdZ"] - position.z}, LIGHTGRAY));
            }

            Route route(holds);

            routes.push_back(route.holds_route);
    
            for(int i = 0; i < holds.size(); i++){
                holds[i].parentRoute = std::make_shared<Route>(route);
            }
        }

    }catch(const json::parse_error& e){
        std::cerr << "Could not parse the file" << e.what() << '\n';
    }

}

void Cube::generateRoute(){

    float maxDist = 0.0f;

    std::vector<Hold> holds;

    std::random_device rd;
    std::mt19937 gen(rd());

    float margin = 0.5f;

    std::uniform_real_distribution<float> distZ(-1.0f, 1.0f);
    std::uniform_real_distribution<float> distY(0.3f, 1.0f);
    
    std::uniform_real_distribution<float> distR(-1.3f, 1.3f);
    
    std::uniform_real_distribution<float> distW(-length/2.1f, length/2.1f);

    std::uniform_int_distribution<int> distS(1,2);
    

    Vector3 lastPos = {position.x + width/2, -height/2, distW(gen)};
    
    while(lastPos.y < height/2){        
        Vector3 newPos = {position.x + width/2, lastPos.y + distY(gen), lastPos.z + distZ(gen)};
        
        float disp = distR(gen);
        int diss = distS(gen);

        
        if(newPos.z > length/2){
            newPos.z = length/2 - 3;
        }
        if(newPos.z < -length/2){
            newPos.z = -length/2 + 3;
        }
        
        maxDist = std::max(maxDist, Vector3Distance(newPos, lastPos))/10;

        lastPos = newPos;

        if(lastPos.y <= height/2){
            Hold h = Hold(position, lastPos, LIGHTGRAY);
            holds.push_back(h);
            if(disp < height/2 && diss == 2){
                holds.push_back(Hold(position, (Vector3){lastPos.x, lastPos.y + disp, lastPos.z + disp}, LIGHTGRAY));
                holds.push_back(Hold(position, (Vector3){lastPos.x, lastPos.y + disp, lastPos.z + disp}, LIGHTGRAY));
            }
        }
    }
    
    //std::cout << "all good" << '\n';

    Route route(holds);

    routes.push_back(route.holds_route);
    
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
                if(routes[i].holds_route[j].position.y >= 10.0f*(k-1) && routes[i].holds_route[j].getWorldPosition().y < 10.0f*k){
                    temp.push_back(routes[i].holds_route[j]);
                }
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
