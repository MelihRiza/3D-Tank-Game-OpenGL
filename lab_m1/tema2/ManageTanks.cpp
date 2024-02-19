#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <random>

using namespace std;
using namespace m1;


#define DISTANCE_TO_ENGAGE 20
#define FORWARD_ENEMY_TANK_SPEED 2
#define BACKWARDS_ENEMY_TANK_SPEED 1
#define DIRECTION_CHANGE_ENEMY_TANK_SPEED 1.5
#define TANK_RADIUS 1.5f

void tema2::ManageTanks(std::vector<tank>* tanks, float deltaTimeSeconds) {
    DrawTanks(tanks, deltaTimeSeconds);

    if (!game_over) {
        CheckToEngage(tanks, deltaTimeSeconds);
        IncrementEnemyReloadTime(tanks, deltaTimeSeconds);
        MoveEnemyTanks(tanks, deltaTimeSeconds);
        CheckTank_TankColision(tanks);
    }
}

void tema2::IncrementEnemyReloadTime(std::vector<tank>* tanks, float deltaTimeSeconds) {
    std::vector<tank>& tanksVec = *tanks;

    for (auto& tank : tanksVec) {
        if (tank.my_tank == false && tank.health > 0) {
            tank.reloading_time_passed += deltaTimeSeconds;
        }
    }
}

void tema2::GenerateRandomPositionedEnemyTanks() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distributionZ(15, 40);
    std::uniform_int_distribution<> distributionX(0, 30);
    std::uniform_int_distribution<> distributionRotation(0, 90);

    int x_pos = distributionX(gen);
    int z_pos = distributionZ(gen);
    int rotation = distributionRotation(gen);
    enemy_tank1 = tank(glm::vec3(x_pos, 0.45, z_pos), rotation, false);

    x_pos = distributionX(gen);
    z_pos = distributionZ(gen);
    rotation = distributionRotation(gen);
    enemy_tank2 = tank(glm::vec3(-x_pos, 0.45, z_pos), -rotation, false);

    x_pos = distributionX(gen);
    z_pos = distributionZ(gen);
    rotation = distributionRotation(gen);
    enemy_tank3 = tank(glm::vec3(x_pos, 0.45, -z_pos), rotation, false);

    x_pos = distributionX(gen);
    z_pos = distributionZ(gen);
    rotation = distributionRotation(gen);
    enemy_tank4 = tank(glm::vec3(-x_pos, 0.45, -z_pos), -rotation, false);

    x_pos = distributionX(gen);
    z_pos = distributionZ(gen);
    rotation = distributionRotation(gen);
    enemy_tank5 = tank(glm::vec3(x_pos, 0.45, -z_pos), rotation, false);

    x_pos = distributionX(gen);
    z_pos = distributionZ(gen);
    rotation = distributionRotation(gen);
    enemy_tank6 = tank(glm::vec3(-x_pos, 0.45, -z_pos), -rotation, false);

    x_pos = distributionX(gen);
    z_pos = distributionZ(gen);
    rotation = distributionRotation(gen);
    enemy_tank7 = tank(glm::vec3(-x_pos, 0.45, -z_pos), -rotation, false);

    x_pos = distributionX(gen);
    z_pos = distributionZ(gen);
    rotation = distributionRotation(gen);
    enemy_tank8 = tank(glm::vec3(-x_pos, 0.45, z_pos), -rotation, false);

    tanks.emplace_back(my_tank);
    tanks.emplace_back(enemy_tank1);
    tanks.emplace_back(enemy_tank2);
    tanks.emplace_back(enemy_tank3);
    tanks.emplace_back(enemy_tank4);
    tanks.emplace_back(enemy_tank5);
    tanks.emplace_back(enemy_tank6);
    tanks.emplace_back(enemy_tank7);
    tanks.emplace_back(enemy_tank8);

}


