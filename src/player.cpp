#include<iostream>
#include "raylib.h"
#include "raymath.h"
#include "player.h"
#include<cmath>
#include "json.hpp"
#include "filesystem"
#include<fstream>

using json = nlohmann::json;

Player::Player() {
    position = {0.0f, 1.8f, 4.0f};
    camera.position = position;
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    isGrounded = true;
}

BoundingBox Player::MakeBox(Vector3 pos, float halfSize){
    return {
        (Vector3){ pos.x - halfSize, pos.y - halfSize, pos.z - halfSize },
        (Vector3){ pos.x + halfSize, pos.y + halfSize, pos.z + halfSize }
    };
}

void Player::loadPlayerFromFile(std::string map_file){
    
    map_file = "maps/" + map_file;
    std::ifstream file(map_file);

    if(!file.is_open()){
        std::cerr << "Could not open the file" << '\n';
    }
    
    json data = json::parse(file);
    
    try{
        position = (Vector3){data["player"]["x"], data["player"]["y"], data["player"]["z"]};
        
        if(data["onHoldLA"] == 1){
            onHoldLA = true;
        }else{
            onHoldLA = false;
        }

        if(data["onHoldRA"] == 1){
            onHoldRA = true;
        }else{
            onHoldRA = false;
        }

        if(data["onHoldLL"] == 1){
            onHoldLL = true;
        }else{
            onHoldLL = false;
        }

        if(data["onHoldRL"] == 1){
            onHoldRL = true;
        }else{
            onHoldRL = false;
        }

        grabPointLA = (Vector3){data["player"]["xLA"], data["player"]["yLA"], data["player"]["zLA"]};
        grabPointRA = (Vector3){data["player"]["xRA"], data["player"]["yRA"], data["player"]["zRA"]};
        grabPointLL = (Vector3){data["player"]["xLL"], data["player"]["yLL"], data["player"]["zLL"]};
        grabPointRL = (Vector3){data["player"]["xRL"], data["player"]["yRL"], data["player"]["zRL"]};

    }catch(const json::parse_error& e){
        std::cerr << "parse error" << e.what() << '\n';
    }
    
    if(file.is_open()){
        file.close();
    }

}

void Player::savePlayerToFile(std::string map_file){
    
    map_file = "maps/" + map_file;
    std::ifstream file(map_file);

    if(!file.is_open()){
        std::cerr << "Could not open the file" << '\n';
    }

    json data = json::parse(file);

    try{
    
    data["player"]["x"] = position.x;
    data["player"]["y"] = position.y;
    data["player"]["z"] = position.z;

    if(onHoldLA == true){
        data["player"]["onHoldLA"] = 1;
    }else{
        data["player"]["onHoldLA"] = 0;
    }

    if(onHoldRA == true){
        data["player"]["onHoldRA"] = 1;
    }else{
        data["player"]["onHoldRA"] = 0;
    }

    if(onHoldLL == true){
        data["player"]["onHoldLL"] = 1;
    }else{
        data["player"]["onHoldLL"] = 0;
    }

    if(onHoldRL == true){
        data["player"]["onHoldRL"] = 1;
    }else{
        data["player"]["onHoldRL"] = 0;
    }

    data["player"]["xLA"] = grabPointLA.x;
    data["player"]["yLA"] = grabPointLA.y;
    data["player"]["zLA"] = grabPointLA.z;

    data["player"]["xRA"] = grabPointRA.x;
    data["player"]["yRA"] = grabPointRA.y;
    data["player"]["zRA"] = grabPointRA.z;
    
    data["player"]["xLL"] = grabPointLL.x;
    data["player"]["yLL"] = grabPointLL.y;
    data["player"]["zLL"] = grabPointLL.z;
    
    data["player"]["xRL"] = grabPointRL.x;
    data["player"]["yRL"] = grabPointRL.y;
    data["player"]["zRL"] = grabPointRL.z;

    }catch(const json::parse_error& e){
        std::cerr << "parse error" << e.what() << '\n';
    }

    if(file.is_open()){
        file.close();
    }
}

