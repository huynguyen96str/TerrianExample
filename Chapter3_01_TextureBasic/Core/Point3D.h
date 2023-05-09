#ifndef POINT3D_H
#define POINT3D_H

#include "Core/GlobalMath.h"

class Point3D
{
public:
    Point3D(double _x, double _y, double _z);
    Point3D();
    double X() const;
    double Y() const;
    double Z() const;
    static Point3D BasisX;
    static Point3D BasisY;
    static Point3D BasisZ;
    static Point3D Zero;
    static double Area(const Point3D& _p1, const Point3D& _p2, const Point3D& _p3);
    static RELATIVE_POSITION Orientation(const Point3D& _pA, const Point3D& _pB, const Point3D& _pCheck);
    static double ScalerTripleProduct(Point3D _vecA, Point3D _vecB, Point3D _vecC);
    static bool Coplaner(const Point3D& _p1, const Point3D& _p2, const Point3D& _p3, const Point3D& _p4);
    static bool Intersect(Point3D a, Point3D b, Point3D c, Point3D d);
    // operator
    bool operator==(const Point3D& _source) const;
    bool operator!=(const Point3D& _source) const;
    Point3D operator+(const Point3D& _source) const;
    Point3D operator-(const Point3D& _source) const;
    bool operator<(const Point3D& _source) const;
    bool operator>(const Point3D& _source) const;
    Point3D operator*(double _value) const;
    Point3D operator/(double _value) const;
    // method
    Point3D Add(const Point3D& source) const;
    double AngleTo(const Point3D& _source) const;
    Point3D CrossProduct(const Point3D& _source) const;
    double DistanceTo(const Point3D& _source) const;
    Point3D Divide(const double& _value) const;
    double DotProduct(const Point3D& _source) const;
    double GetLength() const;
    double IsAlmostEqualTo(const Point3D& source) const;
    bool IsZeroLength() const;
    Point3D Multiphe(const double& _value) const;
    Point3D Negate() const;
    Point3D Normalize() const;
    Point3D Substract(Point3D _source) const;
    QString ToString() const;
    bool IsCollinear(const Point3D& _source) const;
private:
    double _x, _y, _z;
};

#endif // POINT3D_H
