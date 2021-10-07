/// \file Matrix3.cpp
/// \brief Matrix3 class cpp file
/// \author Evan Hanzelman
/// \version A05

#include "Matrix3.hpp"

#include <iostream>
#include <iomanip>
#include <math.h>

#define toRadians(degree) ((degree * M_PI) / 180.0f)

Matrix3::Matrix3()
{
    setToIdentity ();
}

Matrix3::Matrix3(float rx, float ry, float rz,
                    float ux, float uy, float uz,
                    float bx, float by, float bz)
{
    m_right.m_x = rx;
    m_right.m_y = ry;
    m_right.m_z = rz;
    m_up.m_x = ux;
    m_up.m_y = uy;
    m_up.m_z = uz;
    m_back.m_x = bx;
    m_back.m_y = by;
    m_back.m_z = bz;
}

Matrix3::Matrix3(const Vector3& right, const Vector3& up, const Vector3& back)
{
    m_right = right;
    m_up = up;
    m_back = back;
}

Matrix3::Matrix3(const Vector3& right, const Vector3& up, bool makeOrthonormal)
{
    m_right = right;
    m_up = up;
    m_back = right.cross(up);
    if(makeOrthonormal==true)
    {
        orthonormalize();
    }
}

Matrix3::~Matrix3 ()
{

}

void
Matrix3::setToIdentity()
{
    m_right.m_x = 1.0f;
    m_right.m_y = 0.0f;
    m_right.m_z = 0.0f;
    m_up.m_x = 0.0f;
    m_up.m_y = 1.0f;
    m_up.m_z = 0.0f;
    m_back.m_x = 0.0f;
    m_back.m_y = 0.0f;
    m_back.m_z = 1.0f;
}

void
Matrix3 :: setToZero ()
{
    m_right.m_x = 0.0f;
    m_right.m_y = 0.0f;
    m_right.m_z = 0.0f;
    m_up.m_x = 0.0f;
    m_up.m_y = 0.0f;
    m_up.m_z = 0.0f;
    m_back.m_x = 0.0f;
    m_back.m_y = 0.0f;
    m_back.m_z = 0.0f;
}

float*
Matrix3 :: data ()
{
    float* temp = &m_right.m_x;
    return temp;
}

const float*
Matrix3 :: data () const
{
    const float* temp = &m_right.m_x;
    return temp;
}

void
Matrix3 :: setRight (const Vector3& right)
{
    m_right = right;
}

Vector3
Matrix3 :: getRight () const
{
    return m_right;
}

void
Matrix3 :: setUp (const Vector3& up)
{
    m_up = up;
}

Vector3
Matrix3 :: getUp () const
{
    return m_up;
}

void
Matrix3 :: setBack (const Vector3& back)
{
    m_back = back;
}

Vector3
Matrix3 :: getBack () const
{
    return m_back;
}

void
Matrix3 :: setForward (const Vector3& forward)
{
    m_back.set((-1 * forward.m_x), (-1 * forward.m_y), (-1 * forward.m_z));
}

Vector3
Matrix3 :: getForward () const
{
    return -m_back;
}

void
Matrix3 :: invertRotation ()
{
    transpose();
}

void
Matrix3 :: invert ()
{
    float rx = (m_up.m_y * m_back.m_z) - (m_back.m_y * m_up.m_z);
    float ry = (m_back.m_y * m_right.m_z) - (m_right.m_y * m_back.m_z);
    float rz = (m_right.m_y * m_up.m_z) - (m_up.m_y * m_right.m_z);

    float ux = (m_back.m_x * m_up.m_z) - (m_up.m_x * m_back.m_z);
    float uy = (m_right.m_x * m_back.m_z) - (m_back.m_x * m_right.m_z);
    float uz = (m_up.m_x * m_right.m_z) - (m_right.m_x * m_up.m_z);

    float bx = (m_up.m_x * m_back.m_y) - (m_back.m_x * m_up.m_y);
    float by = (m_back.m_x * m_right.m_y) - (m_right.m_x * m_back.m_y);
    float bz = (m_right.m_x * m_up.m_y) - (m_up.m_x * m_right.m_y);

    m_right.set (rx, ry, rz);
    m_up.set (ux, uy, uz);
    m_back.set (bx, by, bz);
    *this *= 1.0f / determinant();
}

