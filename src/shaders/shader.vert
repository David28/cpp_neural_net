// Vertex shader
#version 120

varying vec2 vTexCoord;

void main() {
    // Set the position of the vertex
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // Pass the texture coordinates to the fragment shader
    vTexCoord = gl_MultiTexCoord0.xy;
}