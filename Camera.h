/*
 * Camera.h
 *
 *  Created on: Dec. 3, 2018
 *      Author: marc_
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class Camera {
public:
	Camera(vec3 e);
	virtual ~Camera();

	vec3 eye;

};

#endif /* CAMERA_H_ */
