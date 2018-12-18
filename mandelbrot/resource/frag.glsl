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

dvec2 zoomto(double speed, vec2 coord, dvec2 center)
{
	return coord / speed + center;
}

vec4 junhacolor(float a, float b)
{
	vec2 uv;
	uv.y = 0.5;

	uv.x = a;
	vec4 color1 = texture2D(pallete, uv) * (1 - a);

	uv.x = (1 - a) * b;
	vec4 color2 = texture2D(pallete, uv) * (1 - a);

	uv.x = sin(a*3.1415);
	vec4 color3 = texture2D(pallete, uv); //* (0.5* sin(t) + 0.5);


	vec4 clr = color1 + color2 + color3;
	clr = clr * 0.3333;
	clr[3] = 1;

	return clr;
}

vec4 haebincolor(float a, float b)
{
	vec2 uv;
	uv.x = a;
	uv.y = b;

	vec4 clr = texture2D(pallete, uv);
	return clr;
}

vec4 defaultmandelbrot(float a)
{
	vec4 clr = vec4(a, a, a, 1);
	return clr;
}

void main()
{
	//dvec2 p = dvec2(-1.48458333312, 0.0);
	dvec2 p = dvec2(-0.7280101473, 0.1945);

	



	double zoom = exp(sin(t)*4) + 3;
	dvec2 cen = dvec2(-1, 0) + 0.3 * dvec2(cos(t*0.87), sin(t*0.87));
	dvec2 c = zoomto(zoom, coord, cen);

	double x = c.x* sin(1/float(zoom-2) * 0.9);
	double y = c.y* cos(1/float(zoom-2));

	int max_iter = 200;

	float max = 4;
	float tmp;
	float ratio = 1;
	

	int i;
	for(i = 0; i < max_iter; i++)
	{
		double nx = x*x-y*y + c.x;
		double ny = 2*x*y + c.y;

		if (nx*nx + ny * ny > max)
			break;

		x = nx;
		y = ny;
	}

	float a = float(i) / max_iter;
	float b = mynorm(float(x/(y+0.1)));

	
	color = junhacolor(a, b);
	return;

	
}
