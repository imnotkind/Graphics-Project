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

	int i;
	for(i = 0; i < max_iter; i++)
	{
		double nx = x*x-y*y + cr;
		double ny = 2*x*y + ci;

		if (nx*nx + ny * ny > 4.0) break;

		x = nx;
		y = ny;
	}

	//vec4 r = vec4(tanh(float(x)),tanh(float(y)),tanh(float(x*x+y*y)), 1);

	//color = (r + vec4(1,1,1,1))*0.5;

	if (i == max_iter)
		color = vec4(1, 1, 1, 1);
	else
		color = vec4(0, 0, 0, 1);
}
