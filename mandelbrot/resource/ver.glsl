#version 460 core
precision highp float;

in vec4 position;
uniform mat4 trans;
uniform float scale;

out vec2 coord;

void main()
{
    coord = (scale*position).xy;
    gl_Position = trans*position ;
}
