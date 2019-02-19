/*
 * Camera.cpp
 *
 *  Created on: Dec. 3, 2018
 *      Author: marc_
 */

#include "Camera.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

Camera::Camera(vec3 e) {
	eye = e;

}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}

