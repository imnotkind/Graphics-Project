#version 330 core

// Ouput data
in vec2 coord;
out vec4 color;

uniform float t;

void main()
{

	float x = 0; 
	float y = 0;

	float cr = coord[0];
	float ci = coord[1];

	for(int i = 0; i < 20; i++)
	{
		float nx = x*x-y*y + cr;
		float ny = 2*x*y + ci;

		x = nx;
		y = ny;
	}

	vec4 r = vec4(tanh(x),tanh(y),tanh(x*x+y*y), 1);

	color = (r + vec4(1,1,1,1))*0.5;

}
