#pragma once

#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"


class Button{
    
public:

    float width;
    float height;
    float posX;
    float posY; 

    Color color;
    
    Rectangle rec;
    
    Vector2 textDimensions = {0.0f, 0.0f};
    
    Vector2 textPosition = {0.0f, 0.0f};

    //float fontSize;

    Button(float width, float height, float posX, float posY, Color color);

    void drawButton();
    
    void drawButtonText(float floatSize, Font font, std::string text, std::string horAllign, std::string vertAllign, Color textColor);

    bool isClicked();

};

#endif