void tema2::DrawTanks(std::vector<tank>* tanks, float deltaTimeSeconds) {
    if (tanks == nullptr) {
        return;
    }

    std::vector<tank>& tanksVec = *tanks;

    for (auto& tank : tanksVec) {

        if (tank.my_tank == false) {

            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, tank.GetCurrentPosition());
                modelMatrix = glm::rotate(modelMatrix, tank.rotation, glm::vec3(0, 1, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0.5));
                RenderSimpleMesh(meshes["enemyTankHull"], shaders["LabShader"], modelMatrix, &tank);

            }

            {
                glm::mat4 modelMatrix = glm::mat4(1);

                modelMatrix = glm::translate(modelMatrix, tank.GetCurrentPosition());
                modelMatrix = glm::rotate(modelMatrix, tank.rotation, glm::vec3(0, 1, 0));
                modelMatrix = glm::translate(modelMatrix, tank.GetLeftTrackPosition());

                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15, 0.08, 1.18));
                RenderSimpleMesh(meshes["enemyTankTrack"], shaders["LabShader"], modelMatrix, &tank);

                modelMatrix = glm::mat4(1);

                modelMatrix = glm::translate(modelMatrix, tank.GetCurrentPosition());
                modelMatrix = glm::rotate(modelMatrix, tank.rotation, glm::vec3(0, 1, 0));
                modelMatrix = glm::translate(modelMatrix, tank.GetRightTrackPosition());

                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15, 0.08, 1.18));
                RenderSimpleMesh(meshes["enemyTankTrack"], shaders["LabShader"], modelMatrix, &tank);
            }

            {
                glm::mat4 modelMatrix = glm::mat4(1);

                modelMatrix = glm::translate(modelMatrix, tank.GetCurrentPosition());
                modelMatrix = glm::rotate(modelMatrix, tank.turret_rotation, glm::vec3(0, 1, 0));
                modelMatrix = glm::rotate(modelMatrix, tank.rotation, glm::vec3(0, 1, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0.5));

                RenderSimpleMesh(meshes["enemyTankTurret"], shaders["LabShader"], modelMatrix, &tank);
            }

            {
                float stabilisation = 4.71238;
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, tank.GetGunBasePosition());
                modelMatrix = glm::rotate(modelMatrix, tank.turret_rotation, glm::vec3(0, 1, 0));
                modelMatrix = glm::rotate(modelMatrix, tank.rotation, glm::vec3(0, 1, 0));
                modelMatrix = glm::rotate(modelMatrix, stabilisation, glm::vec3(1, 0, 0));
                RenderSimpleMesh(meshes["enemyTankGun"], shaders["LabShader"], modelMatrix, &tank);

            }

        }
        

    }
}

float signedAngleBetweenVectors(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& axis) {
    float dot = glm::dot(v1, v2);
    float angle = glm::acos(dot / (glm::length(v1) * glm::length(v2)));
    float sign = glm::dot(axis, glm::cross(v1, v2)) < 0 ? -1.0f : 1.0f;

    return angle * sign;
}


void tema2::CheckToEngage(std::vector<tank>* tanks, float deltaTimeSeconds) {
    std::vector<tank>& tanksVec = *tanks;

    for (auto& tank : tanksVec) {
        if (tank.my_tank == false && tank.health > 0) {
            float distance_to_my_tank = glm::distance(tank.current_position, my_tank.current_position);
            if (distance_to_my_tank <= DISTANCE_TO_ENGAGE) {
                glm::vec3 direction_to_target = glm::normalize(glm::vec3(my_tank.current_position - tank.current_position));

                
                float rotation_angle = signedAngleBetweenVectors(tank.gun_direction, direction_to_target, glm::vec3(0, 1, 0));
                
                    
                tank.shooting_direction = direction_to_target;
                tank.turret_rotation = rotation_angle;
                tank.gun_barrel_tip = tank.current_gun_position + tank.shooting_direction * 2.0f;
                 
                tank.ShootBullet(&bullets);
                
            }
        }
    }
}



