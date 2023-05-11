#include "ProceduralTexture.h"

ProceduralTexture::ProceduralTexture()
{

}

ProceduralTexture::~ProceduralTexture()
{
    delete  m_texture;
}

void ProceduralTexture::LoadTile(std::string path)
{
    if (m_numTextureTiles == MAX_TEXTURE_TILES) {
        qDebug() << "Exceeded the maximum of texture tiles with " << QString::fromStdString(path);
        exit(0);
    }
    m_textureTiles[m_numTextureTiles].Image = QImage(QString::fromStdString(path)).mirrored();
    m_numTextureTiles++;
}

void ProceduralTexture::GenerateTexture(int TextureSize, float MinHeight, float MaxHeight,TerrainBase* pTerrain)
{
    if (m_numTextureTiles == 0) {
        qDebug() <<"No texture tiles loaded ";
        exit(0);
    }

    CalculateTextureRegions(MinHeight, MaxHeight);

    int BPP = 3;  // Số byte trên mỗi pixel
    int TextureBytes = TextureSize * TextureSize * BPP; //
    unsigned char* pTextureData = (unsigned char*)malloc(TextureBytes); // là con trỏ tới dữ liệu pixel của texture
    unsigned char* p = pTextureData;

    float HeightMapToTextureRatio = (float)pTerrain->GetSize() / (float)TextureSize;

    printf("Height map to texture ratio: %f\n", HeightMapToTextureRatio);

    for (int y = 0 ; y < TextureSize ; y++) {
        for (int x = 0 ; x < TextureSize ; x++) {

            float InterpolatedHeight = pTerrain->GetHeightInterpolated((float)y * HeightMapToTextureRatio,(float)x * HeightMapToTextureRatio); //  pTerrain->GetHeightInterpolated((float)x * HeightMapToTextureRatio,(float)y * HeightMapToTextureRatio)

            float Red = 0.0f;
            float Green = 0.0f;
            float Blue = 0.0f;

            float redv=0.0;
            float greenv=0.0;
            float bluev=0.0;
            for (int Tile = 0 ; Tile < m_numTextureTiles ; Tile++) {
                QColor Color = m_textureTiles[Tile].Image.pixelColor(x,y);//.GetColor(x, y);

                float BlendFactor = RegionPercent(Tile, InterpolatedHeight);
                redv  = BlendFactor * Color.red();
                greenv= BlendFactor * Color.green();
                bluev =  BlendFactor * Color.blue();

                Red   = Red+redv;
                Green =Green+ greenv;
                Blue  =Blue+ bluev;
            }

            if (Red > 255.0f || Green > 255.0f || Blue > 255.0f) {
                 qDebug() <<"y:" << y <<"x:"<< x<< "red:"<< Red << "green:"<<  Green<< "blue:" << Blue;
                exit(0);
            }

            p[0] = (unsigned char)Red;
            p[1] = (unsigned char)Green;
            p[2] = (unsigned char)Blue;

            p += 3;
        }
    }
    QImage image(pTextureData, TextureSize, TextureSize, QImage::Format_RGB888);
    image.save("heightMap.png");
    m_texture = new QOpenGLTexture(image);
    free(pTextureData);
    //
    m_texture->generateMipMaps(0);
    SetWrapTexture( QOpenGLTexture::DirectionS,  QOpenGLTexture::Repeat);
    SetWrapTexture( QOpenGLTexture::DirectionT,  QOpenGLTexture::Repeat);
    SetFilter(QOpenGLTexture::LinearMipMapLinear,QOpenGLTexture::Linear);
}
void ProceduralTexture::SetToShader(QOpenGLShaderProgram& shader,std::string uniformValue, int portNum)
{
    shader.setUniformValue(uniformValue.c_str(),GL_TEXTURE0 + portNum);
    m_texture->bind(portNum);
}

void ProceduralTexture::SetWrapTexture(QOpenGLTexture::CoordinateDirection direction, QOpenGLTexture::WrapMode wrapmode)
{
    m_texture->setWrapMode(direction, wrapmode);
}

void ProceduralTexture::SetFilter(QOpenGLTexture::Filter minificationF, QOpenGLTexture::Filter magnifitionF)
{

   m_texture->setMinificationFilter(minificationF);
   m_texture->setMagnificationFilter(magnifitionF);
}


void ProceduralTexture::CalculateTextureRegions(float MinHeight, float MaxHeight)
{
    float HeightRange = MaxHeight - MinHeight;

    float RangePerTile = HeightRange / m_numTextureTiles;
    float Remainder = HeightRange - RangePerTile * m_numTextureTiles;

    if (Remainder < 0.0f) {
        qDebug() << "Negative remainder "<< Remainder <<" (num tiles"<<m_numTextureTiles << " range per tile " << RangePerTile;
        exit(0);
    }

    float LastHeight = -1.0f;

    for (int i = 0 ; i < m_numTextureTiles ; i++) {
        m_textureTiles[i].HeightDesc.LowHeight = LastHeight + 1;
        LastHeight += RangePerTile;
        m_textureTiles[i].HeightDesc.OptimalHeight = LastHeight;
        m_textureTiles[i].HeightDesc.HighHeight = m_textureTiles[i].HeightDesc.OptimalHeight + RangePerTile;
        // m_textureTiles[i].HeightDesc.Print(); printf("\n");
    }
}

float ProceduralTexture::RegionPercent(int Tile, float Height)
{
    float Percent = 0.0f;
    if (Height < m_textureTiles[Tile].HeightDesc.LowHeight) {
        Percent = 0.0f;
    } else if (Height > m_textureTiles[Tile].HeightDesc.HighHeight) {
        Percent = 0.0f;
    } else if (Height < m_textureTiles[Tile].HeightDesc.OptimalHeight) {
        float Nom = (float)Height - (float)m_textureTiles[Tile].HeightDesc.LowHeight;
        float Denom = (float)m_textureTiles[Tile].HeightDesc.OptimalHeight - (float)m_textureTiles[Tile].HeightDesc.LowHeight;
        Percent = Nom / Denom;
    } else if (Height >= m_textureTiles[Tile].HeightDesc.OptimalHeight) {
        float Nom = (float)m_textureTiles[Tile].HeightDesc.HighHeight - (float)Height;
        float Denom = (float)m_textureTiles[Tile].HeightDesc.HighHeight - (float)m_textureTiles[Tile].HeightDesc.OptimalHeight;
        Percent = Nom / Denom;
    } else {
        qDebug() <<  "Shouldn't get here! tile" << Tile << " Height "<<  Height;
        exit(0);
    }
    if ((Percent < 0.0f) || (Percent > 1.0f)) {
        qDebug() << "Invalid percent "<<  Percent;
        exit(0);
    }
    return Percent;
}



