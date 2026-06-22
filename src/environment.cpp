#include<iostream>
#include "environment.h"
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif


Model Environment::LoadSkybox(const std::string& path){

    Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    Model skybox = LoadModelFromMesh(cube);    
    
    bool useHDR = false;

    skybox.materials[0].shader = LoadShader(TextFormat("res/shaders/skybox.vs", GLSL_VERSION),
                                            TextFormat("res/shaders/skybox.fs", GLSL_VERSION));
    
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "environmentMap"), (int[1]){ MATERIAL_MAP_CUBEMAP }, SHADER_UNIFORM_INT);
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "doGamma"), (int[1]){ useHDR? 1 : 0 }, SHADER_UNIFORM_INT);
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "vflipped"), (int[1]){ useHDR? 1 : 0 }, SHADER_UNIFORM_INT);

    Shader shdrCubemap = LoadShader(TextFormat("res/shaders/cubemap.vs", GLSL_VERSION),
                                    TextFormat("res/shaders/cubemap.fs", GLSL_VERSION));

    Image image = LoadImage("res/skybox.png");
    skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(image, CUBEMAP_LAYOUT_AUTO_DETECT);
    UnloadImage(image);
    
    return skybox;

}


