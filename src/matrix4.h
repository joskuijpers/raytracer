#pragma once

#include "vector3.h"
#include "vector4.h"

#include <iostream>
#include <iomanip>

/// @note: used some code from http://www.songho.ca/opengl/gl_matrix.html
/// @note: inversion code from http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix

template<typename T> class Matrix4;

#define DEG2RAD ((float)(3.141593f / 180))
#define EPSILON ((float)0.00001f)

/**
 * Matrix in 4 dimensions, x, y, z and w.
 *
 * A classical 3D transformation matrix.
 */
template <typename T>
class Matrix4
{
private:
    T m[16];
    T tm[16];

public:
    inline Matrix4(void) {
        identity();
    }

    inline Matrix4(const T src[16]) {
        m = src;
    }

    inline Matrix4(T m00, T m01, T m02, T m03,
                   T m10, T m11, T m12, T m13,
                   T m20, T m21, T m22, T m23,
                   T m30, T m31, T m32, T m33) {
        set(m00, m01, m02, m03,
            m10, m11, m12, m13,
            m20, m21, m22, m23,
            m30, m31, m32, m33);
    }


    inline void set(const float src[16]) {
        set(src);
    }

    inline void set(T m00, T m01, T m02, T m03,
                    T m04, T m05, T m06, T m07,
                    T m08, T m09, T m10, T m11,
                    T m12, T m13, T m14, T m15) {
        m[0] = m00;  m[1] = m01;  m[2] = m02;  m[3] = m03;
        m[4] = m04;  m[5] = m05;  m[6] = m06;  m[7] = m07;
        m[8] = m08;  m[9] = m09;  m[10]= m10;  m[11]= m11;
        m[12]= m12;  m[13]= m13;  m[14]= m14;  m[15]= m15;
    }

    const T* get() const {
        return m;
    }

    const T* getTranspose() const {
        tm[0] = m[0];   tm[1] = m[4];   tm[2] = m[8];   tm[3] = m[12];
        tm[4] = m[1];   tm[5] = m[5];   tm[6] = m[9];   tm[7] = m[13];
        tm[8] = m[2];   tm[9] = m[6];   tm[10]= m[10];  tm[11]= m[14];
        tm[12]= m[3];   tm[13]= m[7];   tm[14]= m[11];  tm[15]= m[15];

        return tm;
    }

    T getDeterminant(void) const {
        return m[0] * getCofactor(m[5],m[6],m[7], m[9],m[10],m[11], m[13],m[14],m[15]) -
                m[1] * getCofactor(m[4],m[6],m[7], m[8],m[10],m[11], m[12],m[14],m[15]) +
                m[2] * getCofactor(m[4],m[5],m[7], m[8],m[9], m[11], m[12],m[13],m[15]) -
                m[3] * getCofactor(m[4],m[5],m[6], m[8],m[9], m[10], m[12],m[13],m[14]);
    }

#pragma mark - Matrix operations

    inline Matrix4& identity(void) {
        set(1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1);

        return (*this);
    }

    inline Matrix4& transpose(void) {
        swap(m[1], m[4]);
        swap(m[2], m[8]);
        swap(m[3], m[12]);
        swap(m[6], m[9]);
        swap(m[7], m[13]);
        swap(m[11], m[14]);

        return (*this);
    }

    inline Matrix4& invert(void) {
        if(m[3] == 0 && m[7] == 0 && m[11] == 0 && m[15] == 1)
            this->invertAffine();
        else
            this->invertGeneral();
        return (*this);
    }

    inline Matrix4& invertEuclidean(void) {
        T tmp;

        tmp = m[1]; m[1] = m[4]; m[4] = tmp;
        tmp = m[2]; m[2] = m[8]; m[8] = tmp;
        tmp = m[6]; m[6] = m[9]; m[9] = tmp;

        T x = m[12], y = m[13], z = m[14];
        m[12] = -(m[0] * x + m[4] * y + m[8] * z);
        m[13] = -(m[1] * x + m[5] * y + m[9] * z);
        m[14] = -(m[2] * x + m[6] * y + m[10] * z);

        return (*this);
    }

