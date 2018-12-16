#version 400 core


// Ouput data
in vec3 vnormal;
in vec4 vpos;
in vec2 vtex;

in vec3 opos;

uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 normaltrans; // supposed to be rotation only from viewtrans
uniform vec4 vicolor;

uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 light1;
uniform vec4 light2;
uniform vec4 light3;

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
void main()
{

	/*
	float fk = 0.42;
	if(vtex[0] < 0.5 + 0.5*fk && vtex[0] > 0.5 - 0.5*fk)
		if(vtex[1] < 0.5 + 0.5*fk && vtex[1] > 0.5 - 0.5*fk)
		{
			discard;
			return;
			color = vec4(0,1,0,0); return;
		}
	*/

	double x = vpos[0] * sin(t * 0.9); 
	double y = vpos[1] * cos(t);

	double cr = opos[0] + opos[2];
	double ci = opos[2] + opos[1];

	cr = vpos[0]-0.2;
	ci = vpos[1];

	vec3 z = mandel(x, y, cr, ci);

	vec4 mcolor = vec4(1, mynorm(z[1] + 2*z[2]) , mynorm(2*z[1] + z[2]), 1) * float(z[0])/max_iter;
	//mcolor = vec4(float(z[0])/max_iter, 0, 0, 1);



	vec3 L = normalize((light1 - vpos).xyz); //vector of light
	vec3 E = normalize(-vpos.xyz); //vector of eye (since eye is in origin)
	vec3 R = normalize(-reflect(L,vnormal));
	
	float katt = 5.0/length(light1 - vpos);

	vec4 amb_r = ambient;
	vec4 dif_r = mcolor * katt * max(dot(vnormal,L), 0.0); // NL is negative : backside
	vec4 spc_r = mcolor * katt * pow(max(dot(R,E), 0.0), 0.8);
	//
	color = amb_r + clamp(dif_r, 0.0, 1.0) + clamp(spc_r, 0.0, 1.0);
	//color[3] = 0.5;
}
