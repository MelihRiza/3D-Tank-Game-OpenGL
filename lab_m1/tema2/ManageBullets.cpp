#include "lab_m1/tema2/tema2.h"

using namespace std;
using namespace m1;

#include <vector>
#include <string>
#include <iostream>


#define BULLET_SPEED_CONSTANT 20.0f
#define MAX_BULLET_RANGE 30.0f
#define TANK_RADIUS 1.2f

void tema2::ManageBullets(std::vector<building> *buildings, std::vector<bullet> *bullets, std::vector<tank> *tanks, float deltaTimeSeconds) {
    DrawBullets(bullets);

    if (!game_over) {
        ChangeBulletsPosition(bullets, deltaTimeSeconds);
        CheckBullet_TankColision(bullets, tanks);
        CheckBullet_BuildingColision(buildings, bullets);
    }
}

void tema2::ChangeBulletsPosition(std::vector<bullet>* bullets, float deltaTimeSeconds) {
    if (bullets == nullptr) {
        return;
    }

    std::vector<bullet>& bulletsVec = *bullets;

    for (auto& bullet : bulletsVec) {
        bullet.current_bullet_position += bullet.bullet_direction * BULLET_SPEED_CONSTANT * deltaTimeSeconds;
        bullet.traveled_distance += BULLET_SPEED_CONSTANT * deltaTimeSeconds;
    }

    auto removeCondition = [](const bullet& b) { return b.traveled_distance > MAX_BULLET_RANGE; };

    bulletsVec.erase(std::remove_if(bulletsVec.begin(), bulletsVec.end(), removeCondition), bulletsVec.end());
}

void tema2::DrawBullets(std::vector<bullet> *bullets) {
    if (bullets == nullptr) {
        return;
    }

    std::vector<bullet>& bulletsVec = *bullets;

    for (auto& bullet : bulletsVec) {
        
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, bullet.current_bullet_position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.1, 0.1));
        RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, &my_tank);

    }
	
}

void tema2::CheckBullet_TankColision(std::vector<bullet> *bullets, std::vector<tank> *tanks) {
    std::vector<tank>& tanksVec = *tanks;

    for (auto& tank : tanksVec) {
        if (tank.my_tank == false) {
            
            std::vector<bullet>& bulletsVec = *bullets;

            for (auto& bullet : bulletsVec) {
                float distance_to_tank = glm::distance(tank.current_position, bullet.current_bullet_position);

                if (distance_to_tank <= TANK_RADIUS) {
                    if (tank.health > 0) {
                        tank.health -= 1;
                        tank.deformationStrength += 0.1;
                        my_tank.score += 100;
                    }
                    bullet.used_bullet = true;
                }
            }

            bulletsVec.erase(std::remove_if(bulletsVec.begin(), bulletsVec.end(),
                [](const bullet& b) { return b.used_bullet; }), bulletsVec.end());
            
        }
        else if(tank.my_tank == true) {
            
            std::vector<bullet>& bulletsVec = *bullets;

            for (auto& bullet : bulletsVec) {
                
                if (bullet.my_bullet == false) {
                    
                    float distance_to_tank = glm::distance(my_tank.current_position, bullet.current_bullet_position);
                    if (distance_to_tank <= TANK_RADIUS) {
                        if (my_tank.health > 0) {
                            my_tank.health -= 1;
                        }
                        bullet.used_bullet = true;
                    }
                }
            }

            bulletsVec.erase(std::remove_if(bulletsVec.begin(), bulletsVec.end(),
                [](const bullet& b) { return b.used_bullet; }), bulletsVec.end());
        }
    }
}

void tema2::CheckBullet_BuildingColision(std::vector<building>* buildings, std::vector<bullet>* bullets) {
    std::vector<building>& buildingsVec = *buildings;
    std::vector<bullet>& bulletsVec = *bullets;

    for (auto& building : buildingsVec) {
        for (auto& bullet : bulletsVec) {

            if (building.corner1.x < 0 && building.corner1.z > 0 && bullet.current_bullet_position.x < 0 && bullet.current_bullet_position.z > 0) {
                if (abs(bullet.current_bullet_position.x) < abs(building.corner1.x) && abs(bullet.current_bullet_position.x) > abs(building.corner3.x)
                    && abs(bullet.current_bullet_position.z) > abs(building.corner1.z) && abs(bullet.current_bullet_position.z) < abs(building.corner3.z)) {

                    bullet.used_bullet = true;
                }
            }
            else if (building.corner1.x > 0 && building.corner1.z > 0 && bullet.current_bullet_position.x > 0 && bullet.current_bullet_position.z > 0) {
                if (abs(bullet.current_bullet_position.x) < abs(building.corner1.x) && abs(bullet.current_bullet_position.x) > abs(building.corner3.x)
                    && abs(bullet.current_bullet_position.z) > abs(building.corner1.z) && abs(bullet.current_bullet_position.z) < abs(building.corner3.z)) {

                    bullet.used_bullet = true;
                }
            }
            else if (building.corner1.x < 0 && building.corner1.z < 0 && bullet.current_bullet_position.x < 0 && bullet.current_bullet_position.z < 0) {
                if (abs(bullet.current_bullet_position.x) < abs(building.corner1.x) && abs(bullet.current_bullet_position.x) > abs(building.corner3.x)
                    && abs(bullet.current_bullet_position.z) < abs(building.corner1.z) && abs(bullet.current_bullet_position.z) > abs(building.corner3.z)) {

                    bullet.used_bullet = true;
                }
            }
            else if (building.corner1.x > 0 && building.corner1.z < 0 && bullet.current_bullet_position.x > 0 && bullet.current_bullet_position.z < 0) {
                if (abs(bullet.current_bullet_position.x) < abs(building.corner1.x) && abs(bullet.current_bullet_position.x) > abs(building.corner3.x)
                    && abs(bullet.current_bullet_position.z) < abs(building.corner1.z) && abs(bullet.current_bullet_position.z) > abs(building.corner3.z)) {

                    bullet.used_bullet = true;

                }
            }
        }

        bulletsVec.erase(std::remove_if(bulletsVec.begin(), bulletsVec.end(),
            [](const bullet& b) { return b.used_bullet; }), bulletsVec.end());
    }
}
