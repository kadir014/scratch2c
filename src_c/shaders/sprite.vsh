#version 460

layout (location = 0) in vec2 in_pos;

void main(){
    gl_Position = vec4(in_pos.x, in_pos.y, 0.0, 1.0);
}