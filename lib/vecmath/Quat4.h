/*
   Copyright (C) 1997,1998,1999
   Kenji Hiranabe, Eiwa System Management, Inc.

   This program is free software.
   Implemented by Kenji Hiranabe(hiranabe@esm.co.jp),
   conforming to the Java(TM) 3D API specification by Sun Microsystems.

   Permission to use, copy, modify, distribute and sell this software
   and its documentation for any purpose is hereby granted without fee,
   provided that the above copyright notice appear in all copies and
   that both that copyright notice and this permission notice appear
   in supporting documentation. Kenji Hiranabe and Eiwa System Management,Inc.
   makes no representations about the suitability of this software for any
   purpose.  It is provided "AS IS" with NO WARRANTY.
*/
#ifndef QUAT4_H
#define QUAT4_H

#include "AxisAngle4_.h"
#include "Quat4_.h"
#include "VmUtil.h"

VM_BEGIN_NS

template<class T>
void Quat4<T>::mul(const Quat4 &q1, const Quat4 &q2) {
    // store on stack for aliasing-safty
    set(q1.x * q2.w + q1.w * q2.x + q1.y * q2.z - q1.z * q2.y,
        q1.y * q2.w + q1.w * q2.y + q1.z * q2.x - q1.x * q2.z,
        q1.z * q2.w + q1.w * q2.z + q1.x * q2.y - q1.y * q2.x,
        q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);
}

template<class T>
void Quat4<T>::mul(const Quat4 &q1) {
    // store on stack for aliasing-safty
    set(this->x * q1.w + this->w * q1.x + this->y * q1.z - this->z * q1.y,
        this->y * q1.w + this->w * q1.y + this->z * q1.x - this->x * q1.z,
        this->z * q1.w + this->w * q1.z + this->x * q1.y - this->y * q1.x,
        this->w * q1.w - this->x * q1.x - this->y * q1.y - this->z * q1.z);
}

template<class T>
void Quat4<T>::mulInverse(const Quat4 &q1, const Quat4 &q2) {
    T n = norm();
    // zero-div may occur.
    n = 1 / n;
    // store on stack once for aliasing-safty
    set((q1.x * q2.w - q1.w * q2.x - q1.y * q2.z + q1.z * q2.y) * n,
        (q1.y * q2.w - q1.w * q2.y - q1.z * q2.x + q1.x * q2.z) * n,
        (q1.z * q2.w - q1.w * q2.z - q1.x * q2.y + q1.y * q2.x) * n,
        (q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z) * n);
}

template<class T>
void Quat4<T>::mulInverse(const Quat4 &q1) {
    T n = norm();
    // zero-div may occur.
    n = 1 / n;
    // store on stack once for aliasing-safty
    set((this->x * q1.w - this->w * q1.x - this->y * q1.z + this->z * q1.y) * n,
        (this->y * q1.w - this->w * q1.y - this->z * q1.x + this->x * q1.z) * n,
        (this->z * q1.w - this->w * q1.z - this->x * q1.y + this->y * q1.x) * n,
        (this->w * q1.w + this->x * q1.x + this->y * q1.y + this->z * q1.z) * n);
}

template<class T>
void Quat4<T>::set(const Matrix4<T> &m1) {
    setFromMat(m1.m00, m1.m01, m1.m02, m1.m10, m1.m11, m1.m12, m1.m20, m1.m21,
               m1.m22);
}

template<class T>
void Quat4<T>::set(const Matrix3<T> &m1) {
    setFromMat(m1.m00, m1.m01, m1.m02, m1.m10, m1.m11, m1.m12, m1.m20, m1.m21,
               m1.m22);
}

template<class T>
void Quat4<T>::set(const AxisAngle4<T> &a1) {
    this->x = a1.x;
    this->y = a1.y;
    this->z = a1.z;
    T n = VmUtil<T>::sqrt(this->x * this->x + this->y * this->y +
                          this->z * this->z);
    // zero-div may occur.
    T s = VmUtil<T>::sin(0.5 * a1.angle) / n;
    this->x *= s;
    this->y *= s;
    this->z *= s;
    this->w = VmUtil<T>::cos(0.5 * a1.angle);
}

