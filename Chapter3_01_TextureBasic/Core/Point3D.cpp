#include "Point3D.h"
Point3D::Point3D() {
    _x = 0;
    _y = 0;
    _z = 0;
}

Point3D::Point3D(double _x, double _y, double _z){
    this->_x = _x;
    this->_y = _y;
    this->_z = _z;
}
Point3D  Point3D::BasisX = Point3D(1, 0, 0);
Point3D  Point3D::BasisY = Point3D(0, 1, 0);
Point3D  Point3D::BasisZ = Point3D(0, 0, 1);
Point3D  Point3D::Zero = Point3D(0, 0, 0);

double Point3D::X() const {
    return _x;
}
double Point3D::Y() const {
    return _y;
}
double Point3D::Z() const {
    return _z;
}

double Point3D::Area(const Point3D& _p1, const Point3D& _p2, const Point3D& _p3)
{
    float x_, y_, z_;
    Point3D v = _p2 - _p1;
    Point3D w = _p3 - _p1;
    x_ = v._y * w._z - v._z * w._y;
    y_ = v._z * w._x - v._x * w._z;
    z_ = v._x * w._y - v._y * w._x;
    return (x_ + y_ + z_) / 2;
}

bool Point3D::operator==(const Point3D& _source) const
{
    if (IsEqualNumber(_x, _source._x) && IsEqualNumber(_y, _source._y) && IsEqualNumber(_z, _source._z))
    {
        return true;
    }
    return false;
}

bool Point3D::operator!=(const Point3D& _source) const
{
    return !(*this == _source);
}

Point3D Point3D::operator+(const Point3D& _source) const
{
    double x_ = _x + _source._x;
    double y_ = _y + _source._y;
    double z_ = _z + _source._z;
    return Point3D(x_, y_, z_);
}

Point3D Point3D::operator-(const Point3D& _source) const
{
    double x_ = _x - _source._x;
    double y_ = _y - _source._y;
    double z_ = _z - _source._z;
    return Point3D(x_, y_, z_);
}
/*
 2D:
   if(A[X] < B[X] || A[X] == B[X] && A[Y] < B[Y]) return true;
 3D:
     if(A[X] < B[X] || A[X] == B[X] && A[Y] < B[Y] |
                    || A[X] == B[X] && A[Y] == B[Y] && A[Z] < B[Z]) return true;
*/

bool Point3D::operator<(const Point3D& _source) const
{
    if (_x < _source._x || _x == _source._x && _y < _source._y
            || _x == _source._x && _y == _source._y && _z < _source._z)
    {
        return true;
    }
    return false;
}

bool Point3D::operator>(const Point3D& _source) const
{
    if (_x > _source._x || _x == _source._x && _y > _source._y
            || _x == _source._x && _y == _source._y && _z > _source._z)
    {
        return true;
    }
    return false;
}

Point3D Point3D::operator*(double _value) const
{
    double x_ = _x * _value;
    double y_ = _y * _value;
    double z_ = _z * _value;
    return Point3D(x_, y_, z_);
}

Point3D Point3D::operator/(double _value) const
{
    double x_ = _x / _value;
    double y_ = _y / _value;
    double z_ = _z / _value;
    return Point3D(x_, y_, z_);
}

