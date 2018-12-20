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
void main()
{

	

	double cr = opos[0] + opos[2];
	double ci = opos[2] + opos[1];

	double x = cr * sin(t * 0.9); 
	double y = ci * cos(t);

	cr = vpos[0]-0.2;
	ci = vpos[1];

	vec3 z = mandel(x, y, cr, ci);

	vec4 mcolor = vicolor;
	mcolor[0] = 0.5 + 0.5*sin(opos[0] + opos[1]);
	mcolor[1] = 0.5 + 0.5*sin(opos[1] + opos[2]);
	mcolor[2] = 0.5 + 0.5*sin(opos[0] + opos[2]);

	//mcolor = vec4(1, mynorm(z[1] + 2*z[2]) , mynorm(2*z[1] + z[2]), 1) * float(z[0])/max_iter;
	mcolor = vec4(float(z[0])/max_iter, 0, 0, 1);

	mcolor[3] = 1;

	vec4 amb_r = ambient;
	color = amb_r;

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
	
	color = clamp(color, 0.0, 1.0);
	color[3] = mcolor[3];
}
