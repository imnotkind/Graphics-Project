#version 330 core
in vec4 position;
in vec3 normal;
in vec2 texcoord;

uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 normaltrans; // supposed to be rotation only from viewtrans
uniform vec4 vicolor;

uniform float t;
uniform float fractal1;


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

	vpos[0] += fractal1 * 0.1*sin(t * vpos[1] + 0.8*vpos[2]);
	vpos[1] += fractal1 * 0.1*sin(0.9 * t * vpos[2] + 0.3*vpos[0]);
	vpos[2] += fractal1 * 0.1*sin(0.7 * t * vpos[1] + 0.5*vpos[0]);

	opos = p.xyz;
    gl_Position = projection * vpos;
	
}
