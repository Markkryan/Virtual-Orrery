// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// interpolated colour received from vertex stage
in vec2 uv;

in vec4 n;
in vec4 v;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

uniform sampler2D imageTexture;

uniform float kd = 1.0;
uniform float ks = 1.0;

uniform vec3 eye;

void main(void)
{
    // write colour output without modification
    //FragmentColour = vec4(1.0, 1.0, 1.0, 0.0);
   // vec4 view = vec4(eye, 1.0f) - v;
   // vec4 l = v - vec4(0.0, 0.0, 0.0, 1.0);
    //vec4 r = 2.0*dot(n, l)*n - l;
   // vec4 i = 0.25*vec4(1.0, 1.0, 1.0, 1.0) + kd*dot(l, n) + ks*pow(dot(r, view), 1.0);
    vec4 image_color = texture(imageTexture, uv);
    //image_color[0] = image_color[0]*i[0];
    //image_color[1] = image_color[0]*i[1];
   // image_color[2] = image_color[0]*i[2];
   // image_color[3] = image_color[0]*i[3];
	FragmentColour = image_color;
}
