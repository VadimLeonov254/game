#include<iostream>
#include "environment.h"
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <random>

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif


void Environment::LoadSkybox(const std::string& path){

    Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    skybox = LoadModelFromMesh(cube);    
    
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
    
}

void Environment::LoadGround(Shader& shader){
    
    Mesh meshGround = GenMeshPlane(100.0f, 100.0f, 15.0f, 15.0f);

    ground = LoadModelFromMesh(meshGround);

    Texture2D texture_map = LoadTexture("res/green.png");

    ground.materials[0].shader = shader;
    ground.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture("res/green.png");
    ground.materials[0].maps[MATERIAL_MAP_NORMAL].texture = LoadTexture("res/grass_normal.png");
    
    
    GenTextureMipmaps(&ground.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture);
    GenTextureMipmaps(&ground.materials[0].maps[MATERIAL_MAP_NORMAL].texture);

    SetTextureFilter(ground.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture, TEXTURE_FILTER_TRILINEAR);
    SetTextureFilter(ground.materials[0].maps[MATERIAL_MAP_NORMAL].texture, TEXTURE_FILTER_TRILINEAR);


    ground.materials[0].maps[MATERIAL_MAP_METALNESS].value = 0.5f;

/*    Vector3 *vertices = (Vector3 *)meshGround.vertices;
    
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> distH(-0.3f, 0.3f);

    for(int i = 0; i < meshGround.vertexCount; i++){
        vertices[i].y = 1.0f + distH(gen);
    }

    UpdateMeshBuffer(meshGround, 0, meshGround.vertices, meshGround.vertexCount * 3 * sizeof(float), 0);
*/
}