float
Matrix3 :: determinant() const
{
    float temp1 = (m_right.m_x * ((m_up.m_y * m_back.m_z) - (m_up.m_z * m_back.m_y)));
    float temp2 = (m_right.m_y * ((m_up.m_x * m_back.m_z) - (m_up.m_z * m_back.m_x)));
    float temp3 = (m_right.m_z * ((m_up.m_x * m_back.m_y) - (m_up.m_y * m_back.m_x)));
    float temp4 = temp1 - temp2 + temp3;
    return temp4;
}

void
Matrix3 :: transpose ()
{
    Vector3 tempR = m_right;
    Vector3 tempU = m_up;
    Vector3 tempB = m_back;
    Vector3 newRight(tempR.m_x, tempU.m_x, tempB.m_x);
    Vector3 newUp(tempR.m_y, tempU.m_y, tempB.m_y);
    Vector3 newBack(tempR.m_z, tempU.m_z, tempB.m_z);
    m_right = newRight;
    m_up = newUp;
    m_back = newBack;
}

void
Matrix3 :: orthonormalize ()
{
    m_back.normalize();
    m_right = m_up.cross(m_back);
    m_right.normalize();
    m_up = m_back.cross(m_right);
    m_up.normalize();
}

void
Matrix3 :: setToScale (float scale)
{
    m_right.m_x = scale;
    m_right.m_y = 0.0f;
    m_right.m_z = 0.0f;
    m_up.m_x = 0.0f;
    m_up.m_y = scale;
    m_up.m_z = 0.0f;
    m_back.m_x = 0.0f;
    m_back.m_y = 0.0f;
    m_back.m_z = scale;
}

void
Matrix3 :: setToScale (float scaleX, float scaleY, float scaleZ)
{
    m_right.m_x = scaleX;
    m_right.m_y = 0.0f;
    m_right.m_z = 0.0f;
    m_up.m_x = 0.0f;
    m_up.m_y = scaleY;
    m_up.m_z = 0.0f;
    m_back.m_x = 0.0f;
    m_back.m_y = 0.0f;
    m_back.m_z = scaleZ;
}

void
Matrix3 :: setToShearXByYz (float shearY, float shearZ)
{
    m_right.m_x = 1.0f;
    m_right.m_y = 0.0f;
    m_right.m_z = 0.0f;
    m_up.m_x = shearY;
    m_up.m_y = 1.0f;
    m_up.m_z = 0.0f;
    m_back.m_x = shearZ;
    m_back.m_y = 0.0f;
    m_back.m_z = 1.0f;
}

void
Matrix3 :: setToShearYByXz (float shearX, float shearZ)
{
    m_right.m_x = 1.0f;
    m_right.m_y = shearX;
    m_right.m_z = 0.0f;
    m_up.m_x = 0.0f;
    m_up.m_y = 1.0f;
    m_up.m_z = 0.0f;
    m_back.m_x = 0.0f;
    m_back.m_y = shearZ;
    m_back.m_z = 1.0f;
}

void
Matrix3 :: setToShearZByXy (float shearX, float shearY)
{
    m_right.m_x = 1.0f;
    m_right.m_y = 0.0f;
    m_right.m_z = shearX;
    m_up.m_x = 0.0f;
    m_up.m_y = 1.0f;
    m_up.m_z = shearY;
    m_back.m_x = 0.0f;
    m_back.m_y = 0.0f;
    m_back.m_z = 1.0f;
}

void
Matrix3 :: setToRotationX (float angleDegreees)
{
    m_right.m_x = 1.0f;
    m_right.m_y = 0.0f;
    m_right.m_z = 0.0f;
    m_up.m_x = 0.0f;
    m_up.m_y = cos(toRadians(angleDegreees));
    m_up.m_z = sin(toRadians(angleDegreees));
    m_back.m_x = 0.0f;
    m_back.m_y = -sin(toRadians(angleDegreees));
    m_back.m_z = cos(toRadians(angleDegreees));
}

void
Matrix3 :: setToRotationY (float angleDegrees)
{
    m_right.m_x = cos(toRadians(angleDegrees));
    m_right.m_y = 0.0f;
    m_right.m_z = -sin(toRadians(angleDegrees));
    m_up.m_x = 0.0f;
    m_up.m_y = 1.0f;
    m_up.m_z = 0.0f;
    m_back.m_x = sin(toRadians(angleDegrees));
    m_back.m_y = 0.0f;
    m_back.m_z = cos(toRadians(angleDegrees));
}

