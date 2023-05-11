#ifndef GEOMIPGRID_H
#define GEOMIPGRID_H
#include <QOpenGLFunctions_3_3_Core>
#include "Opengl/GlobalOpengl.h"
#include "Opengl/Buffer.h"
#include <QDebug>
#include <TerrainBase.h>
class GeomipGrid {
 public:
    GeomipGrid();

    ~GeomipGrid();

    void CreateGeomipGrid(int Width, int Depth, int PatchSize, const TerrainBase* pTerrain);

    void Destroy();

    void Render();

 private:

//    struct Vertex {
//        Vector3f Pos;
//        Vector2f Tex;
//        Vector3f Normal = Vector3f(0.0f, 0.0f, 0.0f);

//        void InitVertex(const BaseTerrain* pTerrain, int x, int z);
//    };

  void CreateGLState();

    void PopulateBuffers(const TerrainBase* pTerrain);
//    void InitVertices(const BaseTerrain* pTerrain, std::vector<Vertex>& Vertices);
//    void InitIndices(std::vector<uint>& Indices);
//    void CalcNormals(std::vector<Vertex>& Vertices, std::vector<uint>& Indices);

    int AddTriangle(int Index,std::vector<int>& Indices, int v1, int v2, int v3);

    int m_width = 0;
    int m_depth = 0;
    int m_patchSize = 0;

};


#endif // GEOMIPGRID_H
