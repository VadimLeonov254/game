#pragma once

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "climbing.h"
#include<string>

class Environment{
public:

    Model skybox;
    
    void LoadSkybox(const std::string& path);
    
    
    Model ground;
    
    void LoadGround(Shader& shader);
};

#endif
