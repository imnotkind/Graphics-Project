#version 460 core
precision highp float;

// Ouput data
in vec2 coord;
out vec4 color;

uniform float t;

uniform sampler2D pallete;


void main()
{

	double x = 1.5 * coord[0]; //* sin(t*0.9); 
	double y = 1.5 * coord[1]; //* cos(t);

	double cr = 1.5 * coord[0];
	double ci = 1.5 * coord[1];

	int max_iter = 500;

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

	
	

	uv.x = float(i) / max_iter;
	uv.y = clamp(float(ratio), 0.0, 1.0);
	color = texture2D(pallete, uv);
	//color = vec4(float(i) / max_iter, float(i) / max_iter, float(i) / max_iter, 1);
}
