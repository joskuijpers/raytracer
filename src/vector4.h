#pragma once

#include <cmath>
#include <iostream>

template<typename T> class Vector4;
template<typename T> class Vector3;

template <class T> bool operator!= (const Vector4<T> &p1, const Vector4<T> &p2) {
    return (p1[0] != p2[0] ||
            p1[1] != p2[1] ||
            p1[2] != p2[2] ||
            p1[3] != p2[3]);
}

template <class T> const Vector4<T> operator* (const Vector4<T> &p, float factor) {
    return Vector4<T> (p[0] * factor,
                       p[1] * factor,
                       p[2] * factor,
                       p[3] * factor);
}

template <class T> const Vector4<T> operator* (float factor, const Vector4<T> &p) {
    return Vector4<T> (p[0] * factor,
                       p[1] * factor,
                       p[2] * factor,
                       p[3] * factor);
}

template <class T> const Vector4<T> operator* (const Vector4<T> &p1, const Vector4<T> &p2) {
    return Vector4<T> (p1[0] * p2[0],
                       p1[1] * p2[1],
                       p1[2] * p2[2],
                       p1[3] * p2[3]);
}

template <class T> const Vector4<T> operator+ (const Vector4<T> &p1, const Vector4<T> &p2) {
    return Vector4<T> (p1[0] + p2[0],
                       p1[1] + p2[1],
                       p1[2] + p2[2],
                       p1[3] + p2[3]);
}

template <class T> const Vector4<T> operator- (const Vector4<T> &p1, const Vector4<T> &p2) {
    return Vector4<T> (p1[0] - p2[0],
                       p1[1] - p2[1],
                       p1[2] - p2[2],
                       p1[3] - p2[3]);
}

template <class T> const Vector4<T> operator- (const Vector4<T> &p) {
    return Vector4<T> (-p[0],
                       -p[1],
                       -p[2],
                       -p[3]);
}

template <class T> const Vector4<T> operator/ (const Vector4<T> &p, float divisor) {
    return Vector4<T> (p[0]/divisor,
                       p[1]/divisor,
                       p[2]/divisor,
                       p[3]/divisor);
}

template <class T> bool operator== (const Vector4<T> &p1, const Vector4<T> &p2) {
    return (p1[0] == p2[0] &&
            p1[1] == p2[1] &&
            p1[2] == p2[2] &&
            p1[3] == p2[3]);
}

template <class T> bool operator< (const Vector4<T> &a, const Vector4<T> &b) {
    return (a[0] < b[0] &&
            a[1] < b[1] &&
            a[2] < b[2] &&
            a[3] < b[3]);
}

template <class T> bool operator>= (const Vector4<T> &a, const Vector4<T> &b) {
    return (a[0] >= b[0] ||
            a[1] >= b[1] ||
            a[2] >= b[2] ||
            a[3] >= b[3]);
}


/**
 * Vector in 3 dimensions, with basics operators overloaded.
 */
template <typename T>
class Vector4 {

#pragma mark - Instance variables

private:
    T p[4];

public:
    inline Vector4(void)	{
        p[0] = p[1] = p[2] = p[3] = T ();
    }

    inline Vector4(T p0, T p1, T p2, T p3) {
        p[0] = p0;
        p[1] = p1;
        p[2] = p2;
        p[3] = p3;
    }

    inline Vector4(const Vector4 &v) {
        init(v[0], v[1], v[2], v[3]);
    }

    inline Vector4(const Vector3<T> &v) {
        init(v[0], v[1], v[2], 1.f);
    }

    inline Vector4(T* pp) {
        p[0] = pp[0];
        p[1] = pp[1];
        p[2] = pp[2];
        p[3] = pp[3];
    }

#pragma mark - Operators

    inline T& operator[] (int Index) {
        return p[Index];
    }

    inline const T& operator[] (int Index) const {
        return p[Index];
    }

    inline Vector4& operator= (const Vector4 &P) {
        p[0] = P[0];
        p[1] = P[1];
        p[2] = P[2];
        p[3] = P[3];
        return (*this);
    }

    inline Vector4& operator+= (const Vector4 &P) {
        p[0] += P[0];
        p[1] += P[1];
        p[2] += P[2];
        p[3] += P[3];
        return (*this);
    }

    inline Vector4& operator-= (const Vector4 &P) {
        p[0] -= P[0];
        p[1] -= P[1];
        p[2] -= P[2];
        p[3] -= P[3];
        return (*this);
    }

