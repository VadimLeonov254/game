#pragma once

#ifndef ROUTE_H
#define ROUTE_H
#include<vector>
#include "hold.h"

class Hold;

class Route{
public:   

    std::vector<Hold> holds_route;

    float grade;
    
    Route(std::vector<Hold> holds);

};

#endif
