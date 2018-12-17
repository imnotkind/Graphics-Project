#version 330 core


uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 normaltrans; // supposed to be rotation only from viewtrans
uniform vec4 vicolor;

uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 light1;

// Ouput data
in vec4 vocolor;
out vec4 color;

void main()
{

	// Output color = red
	color = vocolor;

}