void Player::applyCollision(float dt, Cube& wall){
    Vector3 correctedPos = prevPos;
    Vector3 testX = {nextPos.x, prevPos.y, prevPos.z};
    if (!CheckCollisionBoxes(MakeBox(testX, 0.5f), wall.getBoundingBox())){
        correctedPos.x = nextPos.x;
    }

    Vector3 testZ = {correctedPos.x, prevPos.y, nextPos.z };
    if (!CheckCollisionBoxes(MakeBox(testZ, 0.5f), wall.getBoundingBox())){
        correctedPos.z = nextPos.z;
    }

    Vector3 testY = {correctedPos.x, nextPos.y, correctedPos.z};
    if (!CheckCollisionBoxes(MakeBox(testY, 0.5f), wall.getBoundingBox())){
        correctedPos.y = nextPos.y;
    }
    position = correctedPos;


    bool onHold = onHoldRA || onHoldLA || onHoldRL || onHoldLL;

    if(IsKeyPressed(KEY_SPACE) && isGrounded == true && !onHold){
        velocity.y = 5.0f;
        isGrounded = false;
    }
    

    bool checkpo = position.x > wall.position.x - wall.width/2 - 0.49f &&
    position.x < wall.position.x + wall.width/2 + 0.49f &&
    position.z > wall.position.z - wall.length/2 - 0.49f &&
    position.z < wall.position.z + wall.length/2 + 0.49f; 

    if(position.y >= wall.height && checkpo == true && onHold == false && isGrounded == false){
        isGrounded = true;
        isTopped = true;
        if(IsKeyPressed(KEY_SPACE)){
            velocity.y = 5.0f;
            isGrounded = false;
        }else{
            if(isRappelling == false){
                velocity.y -= 9.8f * dt;
                position.y += velocity.y * dt;

                if(position.y <= 1.8f + wall.height){
                    position.y = 1.8f + wall.height;
                    velocity.y = 0;
                    isGrounded = true;
                }
            }
        }
    }else{
        isGrounded = false;
        isTopped = false;
    }

    if(isRappelling == false && !onHold && isGrounded == false){
        velocity.y -= 9.8f * dt;
        position.y += velocity.y * dt;
            
        if(position.y <= 1.8f){
            position.y = 1.8f;
            velocity.y = 0;
            isGrounded = true;
        }
    }

}

