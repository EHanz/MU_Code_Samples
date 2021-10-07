/// \file Matrix4.hpp
/// \brief Declaration of Matrix4 class and any associated global functions.
/// \author Gary M. Zoppetti, Ph.D. & Chad Hogg
/// \version A07

#ifndef MATRIX4_HPP
#define MATRIX4_HPP

// For overload of shift operator.
#include <iostream>

// Local includes.
#include "Vector4.hpp"

/// \brief A 4x4 matrix of floats.
/// Basis vectors (right, up, back, and translate) are stored in Vector4s and
///   form the columns of a 4x4 matrix.
/// The matrix is interpreted thus:
/// [ rx ux bx tx ]
/// [ ry uy by ty ]
/// [ rz uz bz tz ]
/// [ rw uw bw tw ]
/// Operations are consistent with column vectors (v' = M * v).
/// If the last row contains [ 0 0 0 1 ] the transform is affine; otherwise it
///    is projective.
class Matrix4
{
public:
  /// \brief Initializes to the identity matrix.
  /// \post rx, uy, bz, and tw are 1.0f while all other elements are 0.0f.
  Matrix4 ();

  /// \brief Initializes a ne matrix from the four basis vectors.
  /// \param[in] right The first column of the new matrix.
  /// \param[in] up The second column of the new matrix.
  /// \param[in] back The third column of the new matrix.
  /// \param[in] translation The fourth column of the new matrix.
  Matrix4 (const Vector4& right, const Vector4& up,
	   const Vector4& back, const Vector4& translation);

  /// \brief Gets the right vector.
  /// \return A copy of the first column.
  Vector4
  getRight () const;

  /// \brief Gets the up vector.
  /// \return A copy of the second column.
  Vector4
  getUp () const;

  /// \brief Gets the back vector.
  /// \return A copy of the third column.
  Vector4
  getBack () const;

  /// \brief Gets the translation vector.
  /// \return A copy of the fourth column.
  Vector4
  getTranslation () const;

  /// \brief Sets this to the identity matrix.
  /// \post The elements along the main diagonal are 1.0f while all others are
  ///   0.0f.
  void
  setToIdentity ();

  /// \brief Sets this to the zero matrix.
  /// \post All elements are 0.0f.
  void
  setToZero ();
    
  /// \brief Gets a const pointer to the first element.
  /// \return A pointer to rx.
  const float*
  data () const;

  // For the projection methods, do all computations using
  //   double-s and only cast to float when NECESSARY. 

  /// \brief Sets this to a symmetric perspective projection matrix.
  /// \param[in] fovYDegrees The vertical field of view angle, in degrees.
  /// \param[in] aspectRatio The ratio of width to height.
  /// \param[in] nearPlaneZ The distance to the near clip plane.
  /// \param[in] farPlaneZ The distance to the far clip plane.
  /// \post This has been set to a symmetric perspective projection.
  void
  setToPerspectiveProjection (double fovYDegrees, double aspectRatio,
			      double nearPlaneZ, double farPlaneZ);

  /// \brief Sets this to an assymetric perspective projection matrix.
  /// \param[in] left The x-value of the left side of the projection plane.
  /// \param[in] right The x-value of the right side of the projection plane.
  /// \param[in] bottom The y-value of the bottom of the projection plane.
  /// \param[in] top The y-value of the top of the projection plane.
  /// \param[in] nearPlaneZ The distance to the near clip plane.
  /// \param[in] farPlaneZ The distance to the far clip plane.
  /// \post This has been set to an asymmetric perspective projection.
  void
  setToPerspectiveProjection (double left, double right,
			      double bottom, double top,
			      double nearPlaneZ, double farPlaneZ);

  /// \brief Sets this to an orthographic projection matrix.
  /// \param[in] left The x-value of the left plane.
  /// \param[in] right The x-value of the right plane.
  /// \param[in] bottom The y-value of the bottom plane.
  /// \param[in] top The y-value of the top plane.
  /// \param[in] nearPlaneZ The distance to the near clip plane.
  /// \param[in] farPlaneZ The distance to the far clip plane.
  /// \post This has been set to an orthographic projection.
  void
  setToOrthographicProjection (double left, double right,
			       double bottom, double top,
			       double nearPlaneZ, double farPlaneZ);
  
private:
  /// \brief The first column of the matrix.
  Vector4 m_right;
  /// \brief The second column of the matrix.
  Vector4 m_up;
  /// \brief The third column of the matrix.
  Vector4 m_back;
  /// \brief The fourth column of the matrix.
  Vector4 m_translation;
};

/// \brief Inserts a matrix into an output stream.
/// Each element of the matrix should have 2 digits of precision and a field
///   width of 10.  Elements should be in this order:
///     rx ux bx tx
///     ry uy by ty
///     rz uz bz tz
///     rw uw bw tw
/// \param[inout] out An output stream.
/// \param[in] m A matrix.
/// \return The output stream.
/// \post The matrix has been inserted into the output stream.
std::ostream&
operator<< (std::ostream& out, const Matrix4& m);

/// \brief Checks whether or not two matrices are equal.
/// Matrices are equal if each of their respective elements are within
///   0.00001f of each other due to floating-point imprecision.
/// \param[in] m1 A matrix.
/// \param[in] m2 Another matrix.
/// \return Whether or not m1 and m2 are equal.
bool
operator== (const Matrix4& m1, const Matrix4& m2);

#endif//MATRIX4_HPP
