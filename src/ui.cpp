#include "ui.h"
#include "player.h"
#include "raylib.h"

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

DrawText(TextFormat("camera pos: %.2f, %.2f, %.2f", player.position.x, player.position.y, player.velocity.z), 10, 10, 20, RED);
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


}

void Ui::drawMap(Cube& wall){
    if(IsKeyPressed(KEY_TAB)){
        mapIsClosed = !mapIsClosed; 
    }
       
    if(mapIsClosed == false){
        DrawRectangle(GetScreenWidth() * 0.1f, GetScreenHeight() * 0.1f, GetScreenWidth() * 0.8f, GetScreenHeight() * 0.8f, (Color){100, 100, 200, 255});
        DrawRectangle(GetScreenWidth() * 0.15f, GetScreenHeight() * 0.15f, GetScreenWidth() * 0.7f, GetScreenHeight() * 0.7f, (Color){249, 251, 255, 255});
        for(int i = 0; i < wall.routes.size(); i++){
            holds_map = wall.routes[i].holds_route;
            for(int k = 0; k < holds_map.size(); k+=10){
                float xNew = (holds_map[k].getWorldPosition().z - wall.position.z) * GetScreenWidth() * 0.7f / wall.length;
                float yNew = (holds_map[k].getWorldPosition().y * GetScreenHeight() - wall.position.y) * 0.7f / wall.height;

                if(k == 0){
                    lastPos = {GetScreenWidth() * 0.5f - xNew, GetScreenHeight() * 0.85f - yNew};
                }else{
                    nextPos = {GetScreenWidth() * 0.5f - xNew, GetScreenHeight() * 0.85f - yNew};
                    DrawLineEx(lastPos, nextPos, 2.5, BLACK);
                    lastPos = nextPos;
                }

            }
        }
    }
}

