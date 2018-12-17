#version 330 core
in vec4 position;
in vec2 texcoord;

uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 normaltrans; // supposed to be rotation only from viewtrans
uniform vec4 vicolor;



out vec4 vocolor;

void main()
{
    vocolor = vicolor * texcoord[0] * texcoord[1];
    gl_Position = projection * modelview * position ;
}
