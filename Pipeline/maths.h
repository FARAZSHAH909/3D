#pragma once
#define _USE_MATH_DEFINES   // so we can use pi and other maths functions like e or sqrt

#include <cmath>
#include <algorithm>
#include <iostream>
#include "GamesEngineeringBase.h"
using namespace std;

// Use a float PI to avoid double->float implicit conversions
constexpr float PI_F = 3.14159265358979323846f;

// Macro - square - useful for squaring functions
#define SQ(x) ((x) * (x))

// Lerp Template
template<typename T>
static T lerp(const T a, const T b, float t)
{
    return a * (1.0f - t) + (b * t);
}

///////////////////////////////////////////////////////////////////////////////////////////////

class Vec3
{
public:
    union
    {
        float v[3];
        struct { float x, y, z; };  // v[0] is x, v[1] is y, v[2] is z
    };

    // Default Constructor
    Vec3() : x(0), y(0), z(0) {}

    // Constructor with parameters
    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    Vec3 operator+(const Vec3& pVec) const { return Vec3(x + pVec.x, y + pVec.y, z + pVec.z); }
    Vec3 operator-(const Vec3& pVec) const { return Vec3(x - pVec.x, y - pVec.y, z - pVec.z); }
    Vec3 operator*(const Vec3& pVec) const { return Vec3(x * pVec.x, y * pVec.y, z * pVec.z); }
    Vec3 operator/(const Vec3& pVec) const { return Vec3(x / pVec.x, y / pVec.y, z / pVec.z); }

    Vec3& operator+=(const Vec3& pVec) { x += pVec.x; y += pVec.y; z += pVec.z; return *this; }
    Vec3& operator-=(const Vec3& pVec) { x -= pVec.x; y -= pVec.y; z -= pVec.z; return *this; }
    Vec3& operator*=(const Vec3& pVec) { x *= pVec.x; y *= pVec.y; z *= pVec.z; return *this; }
    Vec3& operator/=(const Vec3& pVec) { x /= pVec.x; y /= pVec.y; z /= pVec.z; return *this; }

    Vec3 operator*(const float val) const { return Vec3(x * val, y * val, z * val); }
    Vec3 operator/(const float val) const { return Vec3(x / val, y / val, z / val); }
    Vec3 operator-() const { return Vec3(-x, -y, -z); }

    float length() const { return sqrtf(x * x + y * y + z * z); }
    float lengthSquared() const { return x * x + y * y + z * z; }

    Vec3 normalize() const { float l = length(); if (l == 0.0f) return Vec3(0,0,0); return Vec3(x / l, y / l, z / l); }
    float normalize_GetLength() { float l = length(); if (l == 0.0f) return 0.0f; float inv = 1.0f / l; x *= inv; y *= inv; z *= inv; return l; }

    float Dot(const Vec3& pVec) const { return x * pVec.x + y * pVec.y + z * pVec.z; }
    Vec3 Cross(const Vec3& pVec) const { return Vec3(y * pVec.z - z * pVec.y, z * pVec.x - x * pVec.z, x * pVec.y - y * pVec.x); }

    static Vec3 Max(const Vec3& pVec1, const Vec3& pVec2) { return Vec3(max(pVec1.x, pVec2.x), max(pVec1.y, pVec2.y), max(pVec1.z, pVec2.z)); }
    static Vec3 Min(const Vec3& pVec1, const Vec3& pVec2) { return Vec3(min(pVec1.x, pVec2.x), min(pVec1.y, pVec2.y), min(pVec1.z, pVec2.z)); }

    float Max() const { return max(x, max(y, z)); }
    float Min() const { return min(x, min(y, z)); }

    void Print() const { cout << "(" << x << ", " << y << ", " << z << ")" << std::endl; }
};

// Outside Vector helpers
inline float Dot(const Vec3& a, const Vec3& b) { return a.Dot(b); }
inline Vec3 Cross(const Vec3& a, const Vec3& b) { return a.Cross(b); }

////////////////////////////////////////////////////////////////////////////////////////////////////////

