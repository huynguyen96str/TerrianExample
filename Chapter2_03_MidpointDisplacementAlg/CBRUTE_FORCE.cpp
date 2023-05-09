#include "CBRUTE_FORCE.h"

CBRUTE_FORCE::CBRUTE_FORCE(QOpenGLFunctions_3_3_Core* m_glFuns ,int count)
{
    m_buffer = new Buffer(m_glFuns);
    m_isLoadHeightMaps = false;
    m_depth= count;
    m_width= count;
    initial();
}

CBRUTE_FORCE::CBRUTE_FORCE(QOpenGLFunctions_3_3_Core *m_glFuns, QString fileHeightmaps)
{
    m_buffer = new Buffer(m_glFuns);
    m_isLoadHeightMaps= true;
    QByteArray ba = fileHeightmaps.toLocal8Bit();
    m_cterrain.LoadHeightMap( ba.data(), 128 );
    m_cterrain.SetHeightScale( 0.25f );
    int depth = m_cterrain.GetSegment();
    m_depth= depth;
    m_width = depth;
    initial();
}

void CBRUTE_FORCE::initial()
{
    std::vector<float> Vertices;
    std::vector<unsigned int> Indices;
    int NumQuads = (m_width - 1) * (m_depth - 1);
    Indices.resize(NumQuads * 6);
    int Index = 0;
    for (int z = 0; z < m_depth; z++) {
        for (int x = 0; x < m_width; x++) {
            Vertices.push_back(x);
            if(m_isLoadHeightMaps){
                Vertices.push_back(m_cterrain.GetScaledHeightAtPoint( x, z ));
                // Màu 3 kênh đều như nhau: ucColor= m_cterrain.GetTrueHeightAtPoint( x, z )
            }else{
                Vertices.push_back(0);
            }
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
    m_buffer->FillVBO(VBOType::VertexBuffer, &Vertices[0],sizeof (Vertices[0])* Vertices.size(), FillType::Static_draw);
    m_buffer->FillVBO(VBOType::IndexBuffer, &Indices[0],sizeof (Indices[0])* Indices.size(), FillType::Static_draw);
    m_buffer->LinkBuffer(0,3, VBOType::VertexBuffer);
}


void CBRUTE_FORCE::Render(DrawType type, bool isWireFrame)
{
    m_buffer->Render(type, isWireFrame);
}
