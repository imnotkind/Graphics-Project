#version 460 core
precision highp float;

// Ouput data
in vec2 coord;
out vec4 color;

uniform float t;
uniform sampler2D pallete;

float tanhnorm(float x)
{
	return 0.5 + 0.5 * tanh(x);
}

float sinnorm(float x)
{
	return 0.5 + 0.5 * sin(x);
}

dvec2 zoomto(double zoom, vec2 coord, dvec2 center)
{
	return coord / zoom + center;
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
	vec4 color3 = texture2D(pallete, uv) * (0.5* sin(t) + 0.5);


	vec4 clr = color1 + color2 + color3;
	clr = clr * 0.3333;
	clr[3] = 1;

	return clr;
}

vec4 haebincolor(float a, float b)
{
	vec2 uv;
	uv.x = sinnorm(exp(exp(a)) * b);
	uv.y = sinnorm(exp(exp(b)) * a);
	vec4 color1 = texture2D(pallete, uv) * (1 - a) * a * b;

	uv.x = sinnorm(exp(exp(a)));
	uv.y = sinnorm(exp(exp(a)));
	vec4 color2 = texture2D(pallete, uv) * (1 - a) * a;


	vec4 clr = color1 * 2  +  color2 * 2.5;
	clr[3] = 1;

	return clr;

}

vec4 defaultmandelbrot(float a)
{
	vec4 clr = vec4(a, a, a, 1);
	return clr;
}

vec4 noanti(vec2 point)
{
	double zoom = exp(sinnorm(t) * 20);
	dvec2 cen = dvec2(-0.7280101473, 0.1945);
	dvec2 c = zoomto(zoom, point, cen);

	double x = c.x; // * sin(1/float(zoom + 1) * 0.9);
	double y = c.y; // * cos(1/float(zoom + 1));

	int max_iter = 200;

	float max = 4;
	

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

	float a = float(i) / (max_iter);
	float b = tanhnorm(float(x/(y)));

	
	return haebincolor(a,b);
}

vec4 anti(vec2 point)
{
	vec4 ant[4][4];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			ant[i][j] = noanti(coord + vec2(0.001*i, 0.001*j));

	vec4 clr = vec4(0, 0, 0, 0);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			clr += ant[i][j];


	clr = clr / 9.0;
	return clr;
}

void main()
{
	//dvec2 p = dvec2(-1.48458333312, 0.0);
	//dvec2 p = dvec2(-0.7280101473, 0.1945);

	
	color = noanti(coord);
	
	return;
}
