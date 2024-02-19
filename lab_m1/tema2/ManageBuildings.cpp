#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <random>

using namespace std;
using namespace m1;

#define TANK_CENTER_TO_HULL_DISTANCE 0.5f

building::building() {

}

building::building(glm::vec3 building_center, float ox_size, float oy_size, float oz_size) {
	this->building_center = building_center;
	this->ox_side = ox_size;
	this->oy_side = oy_size;
	this->oz_side = oz_size;

	if (building_center.x < 0) {
		this->corner1 = building_center + glm::vec3(-ox_size / 2 - TANK_CENTER_TO_HULL_DISTANCE, 0, -oz_size / 2 - TANK_CENTER_TO_HULL_DISTANCE);
		this->corner2 = building_center + glm::vec3(ox_size / 2 + TANK_CENTER_TO_HULL_DISTANCE, 0, -oz_size / 2 - TANK_CENTER_TO_HULL_DISTANCE);
		this->corner3 = building_center + glm::vec3(ox_size / 2 + TANK_CENTER_TO_HULL_DISTANCE, 0, oz_size / 2 + TANK_CENTER_TO_HULL_DISTANCE);
		this->corner4 = building_center + glm::vec3(-ox_size / 2 - TANK_CENTER_TO_HULL_DISTANCE, 0, oz_size / 2 + TANK_CENTER_TO_HULL_DISTANCE);
	}
	
	else if (building_center.x > 0) {
		this->corner1 = building_center + glm::vec3(ox_size / 2 + TANK_CENTER_TO_HULL_DISTANCE, 0, -oz_size / 2 - TANK_CENTER_TO_HULL_DISTANCE);
		this->corner2 = building_center + glm::vec3(-ox_size / 2 - TANK_CENTER_TO_HULL_DISTANCE, 0, -oz_size / 2 - TANK_CENTER_TO_HULL_DISTANCE);
		this->corner3 = building_center + glm::vec3(-ox_size / 2 - TANK_CENTER_TO_HULL_DISTANCE, 0, oz_size / 2 + TANK_CENTER_TO_HULL_DISTANCE);
		this->corner4 = building_center + glm::vec3(ox_size / 2 + TANK_CENTER_TO_HULL_DISTANCE, 0, oz_size / 2 + TANK_CENTER_TO_HULL_DISTANCE);
	}

}

building::~building() {

}

void tema2::ManageBuildings(std::vector<building>* buildings) {
	DrawBuildings(buildings);
	CheckTank_BuildingColision(buildings, &tanks);
}

void tema2::GenerateRandomPositionedBuildings() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distributionZ(20, 50);
	std::uniform_int_distribution<> distributionX(10, 50);

	int x_pos = distributionX(gen);
	int z_pos = distributionZ(gen);
	building1 = building(glm::vec3(-x_pos, 0, z_pos), 10, 25, 10);

	x_pos = distributionX(gen);
	z_pos = distributionZ(gen);
	building2 = building(glm::vec3(-x_pos, 0, -z_pos), 10, 40, 10);

	x_pos = distributionX(gen);
	z_pos = distributionZ(gen);
	building3 = building(glm::vec3(-x_pos, 0, -z_pos), 10, 30, 10);

	x_pos = distributionX(gen);
	z_pos = distributionZ(gen);
	building4 = building(glm::vec3(x_pos, 0, -z_pos), 20, 30, 10);

	x_pos = distributionX(gen);
	z_pos = distributionZ(gen);
	building5 = building(glm::vec3(x_pos, 0, -z_pos), 5, 40, 5);

	x_pos = distributionX(gen);
	z_pos = distributionZ(gen);
	building6 = building(glm::vec3(x_pos, 0, z_pos), 10, 50, 10);

	buildings.emplace_back(building1);
	buildings.emplace_back(building2);
	buildings.emplace_back(building3);
	buildings.emplace_back(building4);
	buildings.emplace_back(building5);
	buildings.emplace_back(building6);
}

