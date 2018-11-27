#version 330 core
in vec4 position;
in vec3 normal;

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




out vec3 vnormal;
out vec4 vpos;

void main()
{
    vnormal = normalize(normaltrans * vec4(normal, 0.0)).xyz;

	vpos = modelview * position;
    gl_Position = projection * vpos;
	
}