    inline Matrix4& invertAffine(void) {
        // get cofactors of minor matrices
        T cofactor0 = getCofactor(m[5],m[6],m[7], m[9],m[10],m[11], m[13],m[14],m[15]);
        T cofactor1 = getCofactor(m[4],m[6],m[7], m[8],m[10],m[11], m[12],m[14],m[15]);
        T cofactor2 = getCofactor(m[4],m[5],m[7], m[8],m[9], m[11], m[12],m[13],m[15]);
        T cofactor3 = getCofactor(m[4],m[5],m[6], m[8],m[9], m[10], m[12],m[13],m[14]);

        // get determinant
        float determinant = m[0] * cofactor0 - m[1] * cofactor1 + m[2] * cofactor2 - m[3] * cofactor3;
        if(fabs(determinant) <= EPSILON)
        {
            return identity();
        }

        // get rest of cofactors for adj(M)
        T cofactor4 = getCofactor(m[1],m[2],m[3], m[9],m[10],m[11], m[13],m[14],m[15]);
        T cofactor5 = getCofactor(m[0],m[2],m[3], m[8],m[10],m[11], m[12],m[14],m[15]);
        T cofactor6 = getCofactor(m[0],m[1],m[3], m[8],m[9], m[11], m[12],m[13],m[15]);
        T cofactor7 = getCofactor(m[0],m[1],m[2], m[8],m[9], m[10], m[12],m[13],m[14]);

        T cofactor8 = getCofactor(m[1],m[2],m[3], m[5],m[6], m[7],  m[13],m[14],m[15]);
        T cofactor9 = getCofactor(m[0],m[2],m[3], m[4],m[6], m[7],  m[12],m[14],m[15]);
        T cofactor10= getCofactor(m[0],m[1],m[3], m[4],m[5], m[7],  m[12],m[13],m[15]);
        T cofactor11= getCofactor(m[0],m[1],m[2], m[4],m[5], m[6],  m[12],m[13],m[14]);

        T cofactor12= getCofactor(m[1],m[2],m[3], m[5],m[6], m[7],  m[9], m[10],m[11]);
        T cofactor13= getCofactor(m[0],m[2],m[3], m[4],m[6], m[7],  m[8], m[10],m[11]);
        T cofactor14= getCofactor(m[0],m[1],m[3], m[4],m[5], m[7],  m[8], m[9], m[11]);
        T cofactor15= getCofactor(m[0],m[1],m[2], m[4],m[5], m[6],  m[8], m[9], m[10]);

        // build inverse matrix = adj(M) / det(M)
        // adjugate of M is the transpose of the cofactor matrix of M
        float invDeterminant = 1.0f / determinant;
        m[0] =  invDeterminant * cofactor0;
        m[1] = -invDeterminant * cofactor4;
        m[2] =  invDeterminant * cofactor8;
        m[3] = -invDeterminant * cofactor12;

        m[4] = -invDeterminant * cofactor1;
        m[5] =  invDeterminant * cofactor5;
        m[6] = -invDeterminant * cofactor9;
        m[7] =  invDeterminant * cofactor13;

        m[8] =  invDeterminant * cofactor2;
        m[9] = -invDeterminant * cofactor6;
        m[10]=  invDeterminant * cofactor10;
        m[11]= -invDeterminant * cofactor14;
        
        m[12]= -invDeterminant * cofactor3;
        m[13]=  invDeterminant * cofactor7;
        m[14]= -invDeterminant * cofactor11;
        m[15]=  invDeterminant * cofactor15;

        return (*this);
    }

