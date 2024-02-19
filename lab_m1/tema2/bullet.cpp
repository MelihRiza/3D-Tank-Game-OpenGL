#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


bullet::bullet() {

}

bullet::bullet(glm::vec3 current_bullet_position, glm::vec3 bullet_direction, bool my_bullet) {
	this->current_bullet_position = current_bullet_position;
	this->bullet_direction = bullet_direction;
	this->traveled_distance = 0;
	this->my_bullet = my_bullet;
	this->used_bullet = false;
}


bullet::~bullet() {

}