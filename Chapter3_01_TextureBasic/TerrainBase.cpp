#include "TerrainBase.h"

TerrainBase::TerrainBase(QOpenGLFunctions_3_3_Core *glFuns, int depth, int width)
{
    m_depth= depth;
    m_width = width;
    m_glFuns= glFuns;
    InitVerticesAndIndices();
}

TerrainBase::TerrainBase(QOpenGLFunctions_3_3_Core *glFuns)
{
    m_glFuns= glFuns;
}

void TerrainBase::SetSizeTerrian(int depthcount, int widthcount)
{
    m_depth = depthcount;
    m_width = widthcount;
    InitVerticesAndIndices();
}

TerrainBase::~TerrainBase()
{
    Unload();
}

void TerrainBase::Unload()
{
    if(m_buffer!=nullptr) delete m_buffer;
}

void TerrainBase::Render(DrawType type, bool isWireFrame)
{
    if(m_buffer!=nullptr) m_buffer->Render(type, isWireFrame);
}

size_t TerrainBase::GetIndex(int rowIndex, int colIndex)
{
    int m_rows = m_depth;
    int m_cols = m_width;

    if (colIndex < 0 || rowIndex <0) {
        qDebug() <<"negative";
        exit(0);
    }

    if (colIndex >= m_cols || rowIndex >= m_rows) {
        qDebug() << "overflow " << colIndex << m_cols;
        exit(0);
    }
    size_t Index =(3* (m_cols-1) +3)* rowIndex + 3* colIndex;
    return Index;
}

float TerrainBase::GetHeight(int rowIndex, int colIndex)
{
    return  m_vertices.at(GetIndex(rowIndex,colIndex)+1);
}

void TerrainBase::SetHeight(int rowIndex, int colIndex, float value)
{
    m_vertices.at(GetIndex(rowIndex,colIndex) +1) = value;
}

void TerrainBase::InitVerticesAndIndices()
{
    // Thiết lập vertices và indexces
    int NumQuads = (m_width - 1) * (m_depth - 1);
    m_indices.resize(NumQuads * 6);
    int Index = 0;
    for (int z = 0; z < m_depth; z++) {
        for (int x = 0; x < m_width; x++) {
            m_vertices.push_back(x);
            m_vertices.push_back(0);
            m_vertices.push_back(z);
            //set index
            if( z< m_depth-1 && x < m_width-1){
                unsigned int IndexBottomLeft = z * m_width + x;
                unsigned int IndexTopLeft = (z + 1) * m_width + x;
                unsigned int IndexTopRight = (z + 1) * m_width + x + 1;
                unsigned int IndexBottomRight = z * m_width + x + 1;
                // Add top left triangle
                m_indices[Index++] = IndexBottomLeft;
                m_indices[Index++] = IndexTopLeft;
                m_indices[Index++] = IndexTopRight;
                // Add bottom right triangle
                m_indices[Index++] = IndexBottomLeft;
                m_indices[Index++] = IndexTopRight;
                m_indices[Index++] = IndexBottomRight;
            }
        }
    }
}

void TerrainBase::CreateBuffer()
{
    m_buffer = new Buffer(m_glFuns);
    m_buffer->FillVBO(VBOType::VertexBuffer, &m_vertices[0],sizeof (m_vertices[0])* m_vertices.size(), FillType::Static_draw);
    m_buffer->FillVBO(VBOType::IndexBuffer, &m_indices[0],sizeof (m_indices[0])* m_indices.size(), FillType::Static_draw);
    m_buffer->LinkBuffer(0,3, VBOType::VertexBuffer);
}
/// scale the values to a range of 0-255 (because I like things that way)
void TerrainBase::Normalize()
{
    float fMin, fMax;
    float fHeight;
    fMin= GetHeight(0,0);
    fMax= GetHeight(0,0);

    //find the min/max values of the height fTempBuffer
    float height=0.0;
    for (int z = 0; z < m_depth; z++) {
        for (int x = 0; x < m_width; x++) {
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
    for (int z = 0; z < m_depth; z++) {
        for (int x = 0; x < m_width; x++) {
            index= GetIndex(z,x)+1;
            m_vertices.at(index)= ( ( m_vertices.at(index)-fMin )/fHeight )*255.0f;
        }
    }
}
/// scale the values to a range of minRange-maxrange
void TerrainBase::Normalize(float minRange, float maxrange)
{
    float fMin, fMax;
    float fHeight;
    fMin= GetHeight(0,0);
    fMax= GetHeight(0,0);

    //find the min/max values of the height fTempBuffer
    float height=0.0;
    for (int z = 0; z < m_depth; z++) {
        for (int x = 0; x < m_width; x++) {
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

    auto MinMaxDelta =fMax-fMin;
    auto MinMaxRange = maxrange - minRange;
    for (int z = 0; z < m_depth; z++) {
        for (int x = 0; x < m_width; x++) {
            index= GetIndex(z,x)+1;
            m_vertices.at(index) = (( m_vertices.at(index) - fMin)/MinMaxDelta) * MinMaxRange + minRange;
        }
    }
}
