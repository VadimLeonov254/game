#include "ui.h"
#include "player.h"
#include "raylib.h"
#include<iostream>

void Ui::drawCrosshair(Player& player){
    if(player.selectingLA){
        DrawCircle(GetScreenWidth() * 0.45, GetScreenHeight()*0.45, 3, RED);
    }else if(player.onHoldLA == true){
        DrawCircle(GetScreenWidth() * 0.45, GetScreenHeight()*0.45, 3, GREEN);
    }else if(player.onHoldLA == false){
        DrawCircle(GetScreenWidth() * 0.45, GetScreenHeight()*0.45, 3, BLACK);
    }

    if(player.selectingRA){
        DrawCircle(GetScreenWidth() * 0.55, GetScreenHeight()*0.45, 3, RED);
    }else if(player.onHoldRA == true){
        DrawCircle(GetScreenWidth() * 0.55, GetScreenHeight()*0.45, 3, GREEN);
    }else if(player.onHoldRA == false){
        DrawCircle(GetScreenWidth() * 0.55, GetScreenHeight()*0.45, 3, BLACK);
    }

    if(player.selectingRL){
        DrawCircle(GetScreenWidth() * 0.55, GetScreenHeight()*0.55, 3, RED);
    }else if(player.onHoldRL == true){
        DrawCircle(GetScreenWidth() * 0.55, GetScreenHeight()*0.55, 3, GREEN);
    }else if(player.onHoldRL == false){
        DrawCircle(GetScreenWidth() * 0.55, GetScreenHeight()*0.55, 3, BLACK);
    }

    if(player.selectingLL){
        DrawCircle(GetScreenWidth() * 0.45, GetScreenHeight()*0.55, 3, RED);
    }else if(player.onHoldLL == true){
        DrawCircle(GetScreenWidth() * 0.45, GetScreenHeight()*0.55, 3, GREEN);
    }else if(player.onHoldLL == false){
        DrawCircle(GetScreenWidth() * 0.45, GetScreenHeight()*0.55, 3, BLACK);
    }

        
    DrawCircle(GetScreenWidth()/2, GetScreenHeight()/2, 3, BLACK);
 
}

void Ui::drawDebug(Player& player, Cube& wall, Chunk& ch){
    DrawText(TextFormat("Number of chunks: %01i", wall.chunks.size()), 10, 130, 20, RED);
    DrawText(TextFormat("Current chunk: %01i", player.currChunk), 10, 150, 20, RED);
    DrawText(TextFormat("The number of holds in the chunk %01i is %01i", player.currChunk, ch.holds_chunk.size()), 10, 170, 20, RED);
    DrawText(TextFormat("The total number of holds is %01i", tot), 10, 210, 20, RED);

    DrawText(TextFormat("camera pos: %.2f, %.2f, %.2f", player.position.x, player.position.y, player.position.z), 10, 10, 20, RED);
    if(player.onHoldLA || player.onHoldRA){
        DrawText("Climber is on the wall!", 10, 30, 20, RED);
    }else{
        DrawText("Climber is not on the wall!", 10, 30, 20, RED);
    }
                    
    if(player.isGrounded){
        DrawText("Climber is on the ground!", 10, 70, 20, RED);
    }else{
        DrawText("Climber is not on the ground!", 10, 70, 20, RED);
    }

    DrawText(TextFormat("wall pos: %.2f, %.2f, %.2f", wall.position.x, wall.position.y, wall.position.z), 10, 50, 20, RED);
                   
    DrawText(TextFormat("%.2f", player.position.y - 1.8f), GetScreenWidth()/2, 30, 22, Fade(BLACK, 0.5f));

    DrawText(TextFormat("Number of routes on the wall: %01i", wall.routes.size()), 10, 90, 20, RED);
                    
    if(player.selectingLA || player.selectingRA || player.selectingLL || player.selectingRL){
        DrawText("Player is selecting!", 10, 110, 20, RED);
    }else{
        DrawText("Player is not selecting!", 10, 110, 20, RED);
    }

    DrawText(TextFormat("wall dimensions: %.2f, %.2f, %.2f", wall.width, wall.length, wall.height), 10, 230, 20, RED);

}

