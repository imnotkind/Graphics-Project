#version 460 core
precision highp float;

// Ouput data
in vec2 coord;
out vec4 color;

uniform float t;

uniform sampler2D pallete;

float mynorm(float x)
{
	return 0.5 + 0.5 * tanh(x);
}

void main()
{

	double x = 1.5 * coord[0]* sin(t*0.9); 
	double y = 1.5 * coord[1]* cos(t);

	double cr = 1.5 * coord[0];
	double ci = 1.5 * coord[1];

	int max_iter = 200;

	float max = 4;
	float tmp;
	double ratio = 1;
	vec2 uv; //texture doesn't support double vec

	int i;
	for(i = 0; i < max_iter; i++)
	{
		double nx = x*x-y*y + cr;
		double ny = 2*x*y + ci;

		if (nx*nx + ny * ny > max)
		{
			ratio = (nx*nx) / (ny*ny);
			break;
		}

		x = nx;
		y = ny;
	}

	float z = float(i) / max_iter;
	float w = mynorm(float(x/(y+0.1)));

	
	uv.y = 0.5;

	uv.x = z;
	vec4 color1 = texture2D(pallete, uv) * (1-z);

	uv.x = (1-z) * w;
	vec4 color2 = texture2D(pallete, uv) * (1-z);

	uv.x = sin(z*3.1415);
	vec4 color3 = texture2D(pallete, uv) * (0.5* sin(t) + 0.5);


	color = color1 + color2 + color3;
	color = color * 0.3333;
	color[3] = 1;



	return;





	//color = vec4(float(i) / max_iter, float(i) / max_iter, float(i) / max_iter, 1);
}
