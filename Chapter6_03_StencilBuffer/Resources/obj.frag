#version 330 core
out vec4 FragColor;
in vec2  Tex;
in vec3 WorldPos;
in float factor;

uniform sampler2D gTextureHeight0;
uniform sampler2D gTextureHeight1;
uniform sampler2D gTextureHeight2;
uniform sampler2D gTextureHeight3;

uniform float gHeight0 = 64.0;
uniform float gHeight1 = 128.0;
uniform float gHeight2 = 193.0;
uniform float gHeight3 = 256.0;

vec4 CalcTexColor()
{
    vec4 TexColor;
    float Height = WorldPos.y;

    if (Height < gHeight0) {
       TexColor = texture(gTextureHeight0, Tex);
    } else if (Height < gHeight1) {
       vec4 Color0 = texture(gTextureHeight0, Tex);
       vec4 Color1 = texture(gTextureHeight1, Tex);
       float Delta = gHeight1 - gHeight0;
       float Factor = (Height - gHeight0) / Delta;
       TexColor = mix(Color0, Color1, Factor);
    } else if (Height < gHeight2) {
       vec4 Color0 = texture(gTextureHeight1, Tex);
       vec4 Color1 = texture(gTextureHeight2, Tex);
       float Delta = gHeight2 - gHeight1;
       float Factor = (Height - gHeight1) / Delta;
       TexColor = mix(Color0, Color1, Factor);
    } else if (Height < gHeight3) {
       vec4 Color0 = texture(gTextureHeight2, Tex);
       vec4 Color1 = texture(gTextureHeight3, Tex);
       float Delta = gHeight3 - gHeight2;
       float Factor = (Height - gHeight2) / Delta;
       TexColor = mix(Color0, Color1, Factor);
    } else {
       TexColor = texture(gTextureHeight3, Tex);
    }

    return TexColor;
}


void main()
{
  vec4 TexColor = CalcTexColor();
//   if(abs(factor)<0.1){
//       if(abs(factor)==0){
//          FragColor= vec4(1.f,0.f,0.f,1.f);
//       }else{
//          FragColor= vec4(1.f,1.f,0.f,1.f);
//       }
//  }else{
      FragColor =  TexColor;
 // }
}