template<class T>
void Quat4<T>::interpolate(const Quat4 &q1, T alpha) {
    // From Hoggar.
    normalize();
    T n1 = VmUtil<T>::sqrt(q1.norm());
    // zero-div may occur.
    T x1 = q1.x / n1;
    T y1 = q1.y / n1;
    T z1 = q1.z / n1;
    T w1 = q1.w / n1;

    // t is cosine (dot product)
    T t = this->x * x1 + this->y * y1 + this->z * z1 + this->w * w1;

    // same quaternion (avoid domain error)
    if (1.0 <= VmUtil<T>::abs(t))
        return;

    // t is now theta
    t = VmUtil<T>::acos(t);

    T sin_t = VmUtil<T>::sin(t);

    // same quaternion (avoid zero-div)
    if (sin_t == 0.0)
        return;

    T s = VmUtil<T>::sin((1.0 - alpha) * t) / sin_t;
    t = VmUtil<T>::sin(alpha * t) / sin_t;

    // set values
    this->x = s * this->x + t * x1;
    this->y = s * this->y + t * y1;
    this->z = s * this->z + t * z1;
    this->w = s * this->w + t * w1;
}

template<class T>
void Quat4<T>::interpolate(const Quat4 &q1, const Quat4 &q2, T alpha) {
    set(q1);
    interpolate(q2, alpha);
}

template<class T>
void Quat4<T>::setFromMat(T m00, T m01, T m02, T m10, T m11, T m12, T m20,
                          T m21, T m22) {
    // From Ken Shoemake
    // (ftp://ftp.cis.upenn.edu/pub/graphics/shoemake)

    T s;
    T tr = m00 + m11 + m22;
    if (tr >= 0.0) {
        s = VmUtil<T>::sqrt(tr + 1.0);
        this->w = s * 0.5;
        s = 0.5 / s;
        this->x = (m21 - m12) * s;
        this->y = (m02 - m20) * s;
        this->z = (m10 - m01) * s;
    } else {
        T maxm = VmUtil<T>::max(m00, m11, m22);
        if (maxm == m00) {
            s = VmUtil<T>::sqrt(m00 - (m11 + m22) + 1.0);
            this->x = s * 0.5;
            s = 0.5 / s;
            this->y = (m01 + m10) * s;
            this->z = (m20 + m02) * s;
            this->w = (m21 - m12) * s;
        } else if (maxm == m11) {
            s = VmUtil<T>::sqrt(m11 - (m22 + m00) + 1.0);
            this->y = s * 0.5;
            s = 0.5 / s;
            this->z = (m12 + m21) * s;
            this->x = (m01 + m10) * s;
            this->w = (m02 - m20) * s;
        } else {
            s = VmUtil<T>::sqrt(m22 - (m00 + m11) + 1.0);
            this->z = s * 0.5;
            s = 0.5 / s;
            this->x = (m20 + m02) * s;
            this->y = (m12 + m21) * s;
            this->w = (m10 - m01) * s;
        }
    }
}

template<class T>
Quat4<T> &Quat4<T>::operator*=(const Quat4 &m1) {
    mul(m1);
    return *this;
}

template<class T>
Quat4<T> Quat4<T>::operator*(const Quat4 &m1) const {
    return (Quat4(*this)).operator*=(m1);
}

VM_END_NS

#ifdef VM_INCLUDE_IO

template<class T>
inline VM_IOSTREAM_STD::ostream &operator<<(VM_IOSTREAM_STD::ostream &o,
                                            const VM_VECMATH_NS::Quat4<T> &q1) {
    return operator<<(o, (const VM_VECMATH_NS::Tuple4<T> &) q1);
}

#endif

#endif /* QUAT4_H */
