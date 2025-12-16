#version 300 es

precision highp float;

in vec2 TexCoord;

uniform sampler2D imageTex;
uniform vec3 color;

out vec4 diffuseColor;

void main()
{
    vec2 uv = TexCoord;
    diffuseColor = texture(imageTex, uv);
}