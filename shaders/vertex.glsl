// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec4 VertexPosition;
layout(location = 1) in vec2 VertexUV;
layout(location = 2) in vec4 VertexNormal;

uniform mat4 M = mat4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
);

uniform mat4 V = mat4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
);

uniform mat4 P = mat4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
);

// output to be interpolated between vertices and passed to the fragment stage
out vec2 uv;

out vec4 n;

out vec4 v;

void main()
{
	vec4 newVertexPosition = P * V * M * VertexPosition;
    // assign vertex position without modification
    gl_Position = newVertexPosition;

    // assign output colour to be interpolated
    uv = VertexUV;
    
    n = VertexNormal;
    
    v = newVertexPosition;
}
