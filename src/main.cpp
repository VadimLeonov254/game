#include<iostream>
#include "raylib.h"
#include "raymath.h"
#include<cmath>
#include "cube.h"
#include "player.h" 
#include "hold.h"
#include "climbing.h"
#include<vector>
#include<random>
#include<filesystem>
#include "rlgl.h"
#include "environment.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   
    #define GLSL_VERSION            100
#endif


int main(void){
    SetConfigFlags(FLAG_WINDOW_TOPMOST | FLAG_WINDOW_UNDECORATED | FLAG_MSAA_4X_HINT);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "the_climb"); 
    Player player;
    Climbing climbing;
    
    Environment env;

    bool mapIsClosed = true;
    
    Cube wall = climbing.generateWall();
    
    bool isSet = wall.setChunks();

    std::random_device rd;

    std::mt19937 gen(rd());

    std::vector<Hold> holds;
    
    std::vector<Hold> holds_map;
    
    int tot = 0;
    
    Vector2 lastPos;
    Vector2 nextPos;

    bool showMsg = false;
    
    player.camera.projection = CAMERA_PERSPECTIVE;
    
    Camera camera = player.camera;

    int cameraMode = CAMERA_FIRST_PERSON;
    
    Model skybox = env.LoadSkybox("res/skybox.png");

    DisableCursor();

    SetTargetFPS(60);
    
    Shader shader = LoadShader(TextFormat("res/shaders/normalmap.vs", GLSL_VERSION),
                               TextFormat("res/shaders/normalmap.fs", GLSL_VERSION));

    shader.locs[SHADER_LOC_MAP_NORMAL] = GetShaderLocation(shader, "normalMap");
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

    Vector3 lightPosition = { 10.0f, 10.0f, 10.0f };
    int lightPosLoc = GetShaderLocation(shader, "lightPos");    

    Image image = GenImagePerlinNoise(256, 256, 100, 100, 2.0f);

    Texture2D texture = LoadTextureFromImage(image);
    Texture2D texture_map = texture;
    
    Mesh mesh = GenMeshHeightmap(image, (Vector3){256,8,256});
    GenMeshTangents(&mesh);
    Model model = LoadModelFromMesh(mesh);

    texture_map = LoadTexture("res/green.png");
    
    model.materials[0].shader = shader;
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture("res/green.png");
    model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = LoadTexture("res/grass_normal.png");
    
    GenTextureMipmaps(&model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture);
    GenTextureMipmaps(&model.materials[0].maps[MATERIAL_MAP_NORMAL].texture);

    SetTextureFilter(model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture, TEXTURE_FILTER_TRILINEAR);
    SetTextureFilter(model.materials[0].maps[MATERIAL_MAP_NORMAL].texture, TEXTURE_FILTER_TRILINEAR);
   
    float specularExponent = 1.0f;
    int specularExponentLoc = GetShaderLocation(shader, "specularExponent");

    int useNormalMap = 0;
    int useNormalMapLoc = GetShaderLocation(shader, "useNormalMap"); 
    
    Vector3 mapPosition = {-50.0f,-5.0f, -50.0f};
    

    while(!WindowShouldClose()){


        if(player.isTopped == true && player.currHeight == 0){
            wall = climbing.generateWall();
            isSet = wall.setChunks();
            player.isTopped = false;
            for(int i = 0; i < wall.routes.size(); i++){
                for(int k = 0; k < wall.routes[i].holds_route.size(); k++){
                    tot+=1;
                }
            }
        }
        float dt = GetFrameTime();
        
        Vector3 prevPos = player.position;
        Vector3 prevVel = player.velocity;
        
        if(mapIsClosed == true){
            player.update(dt);
            DisableCursor();
            player.applyMovement(dt);
            player.applyCollision(dt, wall);
        }else{
            ShowCursor();
        } 
        climbing.update(player, wall.chunks);


        float lightPos[3] = {lightPosition.x, lightPosition.y, lightPosition.z};
        SetShaderValue(shader, lightPosLoc, lightPos, SHADER_UNIFORM_VEC3);
        float camPos[3] = {camera.position.x, camera.position.y, camera.position.z};
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], camPos, SHADER_UNIFORM_VEC3);

        SetShaderValue(shader, specularExponentLoc, &specularExponent, SHADER_UNIFORM_FLOAT);

        SetShaderValue(shader, useNormalMapLoc, &useNormalMap, SHADER_UNIFORM_INT);


        BeginDrawing();

        ClearBackground(WHITE);

        BeginMode3D(player.camera);
            rlDisableBackfaceCulling();
            rlDisableDepthMask();
            DrawModel(skybox, (Vector3){0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
            rlEnableBackfaceCulling();
            rlEnableDepthMask();
        
            BeginShaderMode(shader);
               DrawModel(model, (Vector3){ -10.0f, -10.0f, -10.0f }, 1.0f, (Color) {255, 255, 255, 255});
            EndShaderMode();
 
            wall.draw();
            for(int k = 0; k < wall.routes.size(); k++){
                holds = wall.routes[k].holds_route;
                for(int i = 0; i < holds.size(); i++){
                    holds[i].draw();
                }
            }
            climbing.drawLimbs(player); 
       

        EndMode3D();
           
        Chunk ch = wall.chunks[player.currChunk];

        DrawText(TextFormat("Number of chunks: %01i", wall.chunks.size()), 10, 130, 20, RED);
        DrawText(TextFormat("Current chunk: %01i", player.currChunk), 10, 150, 20, RED);
        DrawText(TextFormat("The number of holds in the chunk %01i is %01i", player.currChunk, ch.holds_chunk.size()), 10, 170, 20, RED);
        DrawText(TextFormat("The total number of holds is %01i", tot), 10, 210, 20, RED);
       
        if(isSet){
            DrawText("Chunks has been generated!", 10, 190, 20, RED);
        }

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
        }



        EndDrawing();

    }
    

    CloseWindow();
}
