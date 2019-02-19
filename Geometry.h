/*
 * Geometry.h
 *	Class for storing geometry
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "texture.h"

class Geometry {
public:
	Geometry(const char* image_name, float k_diffuse, float k_specular);
	virtual ~Geometry();

	//Data structures for storing vertices, normals colors and uvs
	std::vector<glm::vec4> verts;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec2> uvs;

	glm::mat4 model;
	glm::mat4 reference;
	glm::mat4 orbit;
	glm::mat4 tilt;
	glm::mat4 axis;
	glm::mat4 planet_reference;

	MyTexture texture;
	//Pointers to the vao and vbos associated with the geometry
	GLuint vao;
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint uvBuffer;
	GLuint colorBuffer;

	GLuint uniformlocation;

	float kd;
	float ks;

	//Draw mode for how OpenGL interprets primitives
	GLuint drawMode;
};

#endif /* GEOMETRY_H_ */
