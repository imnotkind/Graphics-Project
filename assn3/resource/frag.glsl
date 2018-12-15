#version 400 core
precision highp float;
// Ouput data
in vec2 coord;
out vec4 color;

uniform float t;

void main()
{

	double x = 1.5 * coord[0] * sin(t*0.9); 
	double y = 1.5 * coord[1] * cos(t);

	double cr = 1.5 * coord[0];
	double ci = 1.5 * coord[1];

	int max_iter = 200;

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
