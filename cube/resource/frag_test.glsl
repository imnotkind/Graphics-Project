#version 400 core


// Ouput data
in vec3 vnormal;
in vec4 vpos;
in vec2 vtex;

in vec3 opos;
in vec3 mpos;

uniform vec4 vicolor;
uniform float fractal1;
uniform sampler2D pallete;

struct SLight
{
	vec4 pos;
	vec4 diffuse;
	vec4 specular;
};
uniform vec4 ambient;

uniform SLight light[3];

uniform float t;

out vec4 color;

int max_iter = 50;
vec3 mandel(double x, double y, double cr, double ci)
{
	float max_d = 10;

	int i;
	for(i = 0; i < max_iter; i++)
	{
		double nx = x*x-y*y + cr;
		double ny = 2*x*y + ci;

		if (nx*nx + ny * ny > max_d) break;

		x = nx;
		y = ny;
	}
	return vec3(i, x, y);
}
float mynorm(float x)
{
	return 0.5 + 0.5 * tanh(x);
}












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


























void main()
{

	
	float mx = (opos[0] + opos[2])/fractal1;
	float my = (opos[2] + opos[1])/fractal1;

	mx = sin(mx);
	my = sin(my);

	double cr = double(mx);
	double ci = double(my);


	double x = cr * sin(t * 0.9); 
	double y = ci * cos(t);


	vec3 z = mandel(x, y, cr, ci);

	vec4 mcolor = vicolor;
	mcolor[0] = 0.5 + 0.5*sin(10/fractal1*(opos[0] + opos[1]));
	mcolor[1] = 0.5 + 0.5*sin(10/fractal1*(opos[1] + opos[2]));
	mcolor[2] = 0.5 + 0.5*sin(10/fractal1*(opos[0] + opos[2]));

	mcolor = vec4(float(z[0] + 25)/(max_iter + 25), 0, 0, 1);
	mcolor = vec4(1, 0, 0, 1);
	mcolor[3] = 1;

	vec4 amb_r = ambient;
	color = noanti(vec2(cr, ci));

	for(int i = 0; i < 3; i++)
	{
		vec4 xx = light[i].pos[3] == 0 ? light[i].pos : light[i].pos - vpos;
		vec3 L = normalize((xx).xyz); //vector of light
		vec3 E = normalize(-vpos.xyz); //vector of eye (since eye is in origin)
		vec3 R = normalize(-reflect(L,vnormal));
	

		float katt = light[i].pos[3] == 0 ? 1 : 5.0/length(light[i].pos- vpos);

		vec4 dif_r = mcolor*light[i].diffuse * katt * max(dot(vnormal,L), 0.0); // NL is negative : backside
		vec4 spc_r = mcolor*light[i].specular * katt * pow(max(dot(R,E), 0.0), 0.8);
		color += clamp(dif_r, 0.0, 1.0) + clamp(spc_r, 0.0, 0.5);
	}
	
	color = clamp(color + mcolor*amb_r, 0.0, 1.0);
	color[3] = mcolor[3];
}
