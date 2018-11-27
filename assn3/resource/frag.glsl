#version 400 core
precision highp float;
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

	int max_iter = 1000;

	float max = 10;

	int i;
	for(i = 0; i < max_iter; i++)
	{
		double nx = x*x-y*y + cr;
		double ny = 2*x*y + ci;

		if (nx*nx + ny * ny > max) break;

		x = nx;
		y = ny;
	}

	color = vec4(float(i)/max_iter, 0, 0, 1);
}
