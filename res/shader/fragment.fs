#version 100

precision mediump float;

uniform sampler2D imageTex; // global shit
varying vec2 TexCoord; // varying meaning passed from vertex
varying float time;

void main()
{
    gl_FragColor = texture2D(imageTex, TexCoord);
    // gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
}