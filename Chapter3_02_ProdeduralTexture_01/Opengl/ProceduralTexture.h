#ifndef PROCEDURALTEXTURE_H
#define PROCEDURALTEXTURE_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include "Opengl/GlobalOpengl.h"
#include <QOpenGLTexture>
#include <QDebug>
#include "TerrainBase.h"


struct TextureRegions
{
    int LowHeight; //lowest possible height (0%)  (0-255)
    int OptimalHeight; //optimal height (100%)    (0-255)
    int HighHeight; //highest possible height (0%)(0,255)
};
struct Color3f{
    float r;
    float g;
    float b;
};

struct TextureTile {
    QImage Image;
    TextureRegions HeightDesc;
};
class ProceduralTexture
{
public:
    ProceduralTexture();
    ~ProceduralTexture();
    void LoadTile(std::string path);
    void SetToShader(QOpenGLShaderProgram& shader, std::string uniformValue, int portNum=0);
    void GenerateTexture(int TextureSize, float MinHeight, float MaxHeight, TerrainBase* terrain);
private:
    void CalculateTextureRegions(float MinHeight, float MaxHeight);
    float RegionPercent(int Tile, float Height);
    #define MAX_TEXTURE_TILES 4
    TextureTile m_textureTiles[MAX_TEXTURE_TILES] = {};
    int m_numTextureTiles = 0;
     QOpenGLTexture * m_texture;
     void SetWrapTexture(QOpenGLTexture::CoordinateDirection direction, QOpenGLTexture::WrapMode wrapmode);
    void SetFilter(QOpenGLTexture::Filter minificationF, QOpenGLTexture::Filter magnifitionF);
};

#endif // PROCEDURALTEXTURE_H
