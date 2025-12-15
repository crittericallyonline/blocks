#version 100

// id,k
attribute vec3 vertex_position; 
attribute vec2 texcoord;

// in
uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform float time;

// out
varying vec2 TexCoord;

void main()
{
    gl_Position = projection_matrix * modelview_matrix * vec4(vertex_position, 1.0);
    TexCoord = texcoord;
}