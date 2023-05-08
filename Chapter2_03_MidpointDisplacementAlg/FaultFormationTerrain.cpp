#include "FaultFormationTerrain.h"

FaultFormationTerrain::FaultFormationTerrain(QOpenGLFunctions_3_3_Core *m_glFuns,int TerrainSize, int Iterations, float MinHeight, float MaxHeight, float Filter)
{
    m_buffer = new Buffer(m_glFuns);
    m_terrainSize = TerrainSize;
    m_iterations = Iterations;
    m_minHeight = MinHeight;
    m_maxHeight = MaxHeight;
    m_filter  = Filter;

    double m_width = m_terrainSize;
    double m_depth = m_terrainSize;

    int NumQuads = (m_width - 1) * (m_depth - 1);
    Indices.resize(NumQuads * 6);
    int Index = 0;
    for (int z = 0; z < m_depth; z++) {
        for (int x = 0; x < m_width; x++) {
            Vertices.push_back(x);
            Vertices.push_back(0.0f);
            Vertices.push_back(z);
            //set index
            if( z< m_depth-1 && x < m_width-1){
                unsigned int IndexBottomLeft = z * m_width + x;
                unsigned int IndexTopLeft = (z + 1) * m_width + x;
                unsigned int IndexTopRight = (z + 1) * m_width + x + 1;
                unsigned int IndexBottomRight = z * m_width + x + 1;
                // Add top left triangle
                Indices[Index++] = IndexBottomLeft;
                Indices[Index++] = IndexTopLeft;
                Indices[Index++] = IndexTopRight;
                // Add bottom right triangle
                Indices[Index++] = IndexBottomLeft;
                Indices[Index++] = IndexTopRight;
                Indices[Index++] = IndexBottomRight;
            }

        }
    }
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
                    size_t index = GetIndex(z,x)+1;
                    float CurHeight = Vertices.at(index);//  m_heightMap.Get(x, z);
                    Vertices.at(index)= CurHeight + Height;
                }
            }
        }
    }
    // normalize: Chuyển chiều cao của các vertex nằm trong đoạn[m_minHeight, m_maxHeight]
    Normalize(m_minHeight, m_maxHeight);
    // Filter để làm mịn chiều cao
    ApplyFIRFilter(m_filter);

    m_buffer->FillVBO(VBOType::VertexBuffer, &Vertices[0],sizeof (Vertices[0])* Vertices.size(), FillType::Static_draw);
    m_buffer->FillVBO(VBOType::IndexBuffer, &Indices[0],sizeof (Indices[0])* Indices.size(), FillType::Static_draw);
    m_buffer->LinkBuffer(0,3, VBOType::VertexBuffer);

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

size_t FaultFormationTerrain::GetIndex(int Row,int Col)
{
    int m_cols = m_terrainSize;
    int m_rows = m_terrainSize;
    if (Col < 0 || Row <0) {
        qDebug() <<"negative";
        exit(0);
    }

    if (Col >= m_cols || Row >= m_rows) {
        qDebug() << "overflow " << Col << m_cols;
        exit(0);
    }
    size_t Index =(3* (m_cols-1) +3)* Row + 3* Col;
    return Index;
}

float FaultFormationTerrain::GetHeight(int row, int col)
{
    return  Vertices.at(GetIndex(row,col)+1);
}

void FaultFormationTerrain::Normalize(float minRange, float maxrange)
{
    float fMin, fMax;
    float fHeight;
    fMin= GetHeight(0,0);
    fMax= GetHeight(0,0);

    //find the min/max values of the height fTempBuffer
    float height=0.0;
    for (int z = 0; z < m_terrainSize; z++) {
        for (int x = 0; x < m_terrainSize; x++) {
            height =  GetHeight(z,x);
            if(height>fMax ) {
                fMax= height;
            }
            else if( height<fMin ){
                fMin= height;
            }
        }
    }
    //find the range of the altitude
    if( fMax<=fMin )
        return;

    fHeight= fMax-fMin;
    int index=0;
    //cách 1:scale the values to a range of 0-255 (because I like things that way)

    //    for (int z = 0; z < m_terrainSize; z++) {
    //        for (int x = 0; x < m_terrainSize; x++) {
    //            index= GetIndex(z,x)+1;
    //            Vertices.at(index)= ( ( Vertices.at(index)-fMin )/fHeight )*255.0f;
    //        }
    //    }
    // Cách 2:
    auto MinMaxDelta =fMax-fMin;
    auto MinMaxRange = maxrange - minRange;
    for (int z = 0; z < m_terrainSize; z++) {
        for (int x = 0; x < m_terrainSize; x++) {
            index= GetIndex(z,x)+1;
            Vertices.at(index) = (( Vertices.at(index) - fMin)/MinMaxDelta) * MinMaxRange + minRange;
        }
    }
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
      Vertices.at(GetIndex(z,x)+1) = NewVal;
      //m_heightMap.Set(x, z, NewVal);
      return NewVal;
}

void FaultFormationTerrain::Render(DrawType type, bool isWireFrame)
{
    m_buffer->Render(type, isWireFrame);
}
