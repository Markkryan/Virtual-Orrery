/*
 * Scene.cpp
 *
 *  Created on: Sep 10, 2018
 *  Author: John Hall
 */

#include "Scene.h"

#include <iostream>

#include "RenderingEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "texture.h"

#include "Camera.h"

#include <vector>
#include <string>
#include <math.h>

using namespace glm;
using namespace std;

Scene::Scene(RenderingEngine* renderer) : renderer(renderer) {
	/*
	MyTexture texture;
	InitializeTexture(&texture, "image1-mandrill.png", GL_TEXTURE_RECTANGLE);

	//Load texture uniform
	//Shaders need to be active to load uniforms
	glUseProgram(renderer->shaderProgram);
	//Set which texture unit the texture is bound to
	glActiveTexture(GL_TEXTURE0);
	//Bind the texture to GL_TEXTURE0
	glBindTexture(GL_TEXTURE_RECTANGLE, texture.textureID);
	//Get identifier for uniform
	GLuint uniformLocation = glGetUniformLocation(renderer->shaderProgram, "imageTexture");
	//Load texture unit number into uniform
	glUniform1i(uniformLocation, 0);

	if(renderer->CheckGLErrors()) {
		cout << "Texture creation failed" << endl;
	}

		// three vertex positions and assocated colours of a triangle
	rectangle.verts.push_back(vec3( -0.9f, -0.9f, 1.0f));
	rectangle.verts.push_back(vec3( 0.9f,  -0.9f, 1.0f));
	rectangle.verts.push_back(vec3( 0.9f, 0.9f, 1.0f));
	rectangle.verts.push_back(vec3( -0.9f, -0.9f, 1.0f));
	rectangle.verts.push_back(vec3( 0.9f, 0.9f, 1.0f));
	rectangle.verts.push_back(vec3( -0.9f, 0.9f, 1.0f));

	rectangle.colors.push_back(glm::vec3( 1.0f, 0.0f, 0.0f));
	rectangle.colors.push_back(glm::vec3( 1.0f, 0.0f, 0.0f));
	rectangle.colors.push_back(glm::vec3( 1.0f, 0.0f, 0.0f));
	rectangle.colors.push_back(glm::vec3( 1.0f, 0.0f, 0.0f));
	rectangle.colors.push_back(glm::vec3( 1.0f, 0.0f, 0.0f));
	rectangle.colors.push_back(glm::vec3( 1.0f, 0.0f, 0.0f));



	rectangle.drawMode = GL_TRIANGLES;

	rectangle.uvs.push_back(vec2( 0.0f, 0.0f));
	rectangle.uvs.push_back(vec2( float(texture.width), 0.f));
	rectangle.uvs.push_back(vec2( float(texture.width), float(texture.height)));
	rectangle.uvs.push_back(vec2( 0.0f, 0.0f));
	rectangle.uvs.push_back(vec2( float(texture.width), float(texture.height)));
	rectangle.uvs.push_back(vec2(0.0f, float(texture.height)));

	//Construct vao and vbos for the triangle
	RenderingEngine::assignBuffers(rectangle);

	//Send the triangle data to the GPU
	//Must be done every time the triangle is modified in any way, ex. verts, colors, normals, uvs, etc.
	RenderingEngine::setBufferData(rectangle);

	//Add the triangle to the scene objects
	objects.push_back(rectangle); */
	glEnable(GL_DEPTH_TEST);

	camera = new Camera(vec3(0.0, 0.0, 0.0));
	updateCamera(90.0, 0.0, 5.0);

	perspective = glm::perspective(glm::radians(45.0f), GLfloat(1024)/GLfloat(1024), 1.0f, 150.0f);
	glUseProgram(renderer->shaderProgram);
	GLuint puniform = glGetUniformLocation(renderer->shaderProgram, "P");
	glUniformMatrix4fv(puniform, 1, GL_FALSE, glm::value_ptr(perspective));


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



	initEarth();
	initSun();
	initStars();
	initMars();
	initMoon();




	earth_rotate = 36.0;
	earth_orbit = 0.09863;
	//earth_orbit = 2.0;


	sun_rotate = 1.4184;

	mars_rotate = 35.089;
	mars_orbit = 0.0524;

	/*earth_rotate = 0.0;
	earth_orbit = 0.0; */

	moon_rotate = 1.3177;
	moon_orbit = 1.3177;
	//moon_orbit = 2.0;


	//updateScene();

}

