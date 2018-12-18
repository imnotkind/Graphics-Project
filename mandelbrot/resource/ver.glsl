#version 460 core
precision highp float;

in vec4 position;
uniform mat4 trans;
uniform float scale;
uniform float t;

out vec2 coord;

void main()
{
	coord.x = position.x;
	coord.y = position.y;
    //coord = (scale*position).xy;
    gl_Position =  position - t;
}
