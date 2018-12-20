#version 330 core
in vec4 position;
in vec2 texcoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec4 vicolor;


out vec4 vocolor;

void main()
{

    vocolor = vicolor;// * texcoord[0] * texcoord[1];
    gl_Position = projection * view * model * position ;
}
