#version 330 core
in vec4 position;
in vec3 normal;
in vec2 texcoord;

uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 normaltrans; // supposed to be rotation only from viewtrans
uniform vec4 vicolor;

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

	vpos = modelview * p;
	opos = p.xyz;
    gl_Position = projection * vpos;
	
}
