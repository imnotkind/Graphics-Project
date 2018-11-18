#version 330 core

// Ouput data
in vec2 coord;
out vec4 color;

uniform float t;

void main()
{

	float x = coord[0]; 
	float y = coord[1];

	vec4 r = vec4(sin(x+t), cos(y+t), sin(x)*cos(y)*sin(t), 1);

	color = (r + vec4(1,1,1,1))*0.5;

}
