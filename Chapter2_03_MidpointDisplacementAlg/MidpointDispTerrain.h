#ifndef MIDPOINTDISPTERRAIN_H
#define MIDPOINTDISPTERRAIN_H

#include "CTERRAIN.h"
#include <QOpenGLFunctions_3_3_Core>
#include "Opengl/GlobalOpengl.h"
#include "Opengl/Buffer.h"
#include "Core/Point3D.h"

class MidpointDispTerrain
{
public:
    MidpointDispTerrain(QOpenGLFunctions_3_3_Core* m_glFuns, int TerrainSize, float Roughness, float MinHeight, float MaxHeight);
private:
    Buffer* m_buffer;
    int m_terrainSize;
    float m_roughness;
    float m_minHeight;
    float m_maxHeight;
    void CreateMidpointDisplacementF32(float Roughness);
    void DiamondStep(int RectSize, float CurHeight);
    void SquareStep(int RectSize, float CurHeight);
    int CalcNextPowerOfTwo(int x);
    float RandomFloatRange(float Start, float End);
    float RandomFloat();
    void Normalize(float minRange, float maxrange);
    std::vector<float> Vertices;
    std::vector<unsigned int> Indices;
    size_t GetIndex(int x, int z);
    float GetHeight(int row, int col);
public:
    void Render(DrawType type, bool isWireFrame);
};

#endif // MIDPOINTDISPTERRAIN_H
