#version 330 core


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

out vec4 color;

void main()
{
	vec4 amb_r = ambient;
	color = amb_r;
	for(int i = 0; i < 3; i++)
	{
		vec4 xx = light[i].pos[3] == 0 ? light[i].pos : light[i].pos - vpos;
		vec3 L = normalize((xx).xyz); //vector of light
		vec3 E = normalize(-vpos.xyz); //vector of eye (since eye is in origin)
		vec3 R = normalize(-reflect(L,vnormal));
	

		float katt = light[i].pos[3] == 0 ? 1 : 5.0/length(light[i].pos- vpos);

		vec4 dif_r = vicolor*light[i].diffuse * katt * max(dot(vnormal,L), 0.0); // NL is negative : backside
		vec4 spc_r = vicolor*light[i].specular * katt * pow(max(dot(R,E), 0.0), 0.8);
		color += clamp(dif_r, 0.0, 1.0) + clamp(spc_r, 0.0, 0.5);
	}
	
	color = clamp(color, 0.0, 1.0);
	color[3] = vicolor[3];


}
