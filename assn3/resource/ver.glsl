#version 330 core
in vec4 position;
uniform mat4 trans;
uniform vec4 vicolor;
out vec4 vocolor;

void main()
{
    vocolor = vicolor;
    gl_Position = trans*position ;
}
