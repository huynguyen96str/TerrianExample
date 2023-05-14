#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
out vec2 Tex;
out vec3 WorldPos;
out float factor;

const vec4 plane = vec4(0,-1,0,0); // ABC: toa do x,y,z cua normal cua plane, D: khoang cach tu origin den mat phang
const vec4 plane2 = vec4(0,1,0,0); // ABC: toa do x,y,z cua normal cua plane, D: khoang cach tu origin den mat phang
void main(){
    vec4 wp = model*vec4(aPos.x,aPos.y,aPos.z,1.0);
    factor = dot(wp, plane2);
     //
     if(abs(factor)<0.1){
          gl_Position =projection * view*model*vec4(aPos.x,aPos.y,aPos.z,1.0);
     }else{

       gl_Position =projection * view*model*vec4(aPos.x,aPos.y,aPos.z,1.0);
     }
    Tex=aTex;
    WorldPos= aPos;
   gl_ClipDistance[0]=dot(wp, plane2);
  // gl_ClipDistance[1]=dot(wp, plane2);
};
