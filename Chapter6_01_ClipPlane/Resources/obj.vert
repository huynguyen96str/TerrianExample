#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
out vec2 Tex;
out vec3 WorldPos;

const vec4 plane = vec4(0,-1,0,0); // ABC: toa do x,y,z cua normal cua plane, D: khoang cach tu origin den mat phang
void main(){
    vec4 wp = model*vec4(aPos.x,aPos.y,aPos.z,1.0);
    gl_ClipDistance[0]=dot(wp, plane);
    gl_Position =projection * view*model*vec4(aPos.x,aPos.y,aPos.z,1.0);
    Tex=aTex;
    WorldPos= aPos;

};