    inline Matrix4& invertGeneral(void) {
        T inv[16], det;

        inv[0] = m[5]  * m[10] * m[15] -
        m[5]  * m[11] * m[14] -
        m[9]  * m[6]  * m[15] +
        m[9]  * m[7]  * m[14] +
        m[13] * m[6]  * m[11] -
        m[13] * m[7]  * m[10];

        inv[4] = -m[4]  * m[10] * m[15] +
        m[4]  * m[11] * m[14] +
        m[8]  * m[6]  * m[15] -
        m[8]  * m[7]  * m[14] -
        m[12] * m[6]  * m[11] +
        m[12] * m[7]  * m[10];

        inv[8] = m[4]  * m[9] * m[15] -
        m[4]  * m[11] * m[13] -
        m[8]  * m[5] * m[15] +
        m[8]  * m[7] * m[13] +
        m[12] * m[5] * m[11] -
        m[12] * m[7] * m[9];

        inv[12] = -m[4]  * m[9] * m[14] +
        m[4]  * m[10] * m[13] +
        m[8]  * m[5] * m[14] -
        m[8]  * m[6] * m[13] -
        m[12] * m[5] * m[10] +
        m[12] * m[6] * m[9];

        inv[1] = -m[1]  * m[10] * m[15] +
        m[1]  * m[11] * m[14] +
        m[9]  * m[2] * m[15] -
        m[9]  * m[3] * m[14] -
        m[13] * m[2] * m[11] +
        m[13] * m[3] * m[10];

        inv[5] = m[0]  * m[10] * m[15] -
        m[0]  * m[11] * m[14] -
        m[8]  * m[2] * m[15] +
        m[8]  * m[3] * m[14] +
        m[12] * m[2] * m[11] -
        m[12] * m[3] * m[10];

        inv[9] = -m[0]  * m[9] * m[15] +
        m[0]  * m[11] * m[13] +
        m[8]  * m[1] * m[15] -
        m[8]  * m[3] * m[13] -
        m[12] * m[1] * m[11] +
        m[12] * m[3] * m[9];

        inv[13] = m[0]  * m[9] * m[14] -
        m[0]  * m[10] * m[13] -
        m[8]  * m[1] * m[14] +
        m[8]  * m[2] * m[13] +
        m[12] * m[1] * m[10] -
        m[12] * m[2] * m[9];

        inv[2] = m[1]  * m[6] * m[15] -
        m[1]  * m[7] * m[14] -
        m[5]  * m[2] * m[15] +
        m[5]  * m[3] * m[14] +
        m[13] * m[2] * m[7] -
        m[13] * m[3] * m[6];

        inv[6] = -m[0]  * m[6] * m[15] +
        m[0]  * m[7] * m[14] +
        m[4]  * m[2] * m[15] -
        m[4]  * m[3] * m[14] -
        m[12] * m[2] * m[7] +
        m[12] * m[3] * m[6];

        inv[10] = m[0]  * m[5] * m[15] -
        m[0]  * m[7] * m[13] -
        m[4]  * m[1] * m[15] +
        m[4]  * m[3] * m[13] +
        m[12] * m[1] * m[7] -
        m[12] * m[3] * m[5];

        inv[14] = -m[0]  * m[5] * m[14] +
        m[0]  * m[6] * m[13] +
        m[4]  * m[1] * m[14] -
        m[4]  * m[2] * m[13] -
        m[12] * m[1] * m[6] +
        m[12] * m[2] * m[5];

        inv[3] = -m[1] * m[6] * m[11] +
        m[1] * m[7] * m[10] +
        m[5] * m[2] * m[11] -
        m[5] * m[3] * m[10] -
        m[9] * m[2] * m[7] +
        m[9] * m[3] * m[6];

        inv[7] = m[0] * m[6] * m[11] -
        m[0] * m[7] * m[10] -
        m[4] * m[2] * m[11] +
        m[4] * m[3] * m[10] +
        m[8] * m[2] * m[7] -
        m[8] * m[3] * m[6];

        inv[11] = -m[0] * m[5] * m[11] +
        m[0] * m[7] * m[9] +
        m[4] * m[1] * m[11] -
        m[4] * m[3] * m[9] -
        m[8] * m[1] * m[7] +
        m[8] * m[3] * m[5];

        inv[15] = m[0] * m[5] * m[10] -
        m[0] * m[6] * m[9] -
        m[4] * m[1] * m[10] +
        m[4] * m[2] * m[9] +
        m[8] * m[1] * m[6] -
        m[8] * m[2] * m[5];

        det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

        if (fabs(det) <= EPSILON)
            return identity();

        det = 1.0 / det;

        for (int i = 0; i < 16; ++i)
            m[i] = inv[i] * det;

        return (*this);
    }

#pragma mark - Transformation matrix

    inline Matrix4& translate(T x, T y, T z) {
        m[0] += m[3] * x;   m[4] += m[7] * x;   m[8] += m[11]* x;   m[12]+= m[15]* x;
        m[1] += m[3] * y;   m[5] += m[7] * y;   m[9] += m[11]* y;   m[13]+= m[15]* y;
        m[2] += m[3] * z;   m[6] += m[7] * z;   m[10]+= m[11]* z;   m[14]+= m[15]* z;

        return (*this);
    }

