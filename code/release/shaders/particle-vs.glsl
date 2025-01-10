#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

out VS_OUT {
    vec3 color;
} vs_out;


void main(){
    gl_Position = ViewMatrix * vec4(aPos.x, aPos.y, 0.0, 1.0);
    vs_out.color = aColor;
}