/*
 Brief: Cpp for Camera class
 Author: Evan Hanzelman
 Version: A03
*/

#include "Camera.hpp"
#include "Vector3.hpp"
#include "Matrix3.hpp"
#include "Transform.hpp"

#include <glm/vec3.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/mat4x4.hpp>
#include "OpenGLContext.hpp"

/*
    Constructor: assigning parameters to class private variables
    Use cross products to set direction variables
*/
Camera :: Camera (const Vector3& eyePosition, const Vector3& localBackDirection,
	  float nearClipPlaneDistance, float farClipPlaneDistance,
	  float aspectRatio, float verticalFieldOfViewDegrees)
{
    // Set position and projection
    setPosition(eyePosition);
    setProjectionSymmetricPerspective(verticalFieldOfViewDegrees, aspectRatio,
            nearClipPlaneDistance, farClipPlaneDistance);

    // Make duplicates of eye position and local back for restoration
    m_originalPosition = eyePosition;
    m_ogLocalBackPosition = localBackDirection;

    // Calculate local right and up
    // Temp hard code of local up so we can use cross product to get orthogonal vector from localBack
    Vector3 localUp (0, 1, 0);
    Vector3 localRight = localUp.cross(localBackDirection);
    localUp = localBackDirection.cross(localRight);

    // Set the transform to the vectors
    m_world.setOrientation(localRight, localUp, localBackDirection);
    m_world.orthonormalize();
}

/*
    Deconstructor
*/
Camera :: ~Camera ()
{

}

/*
    Sets the eye position to parameter value and viewMatrix to true
*/
void
Camera :: setPosition (const Vector3& position)
{
    viewMatrixChanged = true;
    m_eyePosition = position;
}

/*
    Sets position of camera depending on the move function
*/
void
Camera :: moveRight (float distance)
{
    m_world.setPosition(m_world.getPosition() + (m_world.getRight() * distance));
    viewMatrixChanged = true;
}

void
Camera :: moveUp (float distance)
{
    m_world.setPosition(m_world.getPosition() + (m_world.getUp() * distance));
    viewMatrixChanged = true;
}

void
Camera :: moveBack (float distance)
{
    m_world.setPosition(m_world.getPosition() + (m_world.getBack() * distance));
    viewMatrixChanged = true;
}

/*
    Rotates camera by radians rather than the given float in degrees
    Camera rotates on its access using glm::rotate function
*/
void 
Camera :: yaw (float degrees)
{
    m_world.yaw (degrees);
    viewMatrixChanged = true;
    projectionMatrixChanged = true;
}

void
Camera :: pitch (float degrees)
{
    m_world.yaw (degrees);
    viewMatrixChanged = true;
    projectionMatrixChanged = true;
}

void
Camera :: roll (float degrees)
{
    m_world.yaw (degrees);
    viewMatrixChanged = true;
    projectionMatrixChanged = true;
}

/*
    Returns bool of m_viewMatrixChanged checking for the viewMatrix
*/
Transform
Camera :: getViewMatrix ()
{
    if(viewMatrixChanged == true)
    {
        t_viewMatrix.setOrientation(m_world.getOrientation());
        t_viewMatrix.setPosition(m_world.getPosition());
        t_viewMatrix.invertRt ();
    }

    return t_viewMatrix;
}

/*
    sets projection values of near, far, aspect, and FOV
    and sets bool value to true
*/
void
Camera :: setProjectionSymmetricPerspective (double verticalFovDegrees, double aspectRatio,
double nearZ, double farZ)
{
    m_verticalFieldOfViewDegrees = verticalFovDegrees;
    m_aspectRatio = aspectRatio;
    m_nearClipPlaneDistance = nearZ;
    m_farClipPlaneDistance = farZ;
    projectionMatrixChanged = true;
    m_projectionMatrix.setToPerspectiveProjection (verticalFovDegrees, aspectRatio, nearZ, farZ);
}

void
Camera :: setProjectionAssymetricPerspective (double left, double right, 
                                              double bottom, double top,
                                              double near, double far)
{
    m_projectionMatrix.setToPerspectiveProjection (left, right, bottom, top, near, far);
}

void
Camera :: setProjectionOrthographic (double left, double right,
                                     double bottom, double top,
                                     double near, double far)
{
    m_projectionMatrix.setToOrthographicProjection (left, right, bottom, top, near, far);
}

/*
    returns projectionmatrix variable 
    if projectionmatrixchanged is true perspective is set
*/
Matrix4
Camera :: getProjectionMatrix ()
{
    return m_projectionMatrix;
}

/*
    resets the position of the camera to its initial spot
*/
void
Camera :: resetPose ()
{
    m_camMatrix.setBack(m_ogLocalBackPosition);
    m_world.setPosition(m_originalPosition);
    Vector3 temp (0, 1, 0);
    m_camMatrix.setUp(temp);
    m_camMatrix.setRight(m_camMatrix.getUp().cross(m_camMatrix.getBack()));
    m_camMatrix.setUp(m_camMatrix.getBack().cross(m_camMatrix.getRight()));
    m_camMatrix.orthonormalize();
    m_world.setOrientation(m_camMatrix);
}