    inline Matrix4& translate(const Vector3<T>& v) {
        return translate(v[0],v[1],v[2]);
    }

    inline Matrix4& rotate(const float angle, const Vector3<T>& axis) {
        return rotate(angle,axis[0],axis[1],axis[2]);
    }

    inline Matrix4& rotate(const float angle, T x, T y, T z) {
        float c = cosf(angle * DEG2RAD);    // cosine
        float s = sinf(angle * DEG2RAD);    // sine
        float c1 = 1.0f - c;                // 1 - c
        T m0 = m[0],  m4 = m[4],  m8 = m[8],  m12= m[12],
        m1 = m[1],  m5 = m[5],  m9 = m[9],  m13= m[13],
        m2 = m[2],  m6 = m[6],  m10= m[10], m14= m[14];

        // build rotation matrix
        T r0 = x * x * c1 + c;
        T r1 = x * y * c1 + z * s;
        T r2 = x * z * c1 - y * s;
        T r4 = x * y * c1 - z * s;
        T r5 = y * y * c1 + c;
        T r6 = y * z * c1 + x * s;
        T r8 = x * z * c1 + y * s;
        T r9 = y * z * c1 - x * s;
        T r10= z * z * c1 + c;

        // multiply rotation matrix
        m[0] = r0 * m0 + r4 * m1 + r8 * m2;
        m[1] = r1 * m0 + r5 * m1 + r9 * m2;
        m[2] = r2 * m0 + r6 * m1 + r10* m2;
        m[4] = r0 * m4 + r4 * m5 + r8 * m6;
        m[5] = r1 * m4 + r5 * m5 + r9 * m6;
        m[6] = r2 * m4 + r6 * m5 + r10* m6;
        m[8] = r0 * m8 + r4 * m9 + r8 * m10;
        m[9] = r1 * m8 + r5 * m9 + r9 * m10;
        m[10]= r2 * m8 + r6 * m9 + r10* m10;
        m[12]= r0 * m12+ r4 * m13+ r8 * m14;
        m[13]= r1 * m12+ r5 * m13+ r9 * m14;
        m[14]= r2 * m12+ r6 * m13+ r10* m14;

        return (*this);
    }

    inline Matrix4& rotateX(const float angle) {
        return rotate(angle,1,0,0);
    }

    inline Matrix4& rotateY(const float angle) {
        return rotate(angle,0,1,0);
    }

    inline Matrix4& rotateZ(const float angle) {
        return rotate(angle,0,0,1);
    }

    inline Matrix4& scale(T s) {
        return scale(s,s,s);
    }

    inline Matrix4& scale(T x, T y, T z) {
        m[0] *= x;  m[4] *= x;  m[8] *= x;  m[12] *= x;
        m[1] *= y;  m[5] *= y;  m[9] *= y;  m[13] *= y;
        m[2] *= z;  m[6] *= z;  m[10] *= z;  m[14] *= z;

        return (*this);
    }

    inline Matrix4& scale(const Vector3<T>& v) {
        return scale(v[0],v[1],v[2]);
    }

#pragma mark - Operators

    inline Matrix4& operator+ (const Matrix4& rhs) {
        return Matrix4(m[0]+rhs[0],   m[1]+rhs[1],   m[2]+rhs[2],   m[3]+rhs[3],
                       m[4]+rhs[4],   m[5]+rhs[5],   m[6]+rhs[6],   m[7]+rhs[7],
                       m[8]+rhs[8],   m[9]+rhs[9],   m[10]+rhs[10], m[11]+rhs[11],
                       m[12]+rhs[12], m[13]+rhs[13], m[14]+rhs[14], m[15]+rhs[15]);
    }

    inline Matrix4& operator- (const Matrix4& rhs) {
        return Matrix4(m[0]-rhs[0],   m[1]-rhs[1],   m[2]-rhs[2],   m[3]-rhs[3],
                       m[4]-rhs[4],   m[5]-rhs[5],   m[6]-rhs[6],   m[7]-rhs[7],
                       m[8]-rhs[8],   m[9]-rhs[9],   m[10]-rhs[10], m[11]-rhs[11],
                       m[12]-rhs[12], m[13]-rhs[13], m[14]-rhs[14], m[15]-rhs[15]);
    }

