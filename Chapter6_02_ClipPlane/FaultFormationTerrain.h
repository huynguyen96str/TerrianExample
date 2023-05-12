#ifndef FAULTFORMATIONTERRAIN_H
#define FAULTFORMATIONTERRAIN_H

#include <QOpenGLFunctions_3_3_Core>
#include "Opengl/GlobalOpengl.h"
#include "Opengl/Buffer.h"
#include "Core/Point3D.h"
#include "TerrainBase.h"

class FaultFormationTerrain:public TerrainBase
{
public:
    FaultFormationTerrain(QOpenGLFunctions_3_3_Core* m_glFuns,int TerrainSize, int Iterations, float MinHeight, float MaxHeight, float Filter);
private:
    int m_terrainSize;
    void GenRandomTerrainPoints(Point3D& p1, Point3D& p2);
    void ApplyFIRFilter(float Filter);
    float FIRFilterSinglePoint(int x, int z, float PrevVal, float Filter);
};

#endif // FAULTFORMATIONTERRAIN_H
