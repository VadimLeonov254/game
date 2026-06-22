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

Cube::Cube(Vector3 pos, float w, float h, float l, Color c) : position(pos), width(w), height(h), length(l), color(c) {}

void Cube::draw(){
    DrawCube(position, width, height, length, color);
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

    Vector3 lastPos = {position.x, -height/2, distW(gen)};

    while(lastPos.y < height/2){        
        Vector3 newPos = {position.x, lastPos.y + distY(gen), lastPos.z + distZ(gen)};
        
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
            holds.push_back(Hold(this, lastPos, LIGHTGRAY));
            if(disp < height/2 && diss == 2){
                holds.push_back(Hold(this, (Vector3){lastPos.x, lastPos.y + disp, lastPos.z + disp}, LIGHTGRAY));
                holds.push_back(Hold(this, (Vector3){lastPos.x, lastPos.y + disp, lastPos.z + disp}, LIGHTGRAY));
            }
        }
    }

    Route route(holds);

    routes.push_back(route.holds_route);
    
    for(int i = 0; i < holds.size(); i++){
        holds[i].parentHold = std::make_shared<Route>(route);
    }

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
