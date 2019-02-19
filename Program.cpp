/*
 * Program.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#include "Program.h"

#include <iostream>
#include <string>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RenderingEngine.h"
#include "Scene.h"

float radius;
float c_theta;
float c_phi;
float t;
float dx;
float dy;
float x;
float y;

float angle1;
float angle2;

bool click;

bool pause = false;

Scene* currentScene;
Program::Program() {
	setupWindow();
}

Program::~Program() {
	//Must be cleaned up in the destructor because these are allocated to the heap
	delete renderingEngine;
	delete scene;
}

void Program::start() {
	radius = 5.0f;
	c_theta = 0.0;
	c_phi = 90.0;
	double lastTime = glfwGetTime();
	x = 0.0;
	y = 0.0;
	renderingEngine = new RenderingEngine();
	scene = new Scene(renderingEngine);
	currentScene = scene;
	//Main render loop
	scene->updateScene();
	while(!glfwWindowShouldClose(window)) {
		double currentTime = glfwGetTime();
		if (currentTime - lastTime >= 0.1 && pause == false ) {
			scene->updateScene();
			lastTime += 0.1;
		}
		scene->displayScene();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}

void Program::setupWindow() {
	//Initialize the GLFW windowing system
	if (!glfwInit()) {
		std::cout << "ERROR: GLFW failed to initialize, TERMINATING" << std::endl;
		return;
	}

	//Set the custom error callback function
	//Errors will be printed to the console
	glfwSetErrorCallback(ErrorCallback);

	//Attempt to create a window with an OpenGL 4.1 core profile context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	int width = 1024;
	int height = 1024;
	window = glfwCreateWindow(width, height, "CPSC 453 OpenGL Boilerplate", 0, 0);
	if (!window) {
		std::cout << "Program failed to create GLFW window, TERMINATING" << std::endl;
		glfwTerminate();
		return;
	}

	glfwSetScrollCallback(window, ScrollCallback);

	//Set the custom function that tracks key presses
	glfwSetKeyCallback(window, KeyCallback);

	glfwSetMouseButtonCallback(window, MouseButtonCallback);

	glfwSetCursorPosCallback(window, CursorPosCallback);

	//Bring the new window to the foreground (not strictly necessary but convenient)
	glfwMakeContextCurrent(window);

	//Intialize GLAD (finds appropriate OpenGL configuration for your system)
	if (!gladLoadGL()) {
		std::cout << "GLAD init failed" << std::endl;
		return;
	}

	//Query and print out information about our OpenGL environment
	QueryGLVersion();
}



void Program::QueryGLVersion() {
	// query opengl version and renderer information
	std::string version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	std::string glslver = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	std::string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

	std::cout << "OpenGL [ " << version << " ] "
		<< "with GLSL [ " << glslver << " ] "
		<< "on renderer [ " << renderer << " ]" << std::endl;
}

void ErrorCallback(int error, const char* description) {
	std::cout << "GLFW ERROR " << error << ":" << std::endl;
	std::cout << description << std::endl;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//Key codes are often prefixed with GLFW_KEY_ and can be found on the GLFW website
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		pause = !pause;
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		currentScene->resetScene();
	}
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset){
	radius-=float(yoffset);
	if (radius < 3.0f) {
		radius = 3.0;
	} else if (radius > 29.5){
		radius = 29.5;
	}
	currentScene->updateCamera(c_phi, c_theta, radius);

}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		click = true;
	} else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
		click = false;
	}
}

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos){
	if(click){
		dx = 0.25*(x-float(xpos));
		dy = 0.25*(y-float(ypos));
		c_phi += dx;
		c_theta += dy;


		if(c_theta <= -90.0){
			c_theta= -89.999;
		} else if(c_theta >= 90.0){
			c_theta = 89.999;
		}


		currentScene->updateCamera(c_phi, c_theta, radius);


	}

	x = float(xpos);
	y = float(ypos);

}
