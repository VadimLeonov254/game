#pragma once

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "climbing.h"
#include<string>

class Environment{
public:

    Model LoadSkybox(const std::string& path);

};

#endif
