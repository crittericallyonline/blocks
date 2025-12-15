#version 100

precision mediump float;

uniform sampler2D imageTex;
varying vec2 TexCoord;
varying float time;

void main()
{
    gl_FragColor = texture2D(imageTex, TexCoord);
}