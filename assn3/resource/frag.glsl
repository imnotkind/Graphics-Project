#version 400 core

// Ouput data
in vec2 coord;
out vec4 color;

uniform float t;

void main()
{

	double x = 0; 
	double y = 0;

	double cr = coord[0];
	double ci = coord[1];

	for(int i = 0; i < 20; i++)
	{
		double nx = x*x-y*y + cr;
		double ny = 2*x*y + ci;

		x = nx;
		y = ny;
	}

	vec4 r = vec4(tanh(float(x)),tanh(float(y)),tanh(float(x*x+y*y)), 1);

	color = (r + vec4(1,1,1,1))*0.5;

}