Scene::~Scene() {

}

void Scene::parseFile(FILE* filename, Geometry* g){
	vector<int> vIndices, vtIndices, vnIndices;
	vector<vec4> temp_v;
	vector<vec2> temp_vt;
	vector<vec4> temp_vn;
	FILE* pfile;
	pfile = fopen("test.txt", "w+");
	char lh[64];

	while(fscanf(filename, "%s", &lh[0]) != EOF) {
		if(strcmp(lh, "v") == 0) {
			vec4 v = vec4(0.0, 0.0, 0.0, 1.0);
			fscanf(filename, "%f %f %f\n", &v.x, &v.y, &v.z);
			temp_v.push_back(v);
			fprintf(pfile, "%s %f %f %f %f\n", lh, v.x, v.y, v.z, v.w);

		} else if(strcmp(lh, "vt") == 0){
			vec2 vt;
			fscanf(filename, "%f %f\n", &vt.x, &vt.y);
			temp_vt.push_back(vt);
			fprintf(pfile, "%s %f %f\n", lh, vt.x, vt.y);

		} else if(strcmp(lh, "vn") == 0){
			vec4 vn = vec4(0.0, 0.0, 0.0, 1.0);
			fscanf(filename, "%f %f %f\n", &vn.x, &vn.y, &vn.z);
			temp_vn.push_back(vn);
			fprintf(pfile, "%s %f %f %f %f\n", lh, vn.x, vn.y, vn.z, vn.w);

		} else if(strcmp(lh, "f") == 0){
			int vi[3], vti[3], vni[3];
			fscanf(filename, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vi[0], &vti[0], &vni[0], &vi[1], &vti[1], &vni[1], &vi[2], &vti[2], &vni[2]);
			fprintf(pfile, "%s %d/%d/%d %d/%d/%d %d/%d/%d\n", lh, vi[0], vti[0], vni[0], vi[1], vti[1], vni[1], vi[2], vti[2], vni[2]);
			vIndices.push_back(vi[0]);
			vIndices.push_back(vi[1]);
			vIndices.push_back(vi[2]);

			vtIndices.push_back(vti[0]);
			vtIndices.push_back(vti[1]);
			vtIndices.push_back(vti[2]);

			vnIndices.push_back(vni[0]);
			vnIndices.push_back(vni[1]);
			vnIndices.push_back(vni[2]);
		}

	}

	for(unsigned int i = 0; i < vIndices.size(); i++){
		int vertexIndex = vIndices[i];
		vec4 vertex = temp_v[vertexIndex - 1];
		g->verts.push_back(vertex);
	}

	for(unsigned int i = 0; i < vtIndices.size(); i++){
		int uvIndex = vtIndices[i];
		vec2 uv = temp_vt[uvIndex - 1];
		g->uvs.push_back(uv);
	}

	for(unsigned int i = 0; i < vnIndices.size(); i++){
		int normalIndex = vnIndices[i];
		vec4 normal = temp_vn[normalIndex - 1];
		g->normals.push_back(normal);
	}
	g->drawMode = GL_TRIANGLES;

	RenderingEngine::assignBuffers(*g);
	RenderingEngine::setBufferData(*g);
	objects.push_back(*g);
	fclose(pfile);



}

void Scene::updateSun(float s_rotate){
	float s_radians = (s_rotate * M_PI)/180.0;
	float r[16]{
		float(cos(s_radians)), 		0.0, 	float(sin(s_radians)), 	0.0,
		0.0, 						1.0, 	0.0, 					0.0,
		float(-sin(s_radians)), 	0.0,	float(cos(s_radians)), 	0.0,
		0.0,						0.0,	0.0, 					1.0
	};


	sun->axis = glm::make_mat4(r) * sun->axis;
	//glm::make_mat4(t) *
}
void Scene::updateEarth(float e_rotate, float eo_rotate){
	float e_radians, eo_radians;

	e_radians = (e_rotate * M_PI)/180.0;

	eo_radians = (eo_rotate * M_PI)/180.0;

	//axis
	float r[16]{
		float(cos(e_radians)), 		0.0, 	float(sin(e_radians)), 	0.0,
		0.0, 						1.0, 	0.0, 					0.0,
		float(-sin(e_radians)), 	0.0,	float(cos(e_radians)), 	0.0,
		0.0,						0.0,	0.0, 					1.0
	};

	//orbit
	float o[16]{
			float(cos(eo_radians)), 	0.0, 	float(sin(eo_radians)), 	0.0,
			0.0, 						1.0, 	0.0, 						0.0,
			float(-sin(eo_radians)), 	0.0,	float(cos(eo_radians)), 	0.0,
			0.0,						0.0,	0.0, 						1.0
	};

	//tilt


	earth->axis = glm::make_mat4(r) * earth->axis;
	earth->orbit = glm::make_mat4(o) * earth->orbit;
	moon->model = glm::make_mat4(o) * moon->model;
}

