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
    factor = dot(wp, plane);
     //
     if(abs(factor)<0.1){
          gl_Position =projection * view*model*vec4(aPos.x,aPos.y,aPos.z,1.0);
     }else{
          // Chieu diem len mat
//         vec3 _normal= vec3(plane.xyz);
//         vec3 _source= vec3(wp.xyz);
//         float _d = plane.w;
//         float distance = dot(wp, plane) - plane.w;// dotProduct(p.getNormal(), Q) - p.getD();
//         vec3 p1 = vec3(_source.x + distance*_normal.x,_source.y + distance*_normal.y,_source.z + distance*_normal.z)  ;
//         vec3 p2 = vec3(_source.x - distance*_normal.x,_source.y - distance*_normal.y,_source.z - distance*_normal.z)  ;
//         float val = _normal.x * p1.x + _normal.y * p1.y + _normal.z * p1.z + _d;
//         if(wp.y>=0){
//          if(abs(val-0) <0.1){
//              gl_Position =projection * view*vec4(p1.x,p1.y,p1.z,1.0);
//              factor=0;
//          }else{
//              gl_Position =projection * view*vec4(p2.x,p2.y,p2.z,1.0);
//              factor=0;
//          }
//         }else{
//              gl_Position =projection * view*model*vec4(aPos.x,aPos.y,aPos.z,1.0);
//              factor=2.0;
//         }

       gl_Position =projection * view*model*vec4(aPos.x,aPos.y,aPos.z,1.0);
     }
    Tex=aTex;

    WorldPos= aPos;
   gl_ClipDistance[0]=dot(wp, plane);
};
