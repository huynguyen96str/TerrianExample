#version 330 core
out vec4 FragColor;
in vec2  textcoord;
uniform sampler2D texturew;

void main(){
 // FragColor = vec4(1.0f, 0.0f,0.0f,1.f);
    FragColor = texture(texturew,textcoord);
};
