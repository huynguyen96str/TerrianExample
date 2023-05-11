#ifndef MIDPOINTDISPTERRAIN_H
#define MIDPOINTDISPTERRAIN_H


#include <QOpenGLFunctions_3_3_Core>
#include "Opengl/GlobalOpengl.h"
#include "Opengl/Buffer.h"
#include "TerrainBase.h"

class MidpointDispTerrain:public TerrainBase
{
public:
    MidpointDispTerrain(QOpenGLFunctions_3_3_Core* m_glFuns, int TerrainSize, float Roughness, float MinHeight, float MaxHeight);
private:
    int m_terrainSize;
    void CreateMidpointDisplacementF32(float Roughness);
    void DiamondStep(int RectSize, float CurHeight);
    void SquareStep(int RectSize, float CurHeight);
    int CalcNextPowerOfTwo(int x);
    float RandomFloatRange(float Start, float End);
    float RandomFloat();
};

#endif // MIDPOINTDISPTERRAIN_H
