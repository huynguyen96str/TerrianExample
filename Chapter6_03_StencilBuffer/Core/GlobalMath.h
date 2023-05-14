#ifndef GLOBALMATH_H
#define GLOBALMATH_H

#include <QObject>
#include <cmath>
#include <corecrt_math_defines.h>
#include <iostream>
#define TOLERANCE  0.000001
static bool IsEqualNumber(double x, double y) {
    return fabs(x - y) < TOLERANCE;
}
static double Radiance2Degrees(double radiance) {
    return radiance * 360 / (2 * M_PI);
}
static bool _xor(bool x, bool y) {
    return x ^ y;
}

enum class RELATIVE_POSITION {
    /// Vị trí điểm check nằm bên phía miền bên trái so với ranh giới giữa 2 điểm mút
    LEFT,
    /// Vị trí điểm check nằm phía bên phải của ranh giới
    RIGHT,
    /// Vị trí điểm check nằm trên đường thẳng nối 2 mút, nằm ngoài ở phía điểm A
    BEHIND,
    /// Vị trí điểm check nằm trên đường thẳng nối 2 mút, nằm ngoài ở phía điểm B
    BEYOND,
    /// Vị trí điểm check nằm giữa 2 điểm đầu mút
    BETWEEN,
    /// Vị trí điểm check nằm trùng với Điểm đầu mút A.
    ORIGIN,
    /// Vị trí điểm check nằm trùng với điểm cuối mút B.
    DESTINATION,
};
enum class RELATIVE_LINES
{
    /// Hai đường lines không đồng phẳng=> chéo nhau
    NONE_COPLANER,
    /// Hai đường thẳng cắt nhau
    INTERSECT,
    /// Hai đường thẳng song song với nhau
    PARALLEL,
    /// Hai đường thẳng trùng nhau
    COINCIDE,
};
enum class RELATIVE_PLANE_LINE {
    /// Đường thẳng và mặt phẳng song song với nhau
    PARALLEL,
    /// Đường thẳng nằm trong mặt phẳng
    IN2,
    /// Đường thẳng và mặt phẳng cắt nhau
    INTERSECT,
};
enum class RELATIVE_PLANES {
    /// Hai mặt phẳng cắt nhau
    INTERSECT,
    /// Hai mặt phẳng song song với nhau
    PARALLEL,
    /// Hai mặt phẳng trùng nhau
    COINCIDE,
};

#endif // GLOBALMATH_H
