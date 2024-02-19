#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <random>

using namespace std;
using namespace m1;


#define GUN_AND_TRACKS_COLOR glm::vec3(0.5, 0.5, 0.5)

#define GUN_RELOAD_TIME 5
#define GUN_LENGTH 2.0f
#define GAME_TIME 180 

tank::tank() {

}

tank::tank(glm::vec3 current_position, float angle, bool my_tank) {
	this->current_position = current_position;
    this->my_tank = my_tank;

    if (my_tank) {
        this->health = 25; 
        this->hullColor = glm::vec3(0.1, 0.2, 0.17);
        this->turretColor = glm::vec3(0, 0.49, 0.38);
    }
    else {
        this->health = 4;
        this->hullColor = glm::vec3(0.47, 0.31, 0.66);
        this->turretColor = glm::vec3(0.8, 0.36, 0.36);

        float time_spent_moving = 0;

        while (time_spent_moving < GAME_TIME) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distributionMovementType(1, 4);
            std::uniform_int_distribution<> distribuitonTimeForMovement(0, 1);

            int movement_type = distributionMovementType(gen);
            float movement_time = distribuitonTimeForMovement(gen);

            if (movement_type == 1 || movement_type == 4) {
                movement_time += 4;
            }

            this->enemy_tank_movement.emplace_back(movement_type, movement_time);

            time_spent_moving += movement_time;
        }
        time_spent_in_current_movement = 0;
        current_movement = enemy_tank_movement.back();
        enemy_tank_movement.pop_back();
    }
    this->score = 0;

    this->current_gun_position = current_position + glm::vec3(0, 0.175, 0);
    this->left_track_position = current_position + glm::vec3(-0.38, -0.8, -2.95);
    this->right_track_position = current_position + glm::vec3(0.38, -0.8, -2.95);

    if (current_position.z <= 0) {
        this->tank_direction = glm::normalize(glm::vec3(0, 0, current_position.z));
        this->gun_direction = glm::normalize(glm::vec3(0, 0, current_position.z));
    }
    else {
        this->tank_direction = glm::normalize(glm::vec3(0, 0, -current_position.z));
        this->gun_direction = glm::normalize(glm::vec3(0, 0, -current_position.z));
    }

    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec4 transformed = rotationMatrix * glm::vec4(this->tank_direction, 1.0f);
    this->tank_direction = glm::normalize(glm::vec3(transformed));
    this->gun_direction = glm::normalize(glm::vec3(transformed));

    this->rotation = angle;
    this->turret_rotation = 0;

    this->gun_barrel_tip = this->current_gun_position + this->gun_direction * GUN_LENGTH;
    this->reloading_time_passed = 0;

    this->deformationStrength = 0;
}

tank::~tank() {

}


glm::vec3 tank::GetCurrentPosition() {
	return this->current_position;
}


glm::vec3 tank::GetCameraPosition() {
	glm::vec3 camera_position = current_position + glm::vec3(0, 1.5, 3);
	return camera_position;
}

glm::vec3 tank::GetGunBasePosition() {
    return current_position + glm::vec3(0, 0.175, 0);
}

glm::vec3 tank::GetLeftTrackPosition() {
    return glm::vec3(-0.38, -0.4, 0);
}

glm::vec3 tank::GetRightTrackPosition() {
    return glm::vec3(0.38, -0.4, 0);
}

void tank::RotateTank(float angle) {
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec4 transformed = rotationMatrix * glm::vec4(this->tank_direction, 1.0f);
    this->tank_direction = glm::normalize(glm::vec3(transformed));
}


void tank::RotateGun(float angle) {
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec4 transformed = rotationMatrix * glm::vec4(this->gun_direction, 1.0f);
    this->gun_direction = glm::normalize(glm::vec3(transformed));
    this->gun_barrel_tip = this->current_gun_position + this->gun_direction * GUN_LENGTH;
}


void tank::ShootBullet(std::vector<bullet>* bullets) {

    if (this->reloading_time_passed >= GUN_RELOAD_TIME) {
        if (this->my_tank) {
            bullet new_bullet = bullet(this->gun_barrel_tip, this->gun_direction, true);
            bullets->emplace_back(new_bullet);
            this->reloading_time_passed = 0;
        }
        else {
            bullet new_bullet = bullet(this->gun_barrel_tip, this->shooting_direction, false);
            bullets->emplace_back(new_bullet);
            this->reloading_time_passed = 0;
        }
    }
}


void tank::UpdateTankStates(float deltaTimeSeconds) {
    this->reloading_time_passed += deltaTimeSeconds;
}
 