class Vec4
{
public:
    union
    {
        float v[4];
        struct { float x, y, z, w; };
    };

    Vec4() : x(0), y(0), z(0), w(0) {}
    Vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
    Vec4(const Vec3& pVec) : x(pVec.x), y(pVec.y), z(pVec.z), w(1.0f) {}

    Vec4 operator+(const Vec4& p) const { return Vec4(x + p.x, y + p.y, z + p.z, w + p.w); }
    Vec4 operator-(const Vec4& p) const { return Vec4(x - p.x, y - p.y, z - p.z, w - p.w); }
    Vec4 operator*(const Vec4& p) const { return Vec4(x * p.x, y * p.y, z * p.z, w * p.w); }
    Vec4 operator/(const Vec4& p) const { return Vec4(x / p.x, y / p.y, z / p.z, w / p.w); }

    Vec4& operator+=(const Vec4& p) { x += p.x; y += p.y; z += p.z; w += p.w; return *this; }
    Vec4& operator-=(const Vec4& p) { x -= p.x; y -= p.y; z -= p.z; w -= p.w; return *this; }
    Vec4& operator*=(const Vec4& p) { x *= p.x; y *= p.y; z *= p.z; w *= p.w; return *this; }
    Vec4& operator/=(const Vec4& p) { x /= p.x; y /= p.y; z /= p.z; w /= p.w; return *this; }

    Vec4 operator*(const float val) const { return Vec4(x * val, y * val, z * val, w * val); }
    Vec4 operator/(const float val) const { return Vec4(x / val, y / val, z / val, w / val); }
    Vec4 operator-() const { return Vec4(-x, -y, -z, -w); }

    static Vec4 Max(const Vec4& a, const Vec4& b) { return Vec4(max(a.x,b.x), max(a.y,b.y), max(a.z,b.z), max(a.w,b.w)); }
    static Vec4 Min(const Vec4& a, const Vec4& b) { return Vec4(min(a.x,b.x), min(a.y,b.y), min(a.z,b.z), min(a.w,b.w)); }

    float Max() const { return max(x, max(y, max(z, w))); }
    float Min() const { return min(x, min(y, min(z, w))); }

    void Print() const { cout << "(" << x << ", " << y << ", " << z << ", " << w << ")" << std::endl; }

    void divisionByW() { if (w != 0.0f) { float inv = 1.0f / w; x *= inv; y *= inv; z *= inv; w = 1.0f; } }

