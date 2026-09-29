#pragma once

#ifndef ROUTE_H
#define ROUTE_H
#include<vector>
#include "hold.h"

class Hold;

class Route{
public:   

    std::vector<Hold> holds_route;
    
    bool isHovered = false;

    float grade = 0;
    
    Route(std::vector<Hold> holds);

};

#endif
