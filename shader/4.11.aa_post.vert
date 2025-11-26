#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

//layout (std140) uniform Matrices {
//    mat4 projection;
//    mat4 view;
//};
//uniform mat4 model;

void main() {
    gl_Position =  vec4(aPos, 0.0, 1.0);
    TexCoords = aTexCoords;
}