    Vec4 vertexToScreenSpace(int width, int height) const
    {
        return Vec4(((1.0f + x) / 2.0f) * width, ((1.0f - y) / 2.0f) * height, z, 1.0f);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////

class Matrix    // row major, not column major
{
public:
    union { float a[4][4]; float m[16]; };

    Matrix()
    {
        for (int i = 0; i < 16; ++i) m[i] = 0.0f;
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }

    Matrix(float m0, float m1, float m2, float m3,
           float m4, float m5, float m6, float m7,
           float m8, float m9, float m10, float m11,
           float m12, float m13, float m14, float m15)
    {
        m[0]=m0;m[1]=m1;m[2]=m2;m[3]=m3;
        m[4]=m4;m[5]=m5;m[6]=m6;m[7]=m7;
        m[8]=m8;m[9]=m9;m[10]=m10;m[11]=m11;
        m[12]=m12;m[13]=m13;m[14]=m14;m[15]=m15;
    }

    Vec4 mul(const Vec4& pVec) const
    {
        return Vec4(
            pVec.x * m[0] + pVec.y * m[1] + pVec.z * m[2] + pVec.w * m[3],
            pVec.x * m[4] + pVec.y * m[5] + pVec.z * m[6] + pVec.w * m[7],
            pVec.x * m[8] + pVec.y * m[9] + pVec.z * m[10] + pVec.w * m[11],
            pVec.x * m[12] + pVec.y * m[13] + pVec.z * m[14] + pVec.w * m[15]
        );
    }

    Vec3 mulPoint(const Vec3& pVec) const
    {
        return Vec3((pVec.x * m[0] + pVec.y * m[1] + pVec.z * m[2]) + m[3],
                    (pVec.x * m[4] + pVec.y * m[5] + pVec.z * m[6]) + m[7],
                    (pVec.x * m[8] + pVec.y * m[9] + pVec.z * m[10]) + m[11]);
    }

    Vec3 mulVec(const Vec3& pVec) const
    {
        return Vec3((pVec.x * m[0] + pVec.y * m[1] + pVec.z * m[2]),
                    (pVec.x * m[4] + pVec.y * m[5] + pVec.z * m[6]),
                    (pVec.x * m[8] + pVec.y * m[9] + pVec.z * m[10]));
    }

    void translation(const Vec3& pVec) { m[3] += pVec.x; m[7] += pVec.y; m[11] += pVec.z; }
    void scaling(const Vec3& pVec) { m[0] = pVec.x; m[5] = pVec.y; m[10] = pVec.z; }

    // Helper to set identity explicitly
    void setIdentity() { for (int i = 0; i < 16; ++i) m[i] = 0.0f; m[0] = m[5] = m[10] = m[15] = 1.0f; }

    void rotAroundX(float angle) { m[5] = cosf(angle); m[6] = -sinf(angle); m[9] = sinf(angle); m[10] = cosf(angle); }
    void rotAroundY(float angle) { m[0] = cosf(angle); m[2] = sinf(angle); m[8] = -sinf(angle); m[10] = cosf(angle); }
    void rotAroundZ(float angle) { m[0] = cosf(angle); m[1] = -sinf(angle); m[4] = sinf(angle); m[5] = cosf(angle); }

    Matrix multiply(const Matrix& matrix) const
    {
        Matrix ret;
        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c)
            {
                ret.m[r*4 + c] = 0.0f;
                for (int k = 0; k < 4; ++k) ret.m[r*4 + c] += m[r*4 + k] * matrix.m[k*4 + c];
            }
        return ret;
    }

    float& operator[](int index) { return m[index]; }

    Matrix invert() const
    {
        Matrix inv;
        inv.m[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
        inv.m[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
        inv.m[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
        inv.m[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
        inv.m[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
        inv.m[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
        inv.m[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
        inv.m[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
        inv.m[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
        inv.m[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
        inv.m[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
        inv.m[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
        inv.m[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
        inv.m[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
        inv.m[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
        inv.m[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
        float det = m[0] * inv.m[0] + m[1] * inv.m[4] + m[2] * inv.m[8] + m[3] * inv.m[12];
        if (det == 0.0f) return Matrix();
        det = 1.0f / det;
        for (int i = 0; i < 16; ++i) inv.m[i] *= det;
        return inv;
    }

    Matrix transpose(const Matrix& matrix) const
    {
        Matrix r;
        for (int i=0;i<4;++i) for (int j=0;j<4;++j) r.m[i*4+j] = matrix.m[j*4+i];
        return r;
    }

    Matrix perspectiveProjection(float _aspect, float _fov, float _near, float _far) const
    {
        Matrix p;
        float tanHalfFov = tanf((_fov / 180.0f) * PI_F / 2.0f);
        p.m[0] = 1.0f / (_aspect * tanHalfFov);
        p.m[5] = 1.0f / tanHalfFov;
        p.m[10] = _far / (_far - _near);
        p.m[11] = -(_far * _near) / (_far - _near);
        p.m[14] = 1.0f;
        p.m[15] = 0.0f;
        return p;
    }

    Matrix lookAtMatrix(const Vec3& _from, const Vec3& _to, const Vec3& _up) const
    {
        Matrix m;
        Vec3 direction = (_to - _from).normalize();
        Vec3 right = _up.Cross(direction).normalize();
        Vec3 correctedUp = direction.Cross(right).normalize();

        m.m[0] = right.x; m.m[1] = right.y; m.m[2] = right.z; m.m[3] = -_from.Dot(right);
        m.m[4] = correctedUp.x; m.m[5] = correctedUp.y; m.m[6] = correctedUp.z; m.m[7] = -_from.Dot(correctedUp);
        m.m[8] = direction.x; m.m[9] = direction.y; m.m[10] = direction.z; m.m[11] = -_from.Dot(direction);
        m.m[12] = 0.0f; m.m[13] = 0.0f; m.m[14] = 0.0f; m.m[15] = 1.0f;
        return m;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

class SphereCoordinates
{
public:
    float theta, phi;
    SphereCoordinates() : theta(0), phi(0) {}
    SphereCoordinates(float _theta, float _phi) : theta(_theta), phi(_phi) {}
    SphereCoordinates(float x, float y, float z) { FromVec3(x,y,z); }
    void FromVec3(float x, float y, float z) { theta = acosf(z); phi = atan2f(y, x); }
    Vec3 ToVec3() { return Vec3(sinf(theta) * cosf(phi), sinf(theta) * sinf(phi), cosf(theta)); }
};

/////////////////////////////////////////////////////////////////////////////////////////

class Quaternion
{
public:
    union { struct { float a,b,c,d; }; float q[4]; };
    Quaternion() : a(0), b(0), c(0), d(1) {}
    Quaternion(float x, float y, float z, float w) : a(x), b(y), c(z), d(w) {}

    Matrix toMatrix() const
    {
        float aa = a * a, ab = a * b, ac = a * c;
        float bb = b * b, bc = b * c, cc = c * c;
        float da = d * a, db = d * b, dc = d * c;
        Matrix m;
        m.m[0] = 1 - 2 * (bb + cc); m.m[1] = 2 * (ab - dc); m.m[2] = 2 * (ac + db); m.m[3] = 0;
        m.m[4] = 2 * (ab + dc); m.m[5] = 1 - 2 * (aa + cc); m.m[6] = 2 * (bc - da); m.m[7] = 0;
        m.m[8] = 2 * (ac - db); m.m[9] = 2 * (bc + da); m.m[10] = 1 - 2 * (aa + bb); m.m[11] = 0;
        m.m[12] = m.m[13] = m.m[14] = 0; m.m[15] = 1;
        return m;
    }

    Quaternion Normalised() const
    {
        float magnitude = sqrtf(a*a + b*b + c*c + d*d);
        if (magnitude > 0.0f) { float inv = 1.0f / magnitude; return Quaternion(a*inv, b*inv, c*inv, d*inv); }
        return Quaternion(0,0,0,1);
    }

    Quaternion Conjugate() const { return Quaternion(-a, -b, -c, d); }
    Quaternion operator-() const { return Quaternion(-a, -b, -c, -d); }

    static Quaternion Slerp(Quaternion q1, Quaternion q2, float time)
    {
        float dotProduct = q1.a * q2.a + q1.b * q2.b + q1.c * q2.c + q1.d * q2.d;
        if (dotProduct < 0.0f) { q2.a = -q2.a; q2.b = -q2.b; q2.c = -q2.c; q2.d = -q2.d; dotProduct = -dotProduct; }
        float theta = acosf(dotProduct);
        float sinTheta = sinf(theta);
        if (sinTheta == 0.0f) return q1;
        float w1 = sinf(theta * (1 - time)) / sinTheta;
        float w2 = sinf(theta * time) / sinTheta;
        return Quaternion(q1.a * w1 + q2.a * w2, q1.b * w1 + q2.b * w2, q1.c * w1 + q2.c * w2, q1.d * w1 + q2.d * w2);
    }

    static Quaternion quatMul(const Quaternion& q1, const Quaternion& q2)
    {
        return Quaternion(
            q1.d * q2.d - q1.a * q2.a - q1.b * q2.b - q1.c * q2.c,
            q1.d * q2.a + q1.a * q2.d + q1.b * q2.c - q1.c * q2.b,
            q1.d * q2.b - q1.a * q2.c + q1.b * q2.d + q1.c * q2.a,
            q1.d * q2.c + q1.a * q2.b - q1.b * q2.a + q1.c * q2.d);
    }

    static Quaternion FromAxisAngle(const Vec3& axis, float angleRadians)
    {
        Vec3 v = axis.normalize(); float halfAngle = angleRadians * 0.5f; float s = sinf(halfAngle); float w = cosf(halfAngle);
        return Quaternion(v.x * s, v.y * s, v.z * s, w);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

class Colour
{
public:
    float r,g,b,a;
    Colour() : r(0), g(0), b(0), a(0) {}
    Colour(float _r, float _g, float _b, float _a = 1.0f) : r(_r), g(_g), b(_b), a(_a) {}
    Colour(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 255)
        : r(_r / 255.0f), g(_g / 255.0f), b(_b / 255.0f), a(_a / 255.0f) {}
    Colour operator+(const Colour& v) const { return Colour(r+v.r, g+v.g, b+v.b, a+v.a); }
    Colour operator-(const Colour& v) const { return Colour(r-v.r, g-v.g, b-v.b, a-v.a); }
    Colour operator*(const float s) const { return Colour(r*s, g*s, b*s, a*s); }
    Colour operator/(const float s) const { return Colour(r/s, g/s, b/s, a/s); }
    Colour RGBMult(float v) const { return Colour(r*v, g*v, b*v, a); }
};

/////////////////////////////////////////////////////////////////////////////////////////

class ShadingFrame
{
public:
    Vec3 N, T, B;
    ShadingFrame(const Vec3& pVec) { BuildFromNormal(pVec); }
    ShadingFrame() : N(0,0,1), T(1,0,0), B(0,1,0) {}
    void BuildFromNormal(const Vec3& pVec)
    {
        N = pVec.normalize();
        Vec3 helper = (fabs(N.x) > cosf(45.0f * PI_F / 180.0f) ? Vec3(0.0f,1.0f,0.0f) : Vec3(1.0f,0.0f,0.0f));
        T = helper - N * N.Dot(helper);
        T = T.normalize();
        B = N.Cross(T);
        B = B.normalize();
    }
    Vec3 LocalToWorld(const Vec3& pVec) const { return T * pVec.x + B * pVec.y + N * pVec.z; }
    Vec3 WorldToLocal(const Vec3& pVec) const { return Vec3(pVec.Dot(T), pVec.Dot(B), pVec.Dot(N)); }
};

/////////////////////////////////////////////////////////////////////////////////////////

class Triangle
{
public:
    Vec4 v0, v1, v2;
    Triangle(Vec4 a, Vec4 b, Vec4 c) : v0(a), v1(b), v2(c) {}
    float edgeFunction(const Vec4& a, const Vec4& b, const Vec4& p) const { return ((p.x - a.x) * (b.y - a.y)) - ((b.x - a.x) * (p.y - a.y)); }
    void findBounds(Vec4& tr, Vec4& bl, GamesEngineeringBase::Window& canvas) const
    {
        tr.x = min(max(max(v0.x,v1.x), v2.x), canvas.getWidth()-1);
        tr.y = min(max(max(v0.y,v1.y), v2.y), canvas.getHeight()-1);
        bl.x = max(min(min(v0.x,v1.x), v2.x), 0);
        bl.y = max(min(min(v0.y,v1.y), v2.y), 0);
    }
    void barycentricCoordinates(const Vec4& p, float& alpha, float& beta, float& gamma) const
    {
        alpha = edgeFunction(v1, v2, p);
        beta = edgeFunction(v2, v0, p);
        gamma = edgeFunction(v0, v1, p);
        float projArea = edgeFunction(v0, v1, v2);
        float area = 1.0f / projArea;
        alpha *= area; beta *= area; gamma *= area;
    }
};

// Template to nicely colour the triangle
template<typename t>
t simpleInterpolateAttribute(t a0, t a1, t a2, float alpha, float beta, float gamma)
{
    return (a0 * alpha) + (a1 * beta) + (a2 * gamma);
}

/////////////////////////////////////////////////////////////////////////////////////////