    inline Vector4& operator*= (const Vector4 &P) {
        p[0] *= P[0];
        p[1] *= P[1];
        p[2] *= P[2];
        p[3] *= P[3];
        return (*this);
    }

    inline Vector4& operator*= (T s) {
        p[0] *= s;
        p[1] *= s;
        p[2] *= s;
        p[3] *= s;
        return (*this);
    }

    inline Vector4& operator/= (const Vector4 &P) {
        p[0] /= P[0];
        p[1] /= P[1];
        p[2] /= P[2];
        p[3] /= P[3];
        return (*this);
    }

    inline Vector4& operator/= (T s) {
        p[0] /= s;
        p[1] /= s;
        p[2] /= s;
        p[3] /= s;
        return (*this);
    }

#pragma mark - Methods

    inline Vector4 & init (T x, T y, T z, T w) {
        p[0] = x;
        p[1] = y;
        p[2] = z;
        p[3] = w;
        return (*this);
    }

    inline T getSquaredLength() const {
        return this->dot(*this);
    }

    inline T getLength() const {
        return (T)sqrt(getSquaredLength());
    }

    /// Return length after normalization
    inline T normalize(void) {
        T length = getLength();

        if (length == 0.0f)
            return 0;

        T rezLength = 1.0f / length;
        p[0] *= rezLength;
        p[1] *= rezLength;
        p[2] *= rezLength;
        p[3] *= rezLength;
        return length;
    }

    inline void fromTo(const Vector4 &P1, const Vector4 &P2) {
        p[0] = P2[0] - P1[0];
        p[1] = P2[1] - P1[1];
        p[2] = P2[2] - P1[2];
        p[3] = P2[3] - P1[3];
    }

    /*
    inline void getTwoOrthogonals(Vector3 &u, Vector3 &v) const {
        if (fabs(p[0]) < fabs(p[1])) {
            if (fabs(p[0]) < fabs(p[2]))
                u = Vector3 (0, -p[2], p[1]);
            else
                u = Vector3 (-p[1], p[0], 0);
        } else {
            if (fabs(p[1]) < fabs(p[2]))
                u = Vector3 (p[2], 0, -p[0]);
            else
                u = Vector3(-p[1], p[0], 0);
        }
        v = crossProduct (*this, u);
    }

    inline Vector3 projectOn(const Vector3 &N, const Vector3 &P) const {
        T w;

        w = dotProduct (((*this) - P), N);

        return (*this) - (N * w);
    }

    static inline Vector3 segment (const Vector3 & a, const Vector3 & b) {
        Vector3 r;

        r[0] = b[0] - a[0];
        r[1] = b[1] - a[1];
        r[2] = b[2] - a[2];

        return r;
    }*/

    inline T dot(const Vector4 &v) const {
        return p[0] * v[0] + p[1] * v[1] + p[2] * v[2] + p[3] * v[3];
    }

    /// @warning Primary use of Vector4 is homogenous coords, so cross drops the w element.
    inline Vector4 cross(const Vector4 &v) const {
        Vector3<T> result;

        result[0] = p[1] * v[2] - p[2] * v[1];
        result[1] = p[2] * v[0] - p[0] * v[2];
        result[2] = p[0] * v[1] - p[1] * v[0];

        return Vector4(result);
    }

    static inline T squaredDistance(const Vector4 &u, const Vector4 &v) {
        Vector4 tmp;

        tmp = u - v;

        return tmp.getSquaredLength();
    }

    static inline T distance(const Vector4&u, const Vector4 &v) {
        Vector4 tmp;

        tmp = u - v;

        return tmp.getLength();
    }

    static inline Vector4 interpolate(const Vector4 &u, const Vector4 &v, T alpha) {
        return u * (1.0f - alpha) + v * alpha;
    }

    inline void set(T a, T b, T c, T d) {
        p[0] = a;
        p[1] = b;
        p[2] = c;
        p[3] = d;
    }


    T* pointer() {
        return p;
    }

    const T* pointer() const {
        return p;
    }
};

template <class T> inline Vector4<T> swap (Vector4<T> &u, Vector4<T> &v) {
    Vector4<T> tmp;

    tmp = u;
    u = v;
    v = tmp;
}

template <class T> std::ostream& operator<< (std::ostream &output, const Vector4<T> &v) {
    output << v[0] << " " << v[1] << " " << v[2] << " " << v[3];
    
    return output;
}

template <class T> std::istream& operator>> (std::istream &input, Vector4<T> &v) {
    input >> v[0] >> v[1] >> v[2] >> v[3];
    
    return input;
}

typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;
typedef Vector4<int> Vector4i;