void Player::applyMovement(float dt){

    prevPos = position;
    Vector3 forwardFlat = Vector3Normalize({forward.x, 0, forward.z});
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forwardFlat, {0,1,0}));

    Vector3 moveDir = {0,0,0};
      
    nextPos = prevPos;
        
    Vector3 shoulders = {nextPos.x, nextPos.y - 0.3f, nextPos.z};
    Vector3 hips = {nextPos.x, nextPos.y - 0.9f, nextPos.z};
        
    if(onHoldRA == false && onHoldLA == false && onHoldRL == false && onHoldLL == false && isGrounded == true){
        if(IsKeyDown(KEY_W)) moveDir = Vector3Add(moveDir, forwardFlat);
        if(IsKeyDown(KEY_S)) moveDir = Vector3Subtract(moveDir, forwardFlat);
        if(IsKeyDown(KEY_D)) moveDir = Vector3Add(moveDir, right);
        if(IsKeyDown(KEY_A)) moveDir = Vector3Subtract(moveDir, right);

        moveDir = Vector3Normalize(moveDir);
        nextPos = Vector3Add(prevPos, Vector3Scale(moveDir, 5.0f * dt));
            
        position = nextPos;
    }
        
    if(onHoldLA || onHoldRA || onHoldLL || onHoldRL){
            
        if(IsKeyDown(KEY_W)) moveDir = Vector3Add(moveDir, {0,1,0});
        if(IsKeyDown(KEY_S)) moveDir = Vector3Subtract(moveDir, {0,1,0});
        if(IsKeyDown(KEY_D)) moveDir = Vector3Subtract(moveDir, {0,0,1});
        if(IsKeyDown(KEY_A)) moveDir = Vector3Add(moveDir, {0,0,1});
           

        bool checkLegs = onHoldRL || onHoldLL;

        if(Vector3Length(moveDir) > 0){
            moveDir = Vector3Normalize(moveDir);
            velocity = Vector3Add(velocity, Vector3Scale(moveDir, 5.0f * dt));
        }

        velocity.y -= 9.8f * dt;

        nextPos = Vector3Add(prevPos, Vector3Scale(velocity, dt));
            
        Vector3 tempShoulders = {nextPos.x, nextPos.y - 0.3f, nextPos.z};
            
        Vector3 tempHips = {nextPos.x, nextPos.y - 0.9f, nextPos.z};

        if(onHoldRA){
            Vector3 toShoulder = Vector3Subtract(tempShoulders, grabPointRA);
            float dista = Vector3Length(toShoulder);
            if(dista > limbLength){
                Vector3 dir = Vector3Normalize(toShoulder);
                tempShoulders = Vector3Add(grabPointRA, Vector3Scale(dir, limbLength));
                float radialVel = Vector3DotProduct(velocity, dir);
                Vector3 radialComponent = Vector3Scale(dir, radialVel);
                velocity = Vector3Subtract(velocity, radialComponent);
                velocity = Vector3Scale(velocity, 0.96f);
            }
        }
            
        if(onHoldLA){
            Vector3 toShoulder = Vector3Subtract(tempShoulders, grabPointLA);
            float dista = Vector3Length(toShoulder);
            if(dista > limbLength){
                Vector3 dir = Vector3Normalize(toShoulder);
                tempShoulders = Vector3Add(grabPointLA, Vector3Scale(dir, limbLength));
                float radialVel = Vector3DotProduct(velocity, dir);
                Vector3 radialComponent = Vector3Scale(dir, radialVel);
                velocity = Vector3Subtract(velocity, radialComponent);
                velocity = Vector3Scale(velocity, 0.96f);
            }
        }
            

        if(onHoldLA && onHoldRA){
            Vector3 mid = Vector3Lerp(grabPointRA, grabPointLA, 0.5f);
            Vector3 toMid = Vector3Subtract(mid, tempShoulders);
            float stiffness = 8.0f;

        }
            
        tempHips = {tempShoulders.x, tempShoulders.y - 0.6f, tempShoulders.z};

        if(onHoldRL){
            Vector3 toHip = Vector3Subtract(tempHips, grabPointRL);
            float dist = Vector3Length(toHip);

            if(dist > limbLength){
                Vector3 dir = Vector3Normalize(toHip);
                tempHips = Vector3Add(grabPointRL, Vector3Scale(dir, limbLength));
                float radialVel = Vector3DotProduct(velocity, dir);
                Vector3 radialComponent = Vector3Scale(dir, radialVel);
                velocity = Vector3Subtract(velocity, radialComponent);
                velocity = Vector3Scale(velocity, 0.9f);
                stretchedRight = true;
            }else{
                stretchedRight = false;
            }
        }

        if(onHoldLL){
            Vector3 toHip = Vector3Subtract(tempHips, grabPointLL);
            float dist = Vector3Length(toHip);

            if(dist >= limbLength){
                Vector3 dir = Vector3Normalize(toHip);
                tempHips = Vector3Add(grabPointLL, Vector3Scale(dir, limbLength));
                float radialVel = Vector3DotProduct(velocity, dir);
                Vector3 radialComponent = Vector3Scale(dir, radialVel);
                velocity = Vector3Subtract(velocity, radialComponent);
                velocity = Vector3Scale(velocity, 0.9f);
                stretchedLeft = true;
            }else{
                stretchedLeft = false;
            }
        }
            
        bool stretched = stretchedRight || stretchedLeft;

        if(checkLegs){
            Vector3 desShoulders = {tempHips.x, tempHips.y + 0.6f, tempHips.z};

            tempShoulders = Vector3Lerp(tempShoulders, desShoulders, 0.65f);
        }
       
        if((onHoldRL && onHoldLL) && IsKeyDown(KEY_W)){
            velocity.y += std::fabs(Vector3DotProduct(Vector3Subtract(tempShoulders, grabPointLL), Vector3Subtract(tempShoulders, grabPointRL))) * 50 * dt;
            if(staminaLL > 0 && staminaRL > 0){
                staminaLL -= 0.03f;
                staminaRL -= 0.03f;
            }
        }else if ((onHoldRL && !onHoldLL) && IsKeyDown(KEY_W)){
            int add = std::fabs(Vector3DotProduct(Vector3Subtract(tempShoulders, grabPointRL), tempShoulders));
            if(add == 0){
                velocity.y += 1.5 * dt;
            }else{
                velocity.y += add * 1.5f * dt;
            }
            if(staminaRL > 0){
                staminaRL -= 0.05f;
            }
        }else if ((!onHoldRL && onHoldLL) && IsKeyDown(KEY_W)){
            int add = std::fabs(Vector3DotProduct(Vector3Subtract(tempShoulders, grabPointLL), tempShoulders));
            if(add == 0){
                velocity.y += 1.5 * dt;
            }else{
                velocity.y += add * 1.5f * dt;
            }
            if(staminaLL > 0){
                staminaLL -= 0.05f;
            }
        }
        
        if(onHoldRL && !onHoldLL){
            staminaLL += 0.025f;
        }
        
        if(!onHoldRL && onHoldLL){
            staminaRL += 0.025f;
        }
        
        if(stretched && std::fabs(velocity.y) > 0){
            position = nextPos; 
        }
            
        nextPos.x = tempShoulders.x;
        nextPos.y = tempShoulders.y + 0.3f;
        nextPos.z = tempShoulders.z;

        position = nextPos;
        
    }
}

