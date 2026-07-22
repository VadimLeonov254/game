#include<iostream>
#include "hold.h"
#include "cube.h"
#include<random>

Hold::Hold(Vector3 wallPos, Vector3 offset, Color color): position(wallPos), offset(offset), color(color), isVisible(true){
    position.x = offset.x + width/2;
    position.y = position.y + offset.y;
    position.z = position.z + offset.z;
}



Vector3 Hold::getWorldPosition(){
    //std::cout << "wallX: " << position.x + width/2 << '\n';
    return position;
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
