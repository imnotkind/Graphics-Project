#version 330 core
in vec4 position;
in vec3 normal;
in vec2 texcoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat4 normaltrans; // supposed to be rotation only from viewtrans
uniform vec4 vicolor;

uniform float t;
uniform float fractal1;


out vec3 vnormal;
out vec3 opos;

out vec4 vpos;
out vec3 mpos;
out vec2 vtex;

void main()
{
    vnormal = normalize(normaltrans * vec4(normal, 0.0)).xyz;

	vtex = texcoord;
	vec4 p = position;
	
	vec4 temp = model * p;
	vpos = view * temp;
	opos = temp.xyz;
	mpos = p.xyz * 0.01;

	vpos[0] += fractal1 * 0.1*sin(sin(0.3*t) * vpos[1] + 1.8*vpos[2]);
	vpos[1] += fractal1 * 0.1*sin(0.9 * cos(0.8*t) * vpos[2] + 1.3*vpos[0]);
	vpos[2] += fractal1 * 0.1*sin(0.7 * cos(t) * vpos[1] + 1.5*vpos[0]);

    gl_Position = projection * vpos;
}