void
Matrix3 :: setToRotationZ (float angleDegrees)
{
    m_right.m_x = cos(toRadians(angleDegrees));
    m_right.m_y = sin(toRadians(angleDegrees));
    m_right.m_z = 0.0;
    m_up.m_x = -sin(toRadians(angleDegrees));
    m_up.m_y = cos(toRadians(angleDegrees));
    m_up.m_z = 0.0f;
    m_back.m_x = 0.0f;
    m_back.m_y = 0.0f;
    m_back.m_z = 1.0f;
}

void
Matrix3 :: setFromAngleAxis (float angleDegrees, const  Vector3& axis)
{
    m_right.m_x = ((axis.m_x * axis.m_x) + (cos(toRadians(angleDegrees)) * (1-(axis.m_x * axis.m_x))));
    m_right.m_y = ((axis.m_x * axis.m_y) + (1 - cos(angleDegrees) - (axis.m_z * sin(toRadians(angleDegrees)))));
    m_right.m_z = ((axis.m_x * axis.m_z) * (1 - cos(toRadians(angleDegrees) + (axis.m_y * sin(toRadians(angleDegrees))))));
    m_up.m_x = ((axis.m_x * axis.m_y) * (1 - cos(toRadians(angleDegrees) + (axis.m_z * sin(toRadians(angleDegrees))))));
    m_up.m_y = ((axis.m_y * axis.m_y) + (cos(toRadians(angleDegrees) * (1 - (axis.m_y * axis.m_y)))));
    m_up.m_z = ((axis.m_y * axis.m_z) * (1 - cos(toRadians(angleDegrees)) - (axis.m_x *(sin(toRadians(angleDegrees))))));
    m_back.m_x = (((axis.m_x * axis.m_z) * (1 - cos(toRadians(angleDegrees)))) - (axis.m_y * sin(toRadians(angleDegrees))));
    m_back.m_y = (((axis.m_y * axis.m_z) * (1 - cos(toRadians(angleDegrees)) + (axis.m_x * sin(toRadians(angleDegrees))))));
    m_back.m_z = ((axis.m_z * axis.m_z) + (cos(toRadians(angleDegrees) * (1 - (axis.m_z * axis.m_z)))));
}

void
Matrix3 :: negate ()
{
    m_right.negate();
    m_up.negate();
    m_back.negate();
}

Vector3 
Matrix3 :: transform (const Vector3& v) const
{
    Vector3 temp;
    temp = v;
    return v; 
}

Matrix3&
Matrix3 :: operator+= (const Matrix3& m)
{
    m_right = m_right + m.m_right;
    m_up = m_up + m.m_up;
    m_back = m_back + m.m_back;
    return *this;
}

Matrix3&
Matrix3 :: operator-= (const Matrix3& m)
{
    m_right = m_right - m.m_right;
    m_up = m_up - m.m_up;
    m_back = m_back - m.m_back;
    return *this;
}

Matrix3&
Matrix3 :: operator*= (float scalar)
{
    m_right = m_right * scalar;
    m_up = m_up * scalar;
    m_back = m_back * scalar;
    return *this;
}

Matrix3&
Matrix3 :: operator*= (const Matrix3& m)
{
    Vector3 tRight = m.getRight();
    Vector3 tUp = m.getUp();
    Vector3 tBack = m.getBack();

    float rightX = (m_right.m_x * tRight.m_x) + (m_up.m_x * tRight.m_y) + (m_back.m_x * tRight.m_z);
    float rightY = (m_right.m_y * tRight.m_x) + (m_up.m_y * tRight.m_y) + (m_back.m_y * tRight.m_z);
    float rightZ = (m_right.m_z * tRight.m_x) + (m_up.m_z * tRight.m_y) + (m_back.m_z * tRight.m_z);

    float upX = (m_right.m_x * tUp.m_x) + (m_up.m_x * tUp.m_y) + (m_back.m_x * tUp.m_z);
    float upY = (m_right.m_y * tUp.m_x) + (m_up.m_y * tUp.m_y) + (m_back.m_y * tUp.m_z);
    float upZ = (m_right.m_z * tUp.m_x) + (m_up.m_z * tUp.m_y) + (m_back.m_z * tUp.m_z);

    float backX = (m_right.m_x * tBack.m_x) + (m_up.m_x * tBack.m_y) + (m_back.m_x * tBack.m_z);
    float backY = (m_right.m_y * tBack.m_x) + (m_up.m_y * tBack.m_y) + (m_back.m_y * tBack.m_z);
    float backZ = (m_right.m_z * tBack.m_x) + (m_up.m_z * tBack.m_y) + (m_back.m_z * tBack.m_z);

    m_right.set(rightX, rightY, rightZ);
    m_up.set(upX, upY, upZ);
    m_back.set(backX, backY, backZ);
    
    return *this;
}