void Scene::updateMoon(float m_rotate, float mo_rotate){
	float m_radians, mo_radians;

	m_radians = (m_rotate * M_PI)/180.0;

	mo_radians = (mo_rotate * M_PI)/180.0;

		//axis
	float r[16]{
		float(cos(m_radians)), 		0.0, 	float(sin(m_radians)), 	0.0,
		0.0, 							1.0, 	0.0, 						0.0,
		float(-sin(m_radians)), 		0.0,	float(cos(m_radians)), 	0.0,
		0.0,							0.0,	0.0, 						1.0
	};
	//orbit
	float o[16]{
		float(cos(mo_radians)), 	0.0, 	float(sin(mo_radians)), 	0.0,
		0.0, 						1.0, 	0.0, 						0.0,
		float(-sin(mo_radians)), 	0.0,	float(cos(mo_radians)), 	0.0,
		0.0,						0.0,	0.0, 						1.0
	};


	moon->axis = glm::make_mat4(r) * moon->axis;
	moon->orbit = glm::make_mat4(o) * moon->orbit;

}

void Scene::updateMars(float mr_rotate, float mr_orbit){
	float mr_radians, mro_radians;

	mr_radians = (mr_rotate * M_PI)/180.0;

	mro_radians = (mr_orbit * M_PI)/180.0;

	//axis
	float r[16]{
		float(cos(mr_radians)), 		0.0, 	float(sin(mr_radians)), 	0.0,
		0.0, 							1.0, 	0.0, 						0.0,
		float(-sin(mr_radians)), 		0.0,	float(cos(mr_radians)), 	0.0,
		0.0,							0.0,	0.0, 						1.0
	};
	//orbit
	float o[16]{
		float(cos(mro_radians)), 	0.0, 	float(sin(mro_radians)), 	0.0,
		0.0, 						1.0, 	0.0, 						0.0,
		float(-sin(mro_radians)), 	0.0,	float(cos(mro_radians)), 	0.0,
		0.0,						0.0,	0.0, 						1.0
	};


	mars->axis = glm::make_mat4(r) * mars->axis;
	mars->orbit = glm::make_mat4(o) * mars->orbit;

}

void Scene::updateScene(){
	objects.clear();

	updateSun(sun_rotate);
	updateEarth(earth_rotate, earth_orbit);
	updateMars(mars_rotate, mars_orbit);
	updateMoon(moon_rotate, moon_orbit);
	objects.push_back(*sun);
	objects.push_back(*earth);
	objects.push_back(*stars);
	objects.push_back(*mars);
	objects.push_back(*moon);
}

void Scene::updateCamera(float phi, float theta, float radius){
	float radians_phi, radians_theta, x, y, z;

	radians_phi = (phi * M_PI)/180.0;
	radians_theta = (theta * M_PI)/180.0;

	x = radius * cos(radians_theta) * cos(radians_phi);
	y = radius * sin(radians_theta);
	z = radius * cos(radians_theta) * sin(radians_phi);

	camera->eye = vec3(x, y, -z);
	view = glm::lookAt(camera->eye, vec3(0.0, 0.0, 0.0),  vec3(0.0, 1.0, 0.0));

	glUseProgram(renderer->shaderProgram);
	GLuint vuniform = glGetUniformLocation(renderer->shaderProgram, "V");
	glUniformMatrix4fv(vuniform, 1, GL_FALSE, glm::value_ptr(view));

	GLuint eyeuniform = glGetUniformLocation(renderer->shaderProgram, "eye");
	glUniform3fv(eyeuniform, 1, glm::value_ptr(camera->eye));
}

