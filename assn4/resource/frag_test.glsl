#version 400 core


// Ouput data
in vec3 vnormal;
in vec4 vpos;


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


out vec4 color;

void main()
{

	double x = 0; 
	double y = 0;

	double cr = vpos[0];
	double ci = vpos[1];

	int max_iter = 100;

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

	vec4 mcolor = vec4(float(i)/max_iter, 0, 0, 1);


	vec3 L = normalize((light1 - vpos).xyz); //vector of light
	vec3 E = normalize(-vpos.xyz); //vector of eye (since eye is in origin)
	vec3 R = normalize(-reflect(L,vnormal));
	
	float katt = 5.0/length(light1 - vpos);

	vec4 amb_r = ambient;
	vec4 dif_r = mcolor * katt * max(dot(vnormal,L), 0.0); // NL is negative : backside
	vec4 spc_r = mcolor * katt * pow(max(dot(R,E), 0.0), 0.8);

	color = amb_r + clamp(dif_r, 0.0, 1.0) + clamp(spc_r, 0.0, 0.5);


	

}