Point3D Point3D::Add(const Point3D& _source) const {
    return *this + _source;
}
double Point3D::AngleTo(const Point3D& _source) const {
    double dot = DotProduct(_source);
    double lengthu = GetLength();
    double lengthv = _source.GetLength();
    return acos(dot / (lengthu * lengthv));
}
Point3D Point3D::CrossProduct(const Point3D& _source) const {
    double x_ = _y * _source._z - _z * _source._y;
    double y_ = _z * _source._x - _x * _source._z;
    double z_ = _x * _source._y - _y * _source._x;
    return Point3D(x_, y_, z_);
}
double Point3D::DistanceTo(const Point3D& _source) const {
    Point3D vect = *this - _source;
    return vect.GetLength();
}
Point3D Point3D::Divide(const double& _value) const {
    return *this / _value;
}
double Point3D::DotProduct(const Point3D& _source) const {
    return _x * _source._x + _y * _source._y + _z * _source._z;
}
double Point3D::GetLength() const {
    return sqrt(_x * _x + _y * _y + _z * _z);
}
double Point3D::IsAlmostEqualTo(const Point3D& _source) const {
    return *this == _source;
}
bool Point3D::IsZeroLength() const {
    return IsEqualNumber(GetLength(), 0);
}
Point3D Point3D::Multiphe(const double& _value) const {
    return *this * _value;
}
Point3D Point3D::Negate() const {
    return *this * (-1);
}
Point3D Point3D::Normalize() const {
    double length = GetLength();
    return *this / length;
}
Point3D Point3D::Substract(Point3D _source) const {
    return *this - _source;
}
QString Point3D::ToString() const {
    return QString("{%0,%1,%2}").arg(_x).arg(_y).arg(_z);
}
/// <summary>
/// Kiểm tra bộ số có tỷ lệ không a/a' = b/b'= c/c' hay u = k.v
/// </summary>
/// <param name="_source"></param>
/// <returns></returns>
bool Point3D::IsCollinear(const Point3D& _source) const
{
    auto v1 = _x * _source._y - _y * _source._x;
    auto v2 = _y * _source._z - _z * _source._y;
    auto v3 = _x * _source._z - _z * _source._x;
    return IsEqualNumber(v1, 0) && IsEqualNumber(v2, 0) && IsEqualNumber(v3, 0);
}
RELATIVE_POSITION Point3D::Orientation(const Point3D& _pA, const Point3D& _pB, const Point3D& _pCheck)
{
    double area = Area(_pA, _pB, _pCheck);

    if (area > 0 && area < TOLERANCE)
        area = 0;

    if (area < 0 && area > TOLERANCE)
        area = 0;

    Point3D p1 = _pB - _pA;
    Point3D p2 = _pCheck - _pA;

    double p1x, p1y, p2x, p2y;

    p1x = p1._x;
    p1y = p1._y;
    p2x = p2._x;
    p2y = p2._y;

    if (area > 0.0)
        return RELATIVE_POSITION::LEFT;
    if (area < 0.0)
        return RELATIVE_POSITION::RIGHT;
    if ((p1x * p2x < 0.0) || (p1y * p2y < 0.0))
        return RELATIVE_POSITION::BEHIND;
    if (p1.GetLength() < p2.GetLength())
        return RELATIVE_POSITION::BEYOND;
    if (_pA == _pCheck)
        return RELATIVE_POSITION::ORIGIN;
    if (_pB == _pCheck)
        return RELATIVE_POSITION::DESTINATION;
    return RELATIVE_POSITION::BETWEEN;
}
/// <summary>
/// Tích 3 vô hướng liên quan đến 4 điểm có cùng thuộc 1 mặt phẳng hay không tương ứng với 3 vector có thuộc một mặt phẳng hay không.
/// </summary>
/// <param name="_vecA"></param>
/// <param name="_vecB"></param>
/// <param name="_vecC"></param>
/// <returns></returns>
double Point3D::ScalerTripleProduct(Point3D _vecA, Point3D _vecB, Point3D _vecC)
{
    return _vecA.DotProduct(_vecB.CrossProduct(_vecC));
}
/// <summary>
/// Kiểm tra tính đồng phẳng tại 4 điểm
/// </summary>
/// <param name="_p1"></param>
/// <param name="_p2"></param>
/// <param name="_p3"></param>
/// <param name="_p4"></param>
/// <returns></returns>
bool Point3D::Coplaner(const Point3D& _p1, const Point3D& _p2, const Point3D& _p3, const Point3D& _p4)
{
    auto AB = _p2 - _p1;
    auto AC = _p3 - _p1;
    auto AD = _p4 - _p1;
    double value = ScalerTripleProduct(AB, AC, AD);
    return IsEqualNumber(value, 0);
}

bool Point3D::Intersect(Point3D a, Point3D b, Point3D c, Point3D d)
{
    // if one of the end points of a segment is in between other segment endpoints we consider it as intersection.
    if (Orientation(a, b, c) == RELATIVE_POSITION::BETWEEN
            || Orientation(a, b, d) == RELATIVE_POSITION::BETWEEN
            || Orientation(c, d, a) == RELATIVE_POSITION::BETWEEN
            || Orientation(c, d, b) == RELATIVE_POSITION::BETWEEN)
    {
        return true;
    }

    return _xor(Orientation(a, b, c)== RELATIVE_POSITION::LEFT,
                Orientation(a, b, d)== RELATIVE_POSITION::LEFT) &&
            _xor(Orientation(c, d, a)== RELATIVE_POSITION::LEFT,
                 Orientation(c, d, b)== RELATIVE_POSITION::LEFT);
}

