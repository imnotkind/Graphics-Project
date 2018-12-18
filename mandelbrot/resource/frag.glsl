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
	double k;
	k = (exp(t));

	//double px = -1.48458333312;
	//double py = 0;
	
	double px = -0.7280101473;
	double py = 0.1945;


	double cr = (1.5 * (coord[0]))/k + px;
	double ci = (1.5 * (coord[1]))/k + py;

	double x = cr;//* sin(1 / (t*t*t) * 0.9);
	double y = ci;//* cos(1/(t*t*t));

	int max_iter = 500;

	float max = 4;
	float tmp;
	float ratio = 1;
	vec2 uv; //texture doesn't support double vec

	int i;
	for(i = 0; i < max_iter; i++)
	{
		double nx = x*x-y*y + cr;
		double ny = 2*x*y + ci;

		if (nx*nx + ny * ny > max)
			break;

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
	vec4 color3 = texture2D(pallete, uv); //* (0.5* sin(t) + 0.5);


	color = color1 + color2 + color3;
	color = color * 0.3333;
	color[3] = 1;

	


	uv.x = z;
	uv.y = clamp(z*ratio, 0.0, 1.0);
	color = texture2D(pallete, uv);


	

	color = vec4(float(i) / max_iter, float(i) / max_iter, float(i) / max_iter, 1);
	return;
}