void Ui::drawMap(Cube& wall, Player& player){
    if(IsKeyPressed(KEY_TAB)){
        mapIsClosed = !mapIsClosed; 
    }
    
    float rel = wall.height/wall.length;

    float mapHeight = 0.75f * GetScreenHeight(); 
    float mapLength = mapHeight/rel;
    
    float relMap = wall.height/mapHeight; 
    

    if(mapIsClosed == false){
        DrawRectangle(GetScreenWidth() * 0.1f, GetScreenHeight() * 0.1f, GetScreenWidth() * 0.8f, GetScreenHeight() * 0.8f, (Color){40, 40, 40, 255});
        DrawRectangle(GetScreenWidth() * 0.115f, GetScreenHeight() * 0.125f, GetScreenWidth() * 0.77f, GetScreenHeight() * 0.75f, (Color){249, 251, 255, 255});
        
        DrawRectangle((GetScreenWidth() - mapLength)/2.0f, (GetScreenHeight() - mapHeight)/2.0f, mapLength, mapHeight, (Color){128, 138, 131, 255});
        
       /* 
        if(IsKeyPressed(KEY_U)){
            lol++;
        }
       */
        
        for(int i = 0; i < wall.routes.size(); i++){
            holds_map = wall.routes[i].holds_route;
            for(int k = 0; k < holds_map.size(); k += 2){
                
                float xNew = (holds_map[k].position.z + wall.length/2.0 - wall.position.z)/relMap + (GetScreenWidth() - mapLength)/2.0f;
                float yNew = (-holds_map[k].position.y + wall.height)/relMap - (GetScreenHeight() - mapHeight) + mapHeight/2.0f;

                if(k == 0){
                    lastPos = {xNew, yNew};
                }else{
                    
                    nextPos = {xNew, yNew};
                    
                    if(CheckCollisionPointLine(GetMousePosition(), lastPos, nextPos, 5)){
                        wall.routes[i].isHovered = true;
                        player.isHovering = true;
                        break;
                    }else{
                        wall.routes[i].isHovered = false;
                    }    
                    lastPos = nextPos;
                }
            }
        }

        for(int i = 0; i < wall.routes.size(); i++){
            holds_map = wall.routes[i].holds_route;
            for(int k = 0; k < holds_map.size(); k += 2){
                
                float xNew = (holds_map[k].position.z + wall.length/2.0 - wall.position.z)/relMap + (GetScreenWidth() - mapLength)/2.0f;
                float yNew = (-holds_map[k].position.y + wall.height)/relMap - (GetScreenHeight() - mapHeight) + mapHeight/2.0f;

                if(k == 0){
                    lastPos = {xNew, yNew};
                }else{
                    
                    nextPos = {xNew, yNew};
                  

                    if(wall.routes[i].isHovered == true && player.isHovering == true){ 
                        DrawLineEx(lastPos, nextPos, 2.9, RED);
                        Rectangle box = {GetMousePosition().x + 35, GetMousePosition().y, GetScreenWidth() * 0.15f, GetScreenWidth() * 0.15f};
                        DrawRectangleRec(box, BLACK);
                        
                        DrawTextEx(font, TextFormat("5.%01i", int(wall.routes[i].grade * 10)), (Vector2){GetMousePosition().x + 35, GetMousePosition().y}, 50.0f, 1.0f, WHITE);
                        std::cout << int(wall.routes[i].grade * 10) << '\n';
                    }else{
                        DrawLineEx(lastPos, nextPos, 2.5, BLACK);
                    } 
                    lastPos = nextPos;
                }
            }
        }

        float xNewPlayer = (player.position.z + wall.length/2.0 - wall.position.z)/relMap + (GetScreenWidth() - mapLength)/2.0f; 
        float yNewPlayer = (-player.position.y + wall.height)/relMap - (GetScreenHeight() - mapHeight) + mapHeight/2.0f;
        if(!player.isGrounded){
            DrawCircleV((Vector2){xNewPlayer, yNewPlayer}, 4.0f, RED);
        }

    }
}

