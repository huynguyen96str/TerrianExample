#ifndef TERRAINBASE_H
#define TERRAINBASE_H
#include <QOpenGLFunctions_3_3_Core>
#include "Opengl/GlobalOpengl.h"
#include "Opengl/Buffer.h"
#include <QDebug>
class TerrainBase
{
public:
    TerrainBase(QOpenGLFunctions_3_3_Core* glFuns, int depth, int width);
    TerrainBase(QOpenGLFunctions_3_3_Core* glFuns);
    void SetSizeTerrian(int depthcount, int widthcount);
    ~TerrainBase();
    void Unload();
    void Render(DrawType type, bool isWireFrame);
    void SetTextureScale(int value);
private:
    Buffer* m_buffer=nullptr;
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
      std::vector<float> m_textures;
    int m_depth;
    int m_width;
    QOpenGLFunctions_3_3_Core* m_glFuns;
    void InitVerticesAndIndices();
    int m_textureScale =1;
protected:
    void CreateBuffer();
    void Normalize();
    void Normalize(float minRange, float maxrange);
    size_t GetIndex(int rowIndex, int colIndex);
    float GetHeight(int rowIndex, int colIndex);
    void SetHeight(int rowIndex, int colIndex, float value);
};

#endif // TERRAINBASE_H