Matrix3
operator+ (const Matrix3& m1, const Matrix3& m2)
{
    Vector3 m1R = m1.getRight();
    Vector3 m1U = m1.getUp();
    Vector3 m1B = m1.getBack();
    Vector3 m2R = m2.getRight();
    Vector3 m2U = m2.getUp();
    Vector3 m2B = m2.getBack();

    Vector3 rVec ((m1R.m_x + m2R.m_x), (m1R.m_y + m2R.m_y), (m1R.m_z + m2R.m_z));
    Vector3 uVec ((m1U.m_x + m2U.m_x), (m1U.m_y + m2U.m_y), (m1U.m_z + m2U.m_z));
    Vector3 bVec ((m1B.m_x + m2B.m_x), (m1B.m_y + m2B.m_y), (m1B.m_z + m2B.m_z));

    Matrix3 addedMatrix (rVec, uVec, bVec);

    return addedMatrix;
}

Matrix3
operator- (const Matrix3& m1, const Matrix3& m2)
{
    Matrix3 temp;
    temp = m1;
    temp -= m2;
    return temp;
}

Matrix3
operator- (const Matrix3& m)
{
    Matrix3 temp;
    temp = m;
    temp *= -1;
    return temp;
}

Matrix3
operator* (const Matrix3& m, float scalar)
{
    Matrix3 temp;
    temp = m;
    temp *= scalar;
    return temp;
}

Matrix3
operator* (float scalar, const Matrix3& m)
{
    Matrix3 temp;
    temp = m;
    temp *= scalar;
    return temp;
}

Matrix3
operator* (const Matrix3& m1, const Matrix3& m2)
{
    Matrix3 temp = m1;
    temp *= m2;
    return temp;
}

Vector3
operator* (const Matrix3& m, const Vector3& v)
{
    Vector3 deez1 = m.getRight();
    Vector3 deez2 = m.getUp();
    Vector3 deez3 = m.getBack();

    float x = (deez1.m_x * v.m_x) + (deez2.m_x * v.m_y) + (deez3.m_x * v.m_z); 
    float y = (deez1.m_y * v.m_x) + (deez2.m_y * v.m_y) + (deez3.m_y * v.m_z);
    float z = (deez1.m_z * v.m_x) + (deez2.m_z * v.m_y) + (deez3.m_z * v.m_z);
    
    Vector3 temp (x, y, z);

    return temp;
}

std::ostream&
operator<< (std::ostream& out, const Matrix3& m)
{
    Vector3 tempR = m.getRight();
    Vector3 tempU = m.getUp();
    Vector3 tempB = m.getBack();

    out << std::fixed << std::setw( 10 ) << std::setprecision( 2 ) << tempR.m_x << " " << tempU.m_x << " " << tempB.m_x;

    out << std::fixed << std::setw( 10 ) << std::setprecision( 2 ) << tempR.m_y << " " << tempU.m_y << " " << tempB.m_y;

    out << std::fixed << std::setw( 10 ) << std::setprecision( 2 ) << tempR.m_z << " " << tempU.m_z << " " << tempB.m_z;

    return out;
}

bool
operator== (const Matrix3& m1, const Matrix3& m2)
{
    Vector3 mR1 = m1.getRight();
    Vector3 mU1 = m1.getUp();
    Vector3 mB1 = m1.getBack();
    Vector3 mR2 = m2.getRight();
    Vector3 mU2 = m2.getUp();
    Vector3 mB2 = m2.getBack();

    float epsilon = 0.00001f;
    bool rxComp = fabs(mR1.m_x - mR2.m_x) < epsilon;
    bool ryComp = fabs(mR1.m_y - mR2.m_y) < epsilon;
    bool rzComp = fabs(mR1.m_z - mR2.m_z) < epsilon;
    bool uxComp = fabs(mU1.m_x - mU2.m_x) < epsilon;
    bool uyComp = fabs(mU1.m_y - mU2.m_y) < epsilon;
    bool uzComp = fabs(mU1.m_z - mU2.m_z) < epsilon;
    bool bxComp = fabs(mB1.m_x - mB2.m_x) < epsilon;
    bool byComp = fabs(mB1.m_y - mB2.m_y) < epsilon;
    bool bzComp = fabs(mB1.m_z - mB2.m_z) < epsilon;

    if (rxComp && ryComp && rzComp && uxComp && uyComp && uzComp && bxComp && byComp && bzComp) 
    {
        return true;
    }
    else
    {
        return false;
    }
}