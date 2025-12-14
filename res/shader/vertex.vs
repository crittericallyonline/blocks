#version 100

attribute vec3 vertex_position; 

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;


uniform vec2 texCoord;
varying vec2 TexCoord;

void main()
{
    gl_Position = projection_matrix * modelview_matrix * vec4(vertex_position, 1.0);
    // gl_Position = vec4(vertex_position, 1.0);
    TexCoord = vec2(texCoord.x, 1.0-texCoord.y);
}