#include "FaultFormationTerrain.h"
///  m_camera = Camera(QVector3D(0.0f, 10.0f, -50.0f));
///  int size = 256;
///  float minHeight = 0.0f;
///  float maxHeight = 50.0f;
///  float filter = 0.2f;
///  int interations =50;// 100;
///  m_terrain= new FaultFormationTerrain(CurrentContex,size,interations,minHeight,maxHeight,filter);
FaultFormationTerrain::FaultFormationTerrain(QOpenGLFunctions_3_3_Core *m_glFuns,int TerrainSize, int Iterations, float MinHeight, float MaxHeight, float Filter):TerrainBase(m_glFuns, TerrainSize,TerrainSize)
{
    m_terrainSize  = TerrainSize;
    // Iterations
    float DeltaHeight = MaxHeight - MinHeight;
    for (int CurIter = 0 ; CurIter < Iterations ; CurIter++) {
        float IterationRatio = ((float)CurIter / (float)Iterations);
        float Height = MaxHeight - IterationRatio * DeltaHeight;

        Point3D p1, p2;
        GenRandomTerrainPoints(p1, p2);

        int DirX = p2.X() - p1.X();
        int DirZ = p2.Z() - p1.Z();

        for (int z = 0 ; z < m_terrainSize ; z++) {
            for (int x = 0 ; x < m_terrainSize ; x++) {
                int DirX_in = x - p1.X();
                int DirZ_in = z - p1.Z();
                int CrossProduct = DirX_in * DirZ - DirX * DirZ_in;
                if (CrossProduct > 0) {
                    float CurHeight =GetHeight(z,x);
                    SetHeight(z,x,CurHeight + Height);
                }
            }
        }
    }
    // normalize: Chuyển chiều cao của các vertex nằm trong đoạn[m_minHeight, m_maxHeight]
    Normalize(MinHeight, MaxHeight);
    // Filter để làm mịn chiều cao
    ApplyFIRFilter(Filter);
    // Đóng gói tạo buffer
    CreateBuffer();
}

void FaultFormationTerrain::GenRandomTerrainPoints(Point3D &p1, Point3D &p2)
{
    double x = rand() % m_terrainSize;
    double z = rand() % m_terrainSize;
    p1= Point3D(x,0.0,z);
    int Counter = 0;

    double x2=0.0f;
    double z2=0.0f;
    do {
        x2 = rand() % m_terrainSize;
        z2 = rand() % m_terrainSize;
        p2 = Point3D(x2,0.0f, z2);
        if (Counter++ == 1000) {
            qDebug()<<  "Endless loop detected in";
        }
    } while (p1.IsAlmostEqualTo(p2));
}

void FaultFormationTerrain::ApplyFIRFilter(float Filter)
{
    // left to right
    for (int z = 0 ; z < m_terrainSize ; z++) {
        float PrevVal =GetHeight(z,0);// m_heightMap.Get(0, z);
        for (int x = 1 ; x < m_terrainSize ; x++) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }

    // right to left
    for (int z = 0 ; z < m_terrainSize ; z++) {
        float PrevVal =GetHeight(z,m_terrainSize - 1);// m_heightMap.Get(m_terrainSize - 1, z);
        for (int x = m_terrainSize - 2 ; x >= 0 ; x--) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }

    // bottom to top
    for (int x = 0 ; x < m_terrainSize ; x++) {
        float PrevVal = GetHeight(0,x);//m_heightMap.Get(x, 0);
        for (int z = 1 ; z < m_terrainSize ; z++) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }

    // top to bottom
    for (int x = 0 ; x < m_terrainSize ; x++) {
        float PrevVal =GetHeight( m_terrainSize - 1,x);// m_heightMap.Get(x, m_terrainSize - 1);
        for (int z = m_terrainSize - 2 ; z >= 0 ; z--) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }
}

float FaultFormationTerrain::FIRFilterSinglePoint(int x, int z, float PrevVal, float Filter)
{
    float CurVal = GetHeight(z,x);// m_heightMap.Get(x, z);
    float NewVal = Filter * PrevVal + (1 - Filter) * CurVal;
    SetHeight(z,x,NewVal);
    return NewVal;
}