vector<VertexFormat> tank::GetTankHullVertices() {


    glm::vec3 origin_position = glm::vec3(0, 0, 0);
    vector<VertexFormat> vertices
    {
        
        VertexFormat(origin_position, hullColor, glm::vec3(0.2, 0.8, 0.2)), 
        VertexFormat(origin_position + glm::vec3(-1, 0, -1.5), hullColor, glm::vec3(0.9, 0.4, 0.2)),  
        VertexFormat(origin_position + glm::vec3(1, 0, -1.5), hullColor, glm::vec3(0.7, 0.7, 0.1)),
        VertexFormat(origin_position + glm::vec3(-1, 0, 2.5), hullColor, glm::vec3(0.7, 0.3, 0.7)), 
        VertexFormat(origin_position + glm::vec3(1, 0, 2.5), hullColor, glm::vec3(0.3, 0.5, 0.4)), 

        // "sasiu frontal"
        VertexFormat(origin_position + glm::vec3(-1, -0.5, -2.5), hullColor, glm::vec3(0.9, 0.4, 0.2)),  
        VertexFormat(origin_position + glm::vec3(1, -0.5, -2.5), hullColor, glm::vec3(0.7, 0.7, 0.1)), 


        // "burta"
        VertexFormat(origin_position + glm::vec3(-1, -0.7, -2.3), hullColor, glm::vec3(0.9, 0.4, 0.2)),  
        VertexFormat(origin_position + glm::vec3(1, -0.7, -2.3), hullColor, glm::vec3(0.7, 0.7, 0.1)), 
        
        // spate
        VertexFormat(origin_position + glm::vec3(-1, -0.7, 2.5), hullColor, glm::vec3(0.9, 0.4, 0.2)), 
        VertexFormat(origin_position + glm::vec3(1, -0.7, 2.5), hullColor, glm::vec3(0.9, 0.4, 0.2)), 



    };

    return vertices;
}

vector<unsigned int> tank::GetTankHullIndices()
{
    vector<unsigned int> indices
    {
        0, 1, 2,
        0, 3, 1,  // parte superioara sasiu
        0, 2, 4,
        0, 4, 3,

        1, 5, 2,  // parte frontala sasiu
        5, 6, 2,

        5, 7, 6, // burta
        6, 7, 8,

        9, 3, 1,  // lateral 1
        9, 1, 7,
        1, 7, 5,

        10, 4, 2,  // lateral 2
        10, 2, 8,
        2, 8, 6,

        10, 4, 3, // spate
        9, 3, 10,

        10, 8, 9,
        7, 8, 9

    };

    return indices;
}


std::vector<VertexFormat> tank::GetTankTrackVertices() {

    glm::vec3 origin_position = glm::vec3(0, 0, 0);
    vector<VertexFormat> vertices
    {

        VertexFormat(origin_position, GUN_AND_TRACKS_COLOR, glm::vec3(0.2, 0.8, 0.2)),
        VertexFormat(origin_position + glm::vec3(-1, -1, -1), GUN_AND_TRACKS_COLOR, glm::vec3(0.2, 0.8, 0.2)),
        VertexFormat(origin_position + glm::vec3(1, -1, -1), GUN_AND_TRACKS_COLOR, glm::vec3(0.2, 0.8, 0.2)),
        VertexFormat(origin_position + glm::vec3(1, -1, 1), GUN_AND_TRACKS_COLOR, glm::vec3(0.2, 0.8, 0.2)),
        VertexFormat(origin_position + glm::vec3(-1, -1, 1), GUN_AND_TRACKS_COLOR, glm::vec3(0.2, 0.8, 0.2)),

        VertexFormat(origin_position + glm::vec3(-1, 1, -1), GUN_AND_TRACKS_COLOR, glm::vec3(0.2, 0.8, 0.2)),
        VertexFormat(origin_position + glm::vec3(1, 1, -1), GUN_AND_TRACKS_COLOR, glm::vec3(0.2, 0.8, 0.2)),
        VertexFormat(origin_position + glm::vec3(1, 1, 1), GUN_AND_TRACKS_COLOR, glm::vec3(0.2, 0.8, 0.2)),
        VertexFormat(origin_position + glm::vec3(-1, 1, 1), GUN_AND_TRACKS_COLOR, glm::vec3(0.2, 0.8, 0.2)),

    };

    return vertices;
}


std::vector<unsigned int> tank::GetTankTrackIndices() {
    
    vector<unsigned int> indices
    {
        1, 2, 3,
        3, 4, 1,
        1, 2, 5,
        5, 6, 2,
        3, 4, 7,
        4, 7, 8,
        1, 4, 8,
        1, 8, 5,
        6, 2, 3,
        3, 6, 7,
        5, 6, 7,
        5, 7, 8
    };

    return indices;
}