void Scene::initEarth(){
	FILE * pfile;
	float r[16]{
		1.0, 	0.0, 	0.0, 	0.0,
		0.0, 	1.0, 	0.0, 	0.0,
		0.0, 	0.0,	1.0, 	0.0,
		5.84784,	0.0,	0.0, 	1.0
	};

	float t[16]{
		float(cos(0.409105177)), 	float(-sin(0.409105177)), 	0.0, 	0.0,
		float(sin(0.409105177)), 	float(cos(0.409105177)), 	0.0, 	0.0,
		0.0, 						0.0,						1.0, 	0.0,
		0.0,						0.0,						0.0, 	1.0
	};
	earth = new Geometry("earth.jpg", 0.5, 0.5);
	earth->uniformlocation = glGetUniformLocation(renderer->shaderProgram, "imageTexture");
	earth->reference = glm::make_mat4(r);
	earth->tilt = glm::make_mat4(t);
	pfile = fopen("earth.obj", "r");
	parseFile(pfile, earth);
	fclose(pfile);

}

void Scene::initSun(){
	FILE * pfile;
	pfile = fopen("sun.obj", "r");
	sun = new Geometry("sun.jpg", 0.0, 0.0);
	sun->reference = mat4(1.0);
	sun->uniformlocation = glGetUniformLocation(renderer->shaderProgram, "imageTexture");
	parseFile(pfile, sun);
	fclose(pfile);
}

void Scene::initMoon(){
	FILE * pfile;
	float r[16]{
		1.0, 	0.0, 	0.0, 	0.0,
		0.0, 	1.0, 	0.0, 	0.0,
		0.0, 	0.0,	1.0, 	0.0,
		2.0,	0.0,	0.0, 	1.0
	};

	float t[16]{
		float(cos(0.116588)), 	float(-sin(0.116588)), 	0.0, 	0.0,
		float(sin(0.116588)), 	float(cos(0.116588)), 	0.0, 	0.0,
		0.0, 						0.0,						1.0, 	0.0,
		0.0,						0.0,						0.0, 	1.0
	};
	moon = new Geometry("moon.jpg", 0.5, 0.5);
	moon->uniformlocation = glGetUniformLocation(renderer->shaderProgram, "imageTexture");
	moon->reference = glm::make_mat4(r);
	moon->tilt = glm::make_mat4(t);
	moon->planet_reference = earth->reference;
	pfile = fopen("moon.obj", "r");
	parseFile(pfile, moon);
	fclose(pfile);
}

void Scene::initMars(){
	FILE * pfile;
	float r[16]{
		1.0, 	0.0, 	0.0, 	0.0,
		0.0, 	1.0, 	0.0, 	0.0,
		0.0, 	0.0,	1.0, 	0.0,
		(2.0*5.97866),	0.0,	0.0, 	1.0
	};

	float t[16]{
		float(cos(0.43964844)), 	float(-sin(0.43964844)), 	0.0, 	0.0,
		float(sin(0.43964844)), 	float(cos(0.43964844)), 	0.0, 	0.0,
		0.0, 						0.0,						1.0, 	0.0,
		0.0,						0.0,						0.0, 	1.0
	};
	pfile = fopen("mars.obj", "r");
	mars = new Geometry("mars.jpg", 0.5, 0.5);
	mars->reference = glm::make_mat4(r);
	mars->tilt = glm::make_mat4(t);
	mars->uniformlocation = glGetUniformLocation(renderer->shaderProgram, "imageTexture");
	parseFile(pfile, mars);
	fclose(pfile);
}

void Scene::initStars(){
	FILE * pfile;
	stars = new Geometry("stars.jpg", 0.0, 0.0);
	stars->uniformlocation = glGetUniformLocation(renderer->shaderProgram, "imageTexture");
	pfile = fopen("stars.obj", "r");
	parseFile(pfile, stars);
	fclose(pfile);
}

void Scene::resetScene(){
	objects.clear();
	initSun();
	initEarth();
	initMoon();
	initMars();
	initStars();
}
void Scene::displayScene() {

	//glDepthFunc(GL_LESS);
	renderer->RenderScene(objects);

}