void tema2::DrawBuildings(std::vector<building>* buildings) {

	std::vector<building>& buildingsVec = *buildings;


	for (auto& building : buildingsVec) {

		float scale_factor_x = building.ox_side / 2.0f;
		float scale_factor_y = building.oy_side / 2.0f;
		float scale_factor_z = building.oz_side / 2.0f;

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, building.building_center);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale_factor_x, scale_factor_y, scale_factor_z));
		RenderSimpleMesh(meshes["cube"], shaders["LabShader"], modelMatrix, &my_tank);

	}

}

void tema2::CheckTank_BuildingColision(std::vector<building>* buildings, std::vector<tank>* tanks) {
	std::vector<building>& buildingsVec = *buildings;
	std::vector<tank>& tanksVec = *tanks;

	for (auto& building : buildingsVec) {
		for (auto& tank : tanksVec) {

			if (building.corner1.x < 0 && building.corner1.z > 0 && tank.current_position.x < 0 && tank.current_position.z > 0) {
				if (abs(tank.current_position.x) < abs(building.corner1.x) && abs(tank.current_position.x) > abs(building.corner3.x)
					&& abs(tank.current_position.z) > abs(building.corner1.z) && abs(tank.current_position.z) < abs(building.corner3.z)) {

					if (tank.my_tank == false) {
						tank.current_position += tank.tank_direction * (-4.5f);
						tank.current_gun_position += tank.tank_direction * (-4.5f);
						tank.gun_barrel_tip = tank.current_gun_position + tank.shooting_direction * 2.0f;
					}
					else {
						my_tank.current_position += my_tank.tank_direction * (-4.5f);
						my_tank.current_gun_position += my_tank.tank_direction * (-4.5f);
						my_tank.gun_barrel_tip = my_tank.current_gun_position + my_tank.gun_direction * 2.0f;
						camera->MoveForward((-4.5f), my_tank.tank_direction);
					}
				}
				
			}
			else if (building.corner1.x > 0 && building.corner1.z > 0 && tank.current_position.x > 0 && tank.current_position.z > 0) {
				if (abs(tank.current_position.x) < abs(building.corner1.x) && abs(tank.current_position.x) > abs(building.corner3.x)
					&& abs(tank.current_position.z) > abs(building.corner1.z) && abs(tank.current_position.z) < abs(building.corner3.z)) {

					if (tank.my_tank == false) {
						tank.current_position += tank.tank_direction * (-4.5f);
						tank.current_gun_position += tank.tank_direction * (-4.5f);
						tank.gun_barrel_tip = tank.current_gun_position + tank.shooting_direction * 2.0f;
					}
					else {
						my_tank.current_position += my_tank.tank_direction * (-4.5f);
						my_tank.current_gun_position += my_tank.tank_direction * (-4.5f);
						my_tank.gun_barrel_tip = my_tank.current_gun_position + my_tank.gun_direction * 2.0f;
						camera->MoveForward((-4.5f), my_tank.tank_direction);
					}
				}
				
			}
			else if (building.corner1.x < 0 && building.corner1.z < 0 && tank.current_position.x < 0 && tank.current_position.z < 0) {
				if (abs(tank.current_position.x) < abs(building.corner1.x) && abs(tank.current_position.x) > abs(building.corner3.x)
					&& abs(tank.current_position.z) < abs(building.corner1.z) && abs(tank.current_position.z) > abs(building.corner3.z)) {

					if (tank.my_tank == false) {
						tank.current_position += tank.tank_direction * (-4.5f);
						tank.current_gun_position += tank.tank_direction * (-4.5f);
						tank.gun_barrel_tip = tank.current_gun_position + tank.shooting_direction * 2.0f;
					}
					else {
						my_tank.current_position += my_tank.tank_direction * (-4.5f);
						my_tank.current_gun_position += my_tank.tank_direction * (-4.5f);
						my_tank.gun_barrel_tip = my_tank.current_gun_position + my_tank.gun_direction * 2.0f;
						camera->MoveForward((-4.5f), my_tank.tank_direction);
					}
				}
			}
			else if (building.corner1.x > 0 && building.corner1.z < 0 && tank.current_position.x > 0 && tank.current_position.z < 0) {
				if (abs(tank.current_position.x) < abs(building.corner1.x) && abs(tank.current_position.x) > abs(building.corner3.x)
					&& abs(tank.current_position.z) < abs(building.corner1.z) && abs(tank.current_position.z) > abs(building.corner3.z)) {

					if (tank.my_tank == false) {
						tank.current_position += tank.tank_direction * (-4.5f);
						tank.current_gun_position += tank.tank_direction * (-4.5f);
						tank.gun_barrel_tip = tank.current_gun_position + tank.shooting_direction * 2.0f;
					}
					else {
						my_tank.current_position += my_tank.tank_direction * (-4.5f);
						my_tank.current_gun_position += my_tank.tank_direction * (-4.5f);
						my_tank.gun_barrel_tip = my_tank.current_gun_position + my_tank.gun_direction * 2.0f;
						camera->MoveForward((-4.5f), my_tank.tank_direction);
					}

				}
			}
		}

		if (building.corner1.x < 0 && building.corner1.z > 0 && my_tank.current_position.x < 0 && my_tank.current_position.z > 0) {
			if (abs(my_tank.current_position.x) < abs(building.corner1.x) && abs(my_tank.current_position.x) > abs(building.corner3.x)
				&& abs(my_tank.current_position.z) > abs(building.corner1.z) && abs(my_tank.current_position.z) < abs(building.corner3.z)) {

				my_tank.current_position += my_tank.tank_direction * (-4.5f);
				my_tank.current_gun_position += my_tank.tank_direction * (-4.5f);
				my_tank.gun_barrel_tip = my_tank.current_gun_position + my_tank.gun_direction * 2.0f;
				camera->MoveForward((-4.5f), my_tank.tank_direction);
			}

		}
		else if (building.corner1.x > 0 && building.corner1.z > 0 && my_tank.current_position.x > 0 && my_tank.current_position.z > 0) {
			if (abs(my_tank.current_position.x) < abs(building.corner1.x) && abs(my_tank.current_position.x) > abs(building.corner3.x)
				&& abs(my_tank.current_position.z) > abs(building.corner1.z) && abs(my_tank.current_position.z) < abs(building.corner3.z)) {
	
				my_tank.current_position += my_tank.tank_direction * (-4.5f);
				my_tank.current_gun_position += my_tank.tank_direction * (-4.5f);
				my_tank.gun_barrel_tip = my_tank.current_gun_position + my_tank.gun_direction * 2.0f;
				camera->MoveForward((-4.5f), my_tank.tank_direction);
			}

		}
		else if (building.corner1.x < 0 && building.corner1.z < 0 && my_tank.current_position.x < 0 && my_tank.current_position.z < 0) {
			if (abs(my_tank.current_position.x) < abs(building.corner1.x) && abs(my_tank.current_position.x) > abs(building.corner3.x)
				&& abs(my_tank.current_position.z) < abs(building.corner1.z) && abs(my_tank.current_position.z) > abs(building.corner3.z)) {
		
				my_tank.current_position += my_tank.tank_direction * (-4.5f);
				my_tank.current_gun_position += my_tank.tank_direction * (-4.5f);
				my_tank.gun_barrel_tip = my_tank.current_gun_position + my_tank.gun_direction * 2.0f;
				camera->MoveForward((-4.5f), my_tank.tank_direction);
			}
		}
		else if (building.corner1.x > 0 && building.corner1.z < 0 && my_tank.current_position.x > 0 && my_tank.current_position.z < 0) {
			if (abs(my_tank.current_position.x) < abs(building.corner1.x) && abs(my_tank.current_position.x) > abs(building.corner3.x)
				&& abs(my_tank.current_position.z) < abs(building.corner1.z) && abs(my_tank.current_position.z) > abs(building.corner3.z)) {
	
				my_tank.current_position += my_tank.tank_direction * (-4.5f);
				my_tank.current_gun_position += my_tank.tank_direction * (-4.5f);
				my_tank.gun_barrel_tip = my_tank.current_gun_position + my_tank.gun_direction * 2.0f;
				camera->MoveForward((-4.5f), my_tank.tank_direction);

			}
		}
	}

}