vector<VertexFormat> tank::GetTankTurretVertices() {


    glm::vec3 relativePos = glm::vec3(0, 0, 0);
    vector<VertexFormat> vertices
    {
        // baza turela
        VertexFormat(relativePos, turretColor, glm::vec3(0.2, 0.8, 0.2)), 
        VertexFormat(relativePos + glm::vec3(-0.90, 0, -1), turretColor, glm::vec3(0.9, 0.4, 0.2)), 
        VertexFormat(relativePos + glm::vec3(0.90, 0, -1), turretColor, glm::vec3(0.7, 0.7, 0.1)), 
        VertexFormat(relativePos + glm::vec3(-0.90, 0, 1.5), turretColor, glm::vec3(0.7, 0.3, 0.7)), 
        VertexFormat(relativePos + glm::vec3(0.90, 0, 1.5), turretColor, glm::vec3(0.3, 0.5, 0.4)), 


        // top turela
        VertexFormat(relativePos + glm::vec3(-0.90, 0.7, -1), turretColor, glm::vec3(0.9, 0.4, 0.2)), 
        VertexFormat(relativePos + glm::vec3(0.90, 0.7, -1), turretColor, glm::vec3(0.7, 0.7, 0.1)),  
        VertexFormat(relativePos + glm::vec3(-0.90, 0.7, 1.2), turretColor, glm::vec3(0.7, 0.3, 0.7)),  
        VertexFormat(relativePos + glm::vec3(0.90, 0.7, 1.2), turretColor, glm::vec3(0.3, 0.5, 0.4)),  

        // unghiuri turela frontla
        VertexFormat(relativePos + glm::vec3(-0.9, 0.35, -1.4), turretColor, glm::vec3(0.9, 0.4, 0.2)),
        VertexFormat(relativePos + glm::vec3(-0.3, 0.35, -1.4), turretColor, glm::vec3(0.9, 0.4, 0.2)),
        VertexFormat(relativePos + glm::vec3(-0.3, 0, -1), turretColor, glm::vec3(0.9, 0.4, 0.2)), 
        VertexFormat(relativePos + glm::vec3(-0.3, 0.7, -1), turretColor, glm::vec3(0.9, 0.4, 0.2)),

        VertexFormat(relativePos + glm::vec3(0.9, 0.35, -1.4), turretColor, glm::vec3(0.9, 0.4, 0.2)), 
        VertexFormat(relativePos + glm::vec3(0.3, 0.35, -1.4), turretColor, glm::vec3(0.9, 0.4, 0.2)),  
        VertexFormat(relativePos + glm::vec3(0.3, 0, -1), turretColor, glm::vec3(0.9, 0.4, 0.2)), 
        VertexFormat(relativePos + glm::vec3(0.3, 0.7, -1), turretColor, glm::vec3(0.9, 0.4, 0.2))
        
    };

    return vertices;
}


vector<unsigned int> tank::GetTankTurretIndices()
{
    vector<unsigned int> indices
    {
        0, 1, 2, // baza turela
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,

        5, 6, 7, // top turela
        8, 6, 7,

        1, 3, 5,  // sides turela
        5, 3, 7,
        2, 4, 6,
        6, 4, 8,

        4, 8, 7,  // back turela
        7, 3, 4,

        9, 5, 12,
        12, 9, 10,

        10, 11, 1,
        9, 10, 1,

        1, 9, 5,

        10, 11, 12,

        13, 6, 16,
        14, 16, 13,

        15, 13, 14,
        13, 15, 2,

        2, 13, 6,

        14, 15, 16,

        15, 16, 11,
        11, 12, 16
    };

    return indices;
}


vector<VertexFormat> tank::GetTankGunVertices() {
    vector<VertexFormat> vertices;

    float radius = 0.05;
    float height = 4.0;
    int sides = 200;

    glm::vec3 relativePos = glm::vec3(0, 0, 0);

    float theta = 0;
    float x = radius * std::cos(theta);
    float z = radius * std::sin(theta);


    for (int i = 0; i < sides; i++) {
        float theta = ((float)i / sides) * glm::two_pi<float>();
        float x = radius * std::cos(theta);
        float z = radius * std::sin(theta);

        vertices.emplace_back(VertexFormat(relativePos + glm::vec3(x, height * 0.5, z), GUN_AND_TRACKS_COLOR, glm::vec3(0.9, 0.4, 0.2)));
        vertices.emplace_back(VertexFormat(relativePos + glm::vec3(x, 0 * 0.5, z), GUN_AND_TRACKS_COLOR, glm::vec3(0.9, 0.4, 0.2)));


    }
    
    return vertices;
}

vector<unsigned int> tank::GetTankGunIndices() {
    vector<unsigned int> indices;

    int sides = 200;

    for (int i = 0; i < sides - 1; i++) {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 2);

        indices.push_back(2 * i);
        indices.push_back(2 * i + 1);
        indices.push_back(2 * i + 2);

        indices.push_back(2 * i + 1);
        indices.push_back(2 * i + 3);
        indices.push_back(2 * i + 2);

    }

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2 * (sides - 1) + 1);
    
    indices.push_back(0);
    indices.push_back(2 * (sides - 1));
    indices.push_back(2 * (sides - 1) + 1);


    return indices;
}