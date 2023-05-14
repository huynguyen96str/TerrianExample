#version 330
out vec4 FragColor;
in vec2 Tex;
uniform sampler2D gTerrainTexture;
void main()
{
    vec4 TexColor = texture(gTerrainTexture, Tex);
    FragColor =  TexColor;
}