void Player::applyMovementRappelling(float dt){
    prevPos = position;
    Vector3 forwardFlat = Vector3Normalize({forward.x, 0, forward.z});
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forwardFlat, {0,1,0}));

    Vector3 moveDir = {0,0,0};
      
    nextPos = prevPos;
    
    if(IsKeyDown(KEY_SPACE)){
        moveDir = Vector3Subtract(moveDir, {0,1,0});   
        moveDir = Vector3Normalize(moveDir);
        nextPos = Vector3Add(prevPos, Vector3Scale(moveDir, 5.0f * dt));
        position = nextPos;
    }
    
    if(position.y <= 1.8f){
        position.y = 1.8f;
        isRappelling = false;
    }

}

void Player::update(float dt){
    
    Vector2 mouseDelta = GetMouseDelta();

    float sensitivity = 0.002f;

    yaw -= mouseDelta.x * sensitivity;
    pitch -= mouseDelta.y * sensitivity;

    if(pitch > 1.5){
        pitch = 1.5;
    }
    
    if(pitch < -1.5){
        pitch = -1.5;
    }
    
    forward = {cosf(pitch) * sinf(yaw), sinf(pitch), cosf(pitch) * cosf(yaw)};
    forward = Vector3Normalize(forward);
    camera.position = position;
    camera.target = Vector3Add(position, forward);
    shoulders = {position.x, position.y - 0.3f, position.z};
}

void Player::takeToTheTop(Cube& wall){

    position.x = wall.position.x;
    position.y = wall.height + 1.8f;
    position.z = wall.position.z;
    

}

void Player::rappDown(Cube& wall){
    
    float dt = GetFrameTime();
    position.x = wall.position.x + wall.width + 0.5f;
    isRappelling = true;
    
    isGrounded = false;
    

}
