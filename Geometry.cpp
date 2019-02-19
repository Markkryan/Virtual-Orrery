/*
 * Geometry.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#include "Geometry.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Geometry::Geometry(const char* image_name, float k_diffuse, float k_specular) : vao(0), vertexBuffer(0), normalBuffer(0), uvBuffer(0), colorBuffer(0) {
	//vectors are initially empty
	//Pointers are initially null
	//Call RenderingEngine::assignBuffers and RenderingEngine::setBufferData to fully initialize the geometry
	//Overload the constructor for more functionality or create subclasses for specific objects

	model = glm::mat4(1.0);
	reference = glm::mat4(1.0);
	orbit = glm::mat4(1.0);
	tilt = glm::mat4(1.0);
	axis = glm::mat4(1.0);
	planet_reference = glm::mat4(1.0);
	kd = k_diffuse;
	ks = k_specular;
	InitializeTexture(&texture, image_name, GL_TEXTURE_2D);
}

Geometry::~Geometry() {

}

