#version 100

attribute vec3 a_position;

varying vec4 vertexColor;

void main()
{
    gl_Position = vec4(a_position, 1.0);
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
}