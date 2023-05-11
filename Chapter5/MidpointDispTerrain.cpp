#include "MidpointDispTerrain.h"

MidpointDispTerrain::MidpointDispTerrain(QOpenGLFunctions_3_3_Core* m_glFuns, int TerrainSize, float Roughness, float MinHeight, float MaxHeight):TerrainBase(m_glFuns,TerrainSize,TerrainSize)
{
    m_terrainSize = TerrainSize;
    if (Roughness < 0.0f) {
        qDebug() << "Roughness must be positive";
        exit(0);
    }
    // Tinh toan lai chieu cao cua cac dinh
    CreateMidpointDisplacementF32(Roughness);
    // Normalize
    Normalize(MinHeight, MaxHeight);
    // Đóng gói buffer
    CreateBuffer();
}
int MidpointDispTerrain::CalcNextPowerOfTwo(int x)
{
    int ret = 1;

    if (x == 1) {
        return 2;
    }

    while (ret < x) {
        ret = ret * 2;
    }

    return ret;
}


float MidpointDispTerrain::RandomFloatRange(float Start, float End)
{
    if (End == Start) {
        printf("Invalid random range: (%f, %f)\n", Start, End);
        exit(0);
    }

    float Delta = End - Start;

    float RandomValue = RandomFloat() * Delta + Start;

    return RandomValue;
}
float MidpointDispTerrain::RandomFloat()
{
    float Max = RAND_MAX;
    return ((float)std::rand() / Max); // RANDOM()
}


void MidpointDispTerrain::CreateMidpointDisplacementF32(float Roughness)
{
    int RectSize = CalcNextPowerOfTwo(m_terrainSize);
    float CurHeight = (float)RectSize / 2.0f;
    float HeightReduce = pow(2.0f, -Roughness);
    while (RectSize > 0) {
        DiamondStep(RectSize, CurHeight);
        SquareStep(RectSize, CurHeight);
        RectSize /= 2;
        CurHeight *= HeightReduce;
    }
}

void MidpointDispTerrain::DiamondStep(int RectSize, float CurHeight)
{
    int HalfRectSize = RectSize / 2;
    for (int y = 0 ; y < m_terrainSize ; y += RectSize) {
        for (int x = 0 ; x < m_terrainSize ; x += RectSize) {
            int next_x = (x + RectSize) % m_terrainSize;
            int next_y = (y + RectSize) % m_terrainSize;

            if (next_x < x) {
                next_x = m_terrainSize - 1;
            }

            if (next_y < y) {
                next_y = m_terrainSize - 1;
            }

            float TopLeft     = GetHeight(y,x);//m_heightMap.Get(x, y);
            float TopRight    = GetHeight(y,next_x);//m_heightMap.Get(next_x, y);
            float BottomLeft  = GetHeight(next_y,x);//m_heightMap.Get(x, next_y);
            float BottomRight = GetHeight(next_y,next_x); //m_heightMap.Get(next_x, next_y);

            int mid_x = (x + HalfRectSize) % m_terrainSize;
            int mid_y = (y + HalfRectSize) % m_terrainSize;

            float RandValue = RandomFloatRange(CurHeight, -CurHeight);
            float MidPoint = (TopLeft + TopRight + BottomLeft + BottomRight) / 4.0f;

            SetHeight(mid_y,mid_x,  MidPoint + RandValue);// m_heightMap.Set(mid_x, mid_y, MidPoint + RandValue);
        }
    }
}

void MidpointDispTerrain::SquareStep(int RectSize, float CurHeight)
{
    /*                ----------------------------------
                      |                                |
                      |           PrevYCenter          |
                      |                                |
                      |                                |
                      |                                |
    ------------------CurTopLeft..CurTopMid..CurTopRight
                      |                                |
                      |                                |
       CurPrevXCenter CurLeftMid   CurCenter           |
                      |                                |
                      |                                |
                      CurBotLeft------------------------

       CurTopMid = avg(PrevYCenter, CurTopLeft, CurTopRight, CurCenter)
       CurLeftMid = avg(CurPrevXCenterm CurTopleft, CurBotLeft, CurCenter)
    */

    int HalfRectSize = RectSize / 2;

    for (int y = 0 ; y < m_terrainSize ; y += RectSize) {
        for (int x = 0 ; x < m_terrainSize ; x += RectSize) {
            int next_x = (x + RectSize) % m_terrainSize;
            int next_y = (y + RectSize) % m_terrainSize;

            if (next_x < x) {
                next_x = m_terrainSize - 1;
            }

            if (next_y < y) {
                next_y = m_terrainSize - 1;
            }

            int mid_x = (x + HalfRectSize) % m_terrainSize;
            int mid_y = (y + HalfRectSize) % m_terrainSize;

            int prev_mid_x = (x - HalfRectSize + m_terrainSize) % m_terrainSize;
            int prev_mid_y = (y - HalfRectSize + m_terrainSize) % m_terrainSize;

            float CurTopLeft  =GetHeight(y,x) ;//m_heightMap.Get(x, y);
            float CurTopRight =GetHeight(y,next_x) ;//m_heightMap.Get(next_x, y);
            float CurCenter   =GetHeight(mid_y,mid_x) ;//m_heightMap.Get(mid_x, mid_y);
            float PrevYCenter =GetHeight(prev_mid_y,mid_x) ;//m_heightMap.Get(mid_x, prev_mid_y);
            float CurBotLeft  =GetHeight(next_y,x) ;//m_heightMap.Get(x, next_y);
            float PrevXCenter =GetHeight(mid_y,prev_mid_x) ;//m_heightMap.Get(prev_mid_x, mid_y);

            float CurLeftMid = (CurTopLeft + CurCenter + CurBotLeft + PrevXCenter) / 4.0f + RandomFloatRange(-CurHeight, CurHeight);
            float CurTopMid  = (CurTopLeft + CurCenter + CurTopRight + PrevYCenter) / 4.0f + RandomFloatRange(-CurHeight, CurHeight);

            SetHeight(y,mid_x,CurTopMid);// m_heightMap.Set(mid_x, y, CurTopMid);
            SetHeight(mid_y,x,CurLeftMid);// m_heightMap.Set(x, mid_y, CurLeftMid);
        }
    }
}
