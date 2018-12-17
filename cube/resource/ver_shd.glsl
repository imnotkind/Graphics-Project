#version 330 core
in vec4 position;
in vec3 normal;
in vec2 texcoord;

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


out vec3 vnormal;
out vec3 opos;

out vec4 vpos;
out vec2 vtex;

void main()
{
    vnormal = normalize(normaltrans * vec4(normal, 0.0)).xyz;

	vtex = texcoord;
	vec4 p = position;
	//for(int i = 0; i < 3; i++) p[i] = sin(p[i] * t) * p[(i+2)%3]  + p[(i+1)%3] * (0.5 + 0.4 * cos(0.8*t));

	vpos = modelview * p;
	opos = p.xyz;
    gl_Position = projection * vpos;
	
}
