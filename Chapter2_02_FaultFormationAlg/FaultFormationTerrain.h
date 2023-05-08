#ifndef FAULTFORMATIONTERRAIN_H
#define FAULTFORMATIONTERRAIN_H

#include "CTERRAIN.h"
#include <QOpenGLFunctions_3_3_Core>
#include "Opengl/GlobalOpengl.h"
#include "Opengl/Buffer.h"
#include "Core/Point3D.h"
class FaultFormationTerrain
{
public:
    FaultFormationTerrain(QOpenGLFunctions_3_3_Core* m_glFuns,int TerrainSize, int Iterations, float MinHeight, float MaxHeight, float Filter);
private:
    Buffer* m_buffer;
    int m_terrainSize;
    int m_iterations;
    float m_minHeight;
    float m_maxHeight;
    float m_filter;
    void GenRandomTerrainPoints(Point3D& p1, Point3D& p2);
    size_t GetIndex(int x, int z);
    float GetHeight(int row, int col);
    void Normalize(float minRange, float maxrange);
    std::vector<float> Vertices;
    std::vector<unsigned int> Indices;
    void ApplyFIRFilter(float Filter);
    float FIRFilterSinglePoint(int x, int z, float PrevVal, float Filter);
public:
    void Render(DrawType type, bool isWireFrame);
};

#endif // FAULTFORMATIONTERRAIN_H
