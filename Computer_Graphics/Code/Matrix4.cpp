/// \file Matrix4.cpp
/// \brief Class file for Matrix4 
/// \author Evan Hanzelman
/// \version A07

#include <cmath>
#include <iostream>
#include <iomanip>

#include "Matrix4.hpp"

Matrix4 :: Matrix4 ()
{
	setToIdentity();
}

Matrix4 :: Matrix4 (const Vector4& right, const Vector4& up,
    const Vector4& back, const Vector4& translation)
{
	m_right = right;
	m_up = up;
	m_back = back;
	m_translation = translation;
}

Vector4
Matrix4 :: getRight () const
{
	return m_right;
}

Vector4
Matrix4 :: getUp () const
{
	return m_up;
}

Vector4
Matrix4 :: getBack () const
{
	return m_back;
}

Vector4 
Matrix4 :: getTranslation () const
{
	return m_translation;
}

void
Matrix4 :: setToIdentity ()
{
	m_right.m_w = 1.0f;
	m_right.m_x = 0.0f;
	m_right.m_y = 0.0f;
	m_right.m_z = 0.0f;
	m_up.m_w = 0.0f;
	m_up.m_x = 1.0f;
	m_up.m_x = 0.0f;
	m_up.m_y = 0.0f;
	m_up.m_z = 0.0f;
	m_back.m_w = 0.0f;
	m_back.m_x = 0.0f;
	m_back.m_y = 1.0f;
	m_back.m_z = 0.0f;
	m_translation.m_w = 0.0f;
	m_translation.m_x = 0.0f;
	m_translation.m_y = 0.0f;
	m_translation.m_z = 1.0f;
}

void
Matrix4 :: setToZero ()
{
	m_right.m_w = 0.0f;
	m_right.m_x = 0.0f;
	m_right.m_y = 0.0f;
	m_right.m_z = 0.0f;
	m_up.m_w = 0.0f;
	m_up.m_x = 0.0f;
	m_up.m_y = 0.0f;
	m_up.m_z = 0.0f;
	m_back.m_w = 0.0f;
	m_back.m_x = 0.0f;
	m_back.m_y = 0.0f;
	m_back.m_z = 0.0f;
	m_translation.m_w = 0.0f;
	m_translation.m_x = 0.0f;
	m_translation.m_y = 0.0f;
	m_translation.m_z = 0.0f;
}

const float*
Matrix4 :: data () const
{
	const float* temp = &m_right.m_x;
	return temp;
}

//Symmetric
void
Matrix4 :: setToPerspectiveProjection (double fovYDegrees, double aspectRatio,
			      double nearPlaneZ, double farPlaneZ)
{
	m_right.m_w = (1 / (aspectRatio * tan (fovYDegrees / 2)));
	m_right.m_x = 0.0f;
	m_right.m_y = 0.0f;
	m_right.m_z = 0.0f;
	m_up.m_w = 0.0f;
	m_up.m_x = (1 / (tan (fovYDegrees / 2)));
	m_up.m_y = 0.0f;
	m_up.m_z = 0.0f;
	m_back.m_w = 0.0f;
	m_back.m_x = 0.0f;
	m_back.m_y = ((nearPlaneZ + farPlaneZ) / (nearPlaneZ - farPlaneZ));
	m_back.m_z = -1.0f;
	m_translation.m_w = 0.0f;
	m_translation.m_x = 0.0f;
	m_translation.m_y = ((2 * (farPlaneZ * nearPlaneZ)) / (nearPlaneZ - farPlaneZ));
	m_translation.m_z = 0.0f;
}

//Assymetric
void
Matrix4 :: setToPerspectiveProjection (double left, double right,
			      double bottom, double top,
			      double nearPlaneZ, double farPlaneZ)
{
	m_right.m_w = ((2 * nearPlaneZ) / (right - left));
	m_right.m_x = 0.0f;
	m_right.m_y = 0.0f;
	m_right.m_z = 0.0f;
	m_up.m_w = 0.0f;
	m_up.m_x = ((2 * nearPlaneZ) / (top - bottom));
	m_up.m_y = 0.0f;
	m_up.m_z = 0.0f;
	m_back.m_w = ((right + left) / (right - left));
	m_back.m_x = ((top + bottom) / (top - bottom));
	m_back.m_y = ((nearPlaneZ + farPlaneZ) / (nearPlaneZ - farPlaneZ));
	m_back.m_z = -1.0f;
	m_translation.m_w = 0.0f;
	m_translation.m_x = 0.0f;
	m_translation.m_y = ((2 * farPlaneZ * nearPlaneZ) / (nearPlaneZ - farPlaneZ));
	m_translation.m_z = 0.0f;
}