    inline Matrix4& operator+= (const Matrix4& rhs) {
        m[0] += rhs[0];   m[1] += rhs[1];   m[2] += rhs[2];   m[3] += rhs[3];
        m[4] += rhs[4];   m[5] += rhs[5];   m[6] += rhs[6];   m[7] += rhs[7];
        m[8] += rhs[8];   m[9] += rhs[9];   m[10]+= rhs[10];  m[11]+= rhs[11];
        m[12]+= rhs[12];  m[13]+= rhs[13];  m[14]+= rhs[14];  m[15]+= rhs[15];

        return (*this);
    }

    inline Matrix4& operator-= (const Matrix4& rhs) {
        m[0] -= rhs[0];   m[1] -= rhs[1];   m[2] -= rhs[2];   m[3] -= rhs[3];
        m[4] -= rhs[4];   m[5] -= rhs[5];   m[6] -= rhs[6];   m[7] -= rhs[7];
        m[8] -= rhs[8];   m[9] -= rhs[9];   m[10]-= rhs[10];  m[11]-= rhs[11];
        m[12]-= rhs[12];  m[13]-= rhs[13];  m[14]-= rhs[14];  m[15]-= rhs[15];

        return (*this);
    }

    inline Matrix4& operator* (const Matrix4& n) {
        return Matrix4(m[0]*n[0]  + m[4]*n[1]  + m[8]*n[2]  + m[12]*n[3],   m[1]*n[0]  + m[5]*n[1]  + m[9]*n[2]  + m[13]*n[3],   m[2]*n[0]  + m[6]*n[1]  + m[10]*n[2]  + m[14]*n[3],   m[3]*n[0]  + m[7]*n[1]  + m[11]*n[2]  + m[15]*n[3],
                       m[0]*n[4]  + m[4]*n[5]  + m[8]*n[6]  + m[12]*n[7],   m[1]*n[4]  + m[5]*n[5]  + m[9]*n[6]  + m[13]*n[7],   m[2]*n[4]  + m[6]*n[5]  + m[10]*n[6]  + m[14]*n[7],   m[3]*n[4]  + m[7]*n[5]  + m[11]*n[6]  + m[15]*n[7],
                       m[0]*n[8]  + m[4]*n[9]  + m[8]*n[10] + m[12]*n[11],  m[1]*n[8]  + m[5]*n[9]  + m[9]*n[10] + m[13]*n[11],  m[2]*n[8]  + m[6]*n[9]  + m[10]*n[10] + m[14]*n[11],  m[3]*n[8]  + m[7]*n[9]  + m[11]*n[10] + m[15]*n[11],
                       m[0]*n[12] + m[4]*n[13] + m[8]*n[14] + m[12]*n[15],  m[1]*n[12] + m[5]*n[13] + m[9]*n[14] + m[13]*n[15],  m[2]*n[12] + m[6]*n[13] + m[10]*n[14] + m[14]*n[15],  m[3]*n[12] + m[7]*n[13] + m[11]*n[14] + m[15]*n[15]);
    }

    inline Vector4<T>& operator* (const Vector4<T>& rhs) {
        return Vector4<T>(m[0]*rhs.x + m[4]*rhs.y + m[8]*rhs.z  + m[12]*rhs.w,
                       m[1]*rhs.x + m[5]*rhs.y + m[9]*rhs.z  + m[13]*rhs.w,
                       m[2]*rhs.x + m[6]*rhs.y + m[10]*rhs.z + m[14]*rhs.w,
                       m[3]*rhs.x + m[7]*rhs.y + m[11]*rhs.z + m[15]*rhs.w);
    }

    inline Vector3<T>& operator* (const Vector3<T>& rhs) {
        return Vector3<T>(m[0]*rhs.x + m[4]*rhs.y + m[8]*rhs.z,
                       m[1]*rhs.x + m[5]*rhs.y + m[9]*rhs.z,
                       m[2]*rhs.x + m[6]*rhs.y + m[10]*rhs.z);
    }

    inline Matrix4& operator*= (const Matrix4& rhs) {
        *this = *this * rhs;
        return (*this);
    }

