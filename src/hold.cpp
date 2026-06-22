#include "hold.h"
#include "cube.h"
#include<random>

Hold::Hold(Cube* wall, Vector3 offset, Color color)
    : wall(wall), offset(offset), color(color), isVisible(true){}

Vector3 Hold::getWorldPosition(){
    position = {wall -> position.x + wall -> width/2 + width/2, wall -> position.y + offset.y, wall -> position.z + offset.z};
    return {
        wall->position.x + wall -> width/2 + width/2,
        wall->position.y + offset.y,
        wall->position.z + offset.z
    };
}

void Hold::draw(){

    Vector3 pos = getWorldPosition();
    DrawCube(pos, width, height, length, color);
}

BoundingBox Hold::getBoundingBox(){
    Vector3 pos = getWorldPosition();

    return {
        { pos.x - width/2, pos.y - height/2, pos.z - length/2 },
        { pos.x + width/2, pos.y + height/2, pos.z + length/2 }
    };
}