void tema2::MoveEnemyTanks(std::vector<tank>* tanks, float deltaTimeSeconds) {
    std::vector<tank>& tanksVec = *tanks;

    for (auto& tank : tanksVec) {
        if (tank.my_tank == false && tank.health > 0) {

            if (!tank.enemy_tank_movement.empty() && tank.time_spent_in_current_movement >= tank.current_movement.second) {
                tank.current_movement = tank.enemy_tank_movement.back();
                tank.enemy_tank_movement.pop_back();
                tank.time_spent_in_current_movement = 0;
            }

            if (tank.current_movement.first == 1) {
                float distance = deltaTimeSeconds * FORWARD_ENEMY_TANK_SPEED;
                tank.current_position += tank.tank_direction * distance;
                tank.current_gun_position += tank.tank_direction * distance;
                tank.gun_barrel_tip = tank.current_gun_position + tank.shooting_direction * 2.0f;

                tank.time_spent_in_current_movement += deltaTimeSeconds;
            }
            else if (tank.current_movement.first == 4) {
                float distance = deltaTimeSeconds * BACKWARDS_ENEMY_TANK_SPEED;
                tank.current_position -= tank.tank_direction * distance;
                tank.current_gun_position -= tank.tank_direction * distance;
                tank.gun_barrel_tip = tank.current_gun_position + tank.shooting_direction * 2.0f;

                tank.time_spent_in_current_movement += deltaTimeSeconds;
            } 
            else if (tank.current_movement.first == 2) {
                tank.rotation += deltaTimeSeconds * DIRECTION_CHANGE_ENEMY_TANK_SPEED;
                tank.RotateTank(deltaTimeSeconds * DIRECTION_CHANGE_ENEMY_TANK_SPEED);
                tank.RotateGun(deltaTimeSeconds * DIRECTION_CHANGE_ENEMY_TANK_SPEED);

                tank.time_spent_in_current_movement += deltaTimeSeconds;
            }
            else if (tank.current_movement.first == 3) {
                tank.rotation -= deltaTimeSeconds * DIRECTION_CHANGE_ENEMY_TANK_SPEED;
                tank.RotateTank(-deltaTimeSeconds * DIRECTION_CHANGE_ENEMY_TANK_SPEED);
                tank.RotateGun(-deltaTimeSeconds * DIRECTION_CHANGE_ENEMY_TANK_SPEED);

                tank.time_spent_in_current_movement += deltaTimeSeconds;
            }

        }
    }
}


void tema2::CheckTank_TankColision(std::vector<tank>* tanks) {
    
    for (int i = 0; i < tanks->size(); i++) {
        for (int j = 0; j < tanks->size(); j++) {
            if (i != j && (*tanks)[j].my_tank == false && (*tanks)[i].my_tank == false) {
                float distace_between_tanks = glm::distance((*tanks)[j].current_position, (*tanks)[i].current_position);

                if (distace_between_tanks < 2 * TANK_RADIUS) {
                    glm::vec3 direction = (*tanks)[j].current_position - (*tanks)[i].current_position;
                    glm::vec3 final_direction = abs(TANK_RADIUS + TANK_RADIUS - distace_between_tanks) * glm::normalize(direction);

                    (*tanks)[j].current_position += final_direction * (-1.5f);
                    (*tanks)[j].current_gun_position += final_direction * (-1.5f);
                    (*tanks)[j].gun_barrel_tip = (*tanks)[j].current_gun_position + (*tanks)[j].shooting_direction * 2.0f;

                    (*tanks)[i].current_position += final_direction * (1.5f);
                    (*tanks)[i].current_gun_position += final_direction * (1.5f);
                    (*tanks)[i].gun_barrel_tip = (*tanks)[i].current_gun_position + (*tanks)[i].shooting_direction * 2.0f;

                }
            }
            else if (i != j && (*tanks)[j].my_tank == true) {
                float distace_between_tanks = glm::distance(my_tank.current_position, (*tanks)[i].current_position);

                if (distace_between_tanks < 2 * TANK_RADIUS) {
                    glm::vec3 direction = my_tank.current_position - (*tanks)[i].current_position;
                    glm::vec3 final_direction = abs(TANK_RADIUS + TANK_RADIUS - distace_between_tanks) * glm::normalize(direction);

                    my_tank.current_position += final_direction * (-1.5f);
                    my_tank.current_gun_position += final_direction * (-1.5f);
                    my_tank.gun_barrel_tip = (*tanks)[j].current_gun_position + my_tank.gun_direction * 2.0f;
                    camera->MoveForward((-1.5f), final_direction);

                    (*tanks)[i].current_position += final_direction * (1.5f);
                    (*tanks)[i].current_gun_position += final_direction * (1.5f);
                    (*tanks)[i].gun_barrel_tip = (*tanks)[i].current_gun_position + (*tanks)[i].shooting_direction * 2.0f;

                }
            }

        }
    }
}