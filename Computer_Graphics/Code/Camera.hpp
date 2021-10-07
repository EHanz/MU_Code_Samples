/// \file Camera.hpp
/// \brief Declaration of Camera class and any associated global functions.
/// \author Chad Hogg, Evan Hanzelman
/// \version A03

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "OpenGLContext.hpp"
#include "Vector3.hpp"
#include "Matrix3.hpp"
#include "Transform.hpp"
#include "Matrix4.hpp"

/// \brief An eye that is viewing the scene.
class Camera
{
public:

  /// \brief Constructs a new Camera.
  ///
  /// \param eyePosition The location (in world coordinates) where the camera
  ///   is located.
  /// \param localBackDirection A vector pointing backwards from the camera.
  /// \param nearClipPlaneDistance The distance to the closest things the
  ///   camera can "see".
  /// \param farClipPlaneDistance The distance to the farthest things the
  ///   camera can "see".
  /// \param aspectRatio The window's width divided by height.
  /// \param verticalFieldOfViewDegrees The angle determining how much the
  ///   camera "sees".
  /// \post All of the parameters have been copied into their respective data
  ///   members, and the local right and up have been calculated to be
  ///   orthonormal to the local back.
  Camera (const Vector3& eyePosition, const Vector3& localBackDirection,
	  float nearClipPlaneDistance, float farClipPlaneDistance,
	  float aspectRatio, float verticalFieldOfViewDegrees);

  /// \brief Destructs a Camera.
  ~Camera ();
  
  /// \brief Sets the position (eye point) of the camera.
  /// \param[in] position The new position of the camera.
  /// \post The camera's location has been changed.
  void
  setPosition (const Vector3& position);

  /// \brief Moves the position (eye point) of the camera right or left.
  /// \param[in] distance How far to move along the right vector.
  /// \post The camera's location has been changed.
  void
  moveRight (float distance);

  /// \brief Moves the position (eye point) of the camera up or down.
  /// \param[in] distance How far to move along the up vector.
  /// \post The camera's location has been changed.
  void
  moveUp (float distance);

  /// \brief Moves the position (eye point) of the camera back or forward.
  /// \param[in] distance How far to move along the back vector.
  /// \post The camera's location has been changed.
  void
  moveBack (float distance);

  /// \brief Rotates the camera counterclockwise around the up vector.
  /// \param[in] degrees How far to rotate.
  /// \post The camera's orientation has been changed.
  void
  yaw (float degrees);

  void
  pitch(float degrees);

  void
  roll(float degrees);

  /// \brief Gets the view matrix, recalculating it only if necessary.
  /// \return A view matrix based on the camera's location and axis vectors.
  Transform
  getViewMatrix ();

  /// \brief Sets the values needed to specify a projection.
  /// \param[in] verticalFovDegrees The viewing angle.
  /// \param[in] aspectRatio The width / height.
  /// \param[in] nearZ The distance to closest visible objects.
  /// \param[in] farZ The distance to furthest visible objects.
  void
  setProjectionSymmetricPerspective (double verticalFovDegrees, double aspectRatio,
		 double nearZ, double farZ); 

  void
  setProjectionAssymetricPerspective (double left, double right,
                                      double bottom, double top,
                                      double near, double far);

  void
  setProjectionOrthographic (double left, double right,
                             double bottom, double top,
                             double near, double far);

  /// \brief Gets the projection matrix, recalculating it only if necessary.
  /// \return A projection matrix based on the vertical field of view,
  ///   aspect ratio, and near and far planes.
  Matrix4
  getProjectionMatrix ();

  /// \brief Resets the camera to its original pose.
  /// \post The position (eye point) is the same as what had been specified in
  ///   the constructor.
  /// \post The back vector is the same as what had been specified in the
  ///   constructor.
  /// \post The up and right vectors are computed as they had been in the
  ///   constructor.
  void
  resetPose ();

private:

  /// The location of the camera.
  Vector3 m_eyePosition;
  /// A vector pointing back from the camera.
  Vector3 m_localBackDirection;
  /// A vector pointing right from the camera.
  Vector3 m_localRightDirection;
  /// A vector pointing up from the camera.
  Vector3 m_localUpDirection;
  
  //Added data members fpr resetting pose plus two booleans
  Vector3 m_originalPosition;
  Vector3 m_ogLocalBackPosition;

  Matrix3 m_camMatrix;

  Transform m_world;
  Transform t_viewMatrix;

  bool viewMatrixChanged;
  bool projectionMatrixChanged;

  /// The distance to the closest visible objects.
  float m_nearClipPlaneDistance;
  /// The distance to the furthest visible objects.
  float m_farClipPlaneDistance;
  /// The width width / height;
  float m_aspectRatio;
  /// The viewing angle.
  float m_verticalFieldOfViewDegrees;

  /// The projection matrix.
  Matrix4 m_projectionMatrix;
  /// The view matrix.
  Matrix4 m_viewMatrix;

};

#endif//CAMERA_HPP