void
Matrix4 :: setToOrthographicProjection (double left, double right,
			       double bottom, double top,
			       double nearPlaneZ, double farPlaneZ)
{
	m_right.m_w = ((2) / (right - left));
	m_right.m_x = 0.0f;
	m_right.m_y = 0.0f;
	m_right.m_z = 0.0f;
	m_up.m_w = 0.0f;
	m_up.m_x = ((2) / (top - bottom));
	m_up.m_y = 0.0f;
	m_up.m_z = 0.0f;
	m_back.m_w = 0.0f;
	m_back.m_x = 0.0f;
	m_back.m_y = ((2) / (nearPlaneZ - farPlaneZ));
	m_back.m_z = 0.0f;
	m_translation.m_w = -((right + left) / (right - left));
	m_translation.m_x = -((top + bottom) / (top - bottom));
	m_translation.m_y = -((nearPlaneZ + farPlaneZ) / (nearPlaneZ - farPlaneZ));
	m_translation.m_z = 1.0f;
}

std::ostream&
operator<< (std::ostream& out, const Matrix4& m)
{
	Vector4 tempR = m.getRight();
	Vector4 tempU = m.getUp();
	Vector4 tempB = m.getBack();
	Vector4 tempT = m.getTranslation();

	out << std::fixed << std::setw( 10 ) << std::setprecision( 2 ) << tempR.m_w << " " << tempU.m_w << " " << tempB.m_w << " " << tempT.m_w;

	out << std::fixed << std::setw( 10 ) << std::setprecision( 2 ) << tempR.m_x << " " << tempU.m_x << " " << tempB.m_x << " " << tempT.m_x;

    out << std::fixed << std::setw( 10 ) << std::setprecision( 2 ) << tempR.m_y << " " << tempU.m_y << " " << tempB.m_y << " " << tempT.m_y;

    out << std::fixed << std::setw( 10 ) << std::setprecision( 2 ) << tempR.m_z << " " << tempU.m_z << " " << tempB.m_z << " " << tempT.m_z;
}

bool
operator== (const Matrix4& m1, const Matrix4& m2)
{
	Vector4 mR1 = m1.getRight();
	Vector4 mU1 = m1.getUp();
	Vector4 mB1 = m1.getBack();
	Vector4 mT1 = m1.getTranslation();

	Vector4 mR2 = m2.getRight();
	Vector4 mU2 = m2.getUp();
	Vector4 mB2 = m2.getBack();
	Vector4 mT2 = m2.getTranslation();

	float epsilon = 0.00001f;

	bool twComp = fabs(mT1.m_w - mT2.m_w) < epsilon;
	bool txComp = fabs(mT1.m_x - mT2.m_x) < epsilon;
	bool tyComp = fabs(mT1.m_y - mT2.m_y) < epsilon;
	bool tzComp = fabs(mT1.m_z - mT2.m_z) < epsilon;

	bool rwComp = fabs(mR1.m_w - mR2.m_w) < epsilon;
	bool rxComp = fabs(mR1.m_x - mR2.m_x) < epsilon;
	bool ryComp = fabs(mR1.m_y - mR2.m_y) < epsilon;
	bool rzComp = fabs(mR1.m_z - mR2.m_z) < epsilon;

	bool uwComp = fabs(mU1.m_w - mU2.m_w) < epsilon;
	bool uxComp = fabs(mU1.m_x - mU2.m_x) < epsilon;
	bool uyComp = fabs(mU1.m_y - mU2.m_y) < epsilon;
	bool uzComp = fabs(mU1.m_z - mU2.m_z) < epsilon;

	bool bwComp = fabs(mB1.m_w - mB2.m_w) < epsilon;
	bool bxComp = fabs(mB1.m_x - mB2.m_x) < epsilon;
	bool byComp = fabs(mB1.m_y - mB2.m_y) < epsilon;
	bool bzComp = fabs(mB1.m_z - mB2.m_z) < epsilon;

	if (twComp && txComp && tyComp && tzComp && rwComp && rxComp && ryComp && rzComp && uwComp && uxComp && uyComp && uzComp && bwComp && bxComp && byComp && bzComp)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}