    inline bool operator== (const Matrix4& n) {
        return (m[0] == n[0])  && (m[1] == n[1])  && (m[2] == n[2])  && (m[3] == n[3])  &&
        (m[4] == n[4])  && (m[5] == n[5])  && (m[6] == n[6])  && (m[7] == n[7])  &&
        (m[8] == n[8])  && (m[9] == n[9])  && (m[10]== n[10]) && (m[11]== n[11]) &&
        (m[12]== n[12]) && (m[13]== n[13]) && (m[14]== n[14]) && (m[15]== n[15]);
    }

    inline bool operator!= (const Matrix4& n) {
        return (m[0] != n[0])  || (m[1] != n[1])  || (m[2] != n[2])  || (m[3] != n[3])  ||
        (m[4] != n[4])  || (m[5] != n[5])  || (m[6] != n[6])  || (m[7] != n[7])  ||
        (m[8] != n[8])  || (m[9] != n[9])  || (m[10]!= n[10]) || (m[11]!= n[11]) ||
        (m[12]!= n[12]) || (m[13]!= n[13]) || (m[14]!= n[14]) || (m[15]!= n[15]);
    }

    inline float operator[] (int index) const {
        return m[index];
    }

    inline float& operator[] (int index) {
        return m[index];
    }

private:
    inline T getCofactor(T m0, T m1, T m2,
                         T m3, T m4, T m5,
                         T m6, T m7, T m8)
    {
        return m0 * (m4 * m8 - m5 * m7) -
        m1 * (m3 * m8 - m5 * m6) +
        m2 * (m3 * m7 - m4 * m6);
    }
};

template <class T> inline Matrix4<T> swap (Matrix4<T> &u, Matrix4<T> &v) {
    Matrix4<T> tmp;

    tmp = u;
    u = v;
    v = tmp;
}

template <class T>  inline Matrix4<T> operator-(const Matrix4<T>& rhs) {
    return Matrix4<T>(-rhs[0], -rhs[1], -rhs[2], -rhs[3], -rhs[4], -rhs[5], -rhs[6], -rhs[7], -rhs[8], -rhs[9], -rhs[10], -rhs[11], -rhs[12], -rhs[13], -rhs[14], -rhs[15]);
}

template <class T> inline Matrix4<T> operator*(T s, const Matrix4<T>& rhs) {
    return Matrix4<T>(s*rhs[0], s*rhs[1], s*rhs[2], s*rhs[3], s*rhs[4], s*rhs[5], s*rhs[6], s*rhs[7], s*rhs[8], s*rhs[9], s*rhs[10], s*rhs[11], s*rhs[12], s*rhs[13], s*rhs[14], s*rhs[15]);
}

template <class T> inline Vector4<T> operator*(const Vector4<T>& v, const Matrix4<T>& m) {
    return Vector4<T>(v.x*m[0] + v.y*m[1] + v.z*m[2] + v.w*m[3],  v.x*m[4] + v.y*m[5] + v.z*m[6] + v.w*m[7],  v.x*m[8] + v.y*m[9] + v.z*m[10] + v.w*m[11], v.x*m[12] + v.y*m[13] + v.z*m[14] + v.w*m[15]);
}

template <class T> inline Vector3<T> operator*(const Vector3<T>& v, const Matrix4<T>& m) {
    return Vector3<T>(v.x*m[0] + v.y*m[1] + v.z*m[2],  v.x*m[4] + v.y*m[5] + v.z*m[6],  v.x*m[8] + v.y*m[9] + v.z*m[10]);
}

template <class T> inline std::ostream& operator<<(std::ostream& os, const Matrix4<T>& m)
{
    os << std::fixed << std::setprecision(5);
    os << "[" << std::setw(10) << m[0] << " " << std::setw(10) << m[4] << " " << std::setw(10) << m[8]  <<  " " << std::setw(10) << m[12] << "]\n"
    << "[" << std::setw(10) << m[1] << " " << std::setw(10) << m[5] << " " << std::setw(10) << m[9]  <<  " " << std::setw(10) << m[13] << "]\n"
    << "[" << std::setw(10) << m[2] << " " << std::setw(10) << m[6] << " " << std::setw(10) << m[10] <<  " " << std::setw(10) << m[14] << "]\n"
    << "[" << std::setw(10) << m[3] << " " << std::setw(10) << m[7] << " " << std::setw(10) << m[11] <<  " " << std::setw(10) << m[15] << "]\n";
    os << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
    return os;
}

typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;
typedef Matrix4<int> Matrix4i;

#undef DEG2RAD
#undef EPSILON
