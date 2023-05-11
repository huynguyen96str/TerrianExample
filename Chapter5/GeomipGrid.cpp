#include "GeomipGrid.h"

GeomipGrid::GeomipGrid()
{

}

void GeomipGrid::CreateGeomipGrid(int Width, int Depth, int PatchSize, const TerrainBase *pTerrain)
{
    if ((Width - 1) % (PatchSize - 1) != 0) {
        int RecommendedWidth = ((Width - 1 + PatchSize - 1) / (PatchSize - 1)) * (PatchSize - 1) + 1;
        printf("Width minus 1 (%d) must be divisible by PatchSize minus 1 (%d)\n", Width, PatchSize);
        printf("Try using Width = %d\n", RecommendedWidth);
        exit(0);
    }

    if ((Depth - 1) % (PatchSize - 1) != 0) {
        int RecommendedDepth = ((Depth - 1 + PatchSize - 1) / (PatchSize - 1)) * (PatchSize - 1) + 1;
        printf("Depth minus 1 (%d) must be divisible by PatchSize minus 1 (%d)\n", Depth, PatchSize);
        printf("Try using Width = %d\n", RecommendedDepth);
        exit(0);
    }

    if (PatchSize < 3) {
        printf("The minimum patch size is 3 (%d)\n", PatchSize);
        exit(0);
    }

    if (PatchSize % 2 == 0) {
        printf("Patch size must be an odd number (%d)\n", PatchSize);
        exit(0);
    }

    m_width = Width;
    m_depth = Depth;
    m_patchSize = PatchSize;

    CreateGLState();

    PopulateBuffers(pTerrain);

}

void GeomipGrid::CreateGLState()
{

}

void GeomipGrid::PopulateBuffers(const TerrainBase *pTerrain)
{

}
