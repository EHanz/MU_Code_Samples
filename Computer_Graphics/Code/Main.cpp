/// \file Main.cpp
/// \brief Declaration of main function, helper functions, and helper classes.
/// \author Gary M. Zopetti, Chad Hogg, Evan Hanzelman
/// \version A02
///
/// This is a fairly simple program that uses OpenGL 3.3 to draw a scene.  It
///   allows limited movement of a very simple virtual camera.

/******************************************************************/
// System includes
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>

// Use GLEW so we can access the latest OpenGL functionality
// Always include GLEW before GLFW!
#include <GL/glew.h>
// Use GLFW so we can create windows and handle events in a
//   platform-independent way
#include <GLFW/glfw3.h>

// GLM is a header-only library, so we do not need to link to it
// It provides matrix classes and mathematical functions
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
// Need for "value_ptr", which allows us to get a pointer from a matrix
#include <glm/gtc/type_ptr.hpp>
// Transforms like lookat, perspective, rotate, scale, translate
#include <glm/gtc/matrix_transform.hpp>


/******************************************************************/
// Local includes
#include "RealOpenGLContext.hpp"
#include "ShaderProgram.hpp"
#include "Mesh.hpp"
#include "Scene.hpp"
#include "MyScene.hpp"
#include "Camera.hpp"
#include "Vector3.hpp"
#include "KeyBuffer.hpp"
#include "MouseBuffer.hpp"
#include "Transform.hpp"
#include "Matrix4.hpp"
/******************************************************************/
// Local types
/// \brief The location and orientation through which we view the Scene.
///
/// This will be replaced in a future assignment with a much more robust and
///   interesting version, but is sufficient for now.

/******************************************************************/
// Global variables

/// \brief The OpenGLContext through which all OpenGL calls will be made.
///
/// This should be allocated in ::init and deallocated in ::releaseGlResources.
OpenGLContext* g_context;
// We use one VAO for each object we draw
/// \brief A collection of the VAOs for each of the objects we want to draw.
///
/// This will be filled in initScene, and its contents need to be deleted in
///   releaseGlResources.
Scene* g_scene;
Mesh* mesh;
/// \brief The ShaderProgram that transforms and lights the primitives.
///
/// This should be allocated in ::initShaders and deallocated in
///   ::releaseGlResources.
ShaderProgram* g_shaderProgram;
ShaderProgram* g_normalShaderProgram;
/// \brief The Camera that views the Scene.
///
/// This should be allocated in ::initCamera and deallocated in
///   ::releaseGlResources.
Camera* g_camera;
KeyBuffer* g_keyBuffer;
MouseBuffer* g_mouseBuffer;
/******************************************************************/
// Function prototypes

/// \brief Initializes all libraries and global variables.
/// \param[out] window A pointer that will be filled with a GLFWwindow.
///
/// This should be called once at the beginning of the application.
void
init (GLFWwindow*& window);

/// \brief Initializes the GLFW library.  Should only be called by ::init.
void
initGlfw ();

/// \brief Initializes the GLEW library.  Should only be called by ::init.
void
initGlew ();

/// \brief Creates and initializes the window.  Should only be called by
///   ::init.
/// \param[out] window A pointer that will be filled with a GLFWwindow.
void
initWindow (GLFWwindow*& window);

/// \brief Re-renders the window.  This should be called whenever the window
///   size changes.
/// \param[in] window The GLFWwindow to reset.
/// \param[in] width The new width of the window.
/// \param[in] height The new height of the window.
void
resetViewport (GLFWwindow* window, int width, int height);

/// \brief Creates the Scene.  Should only be called by ::init.
void
initScene ();

/// \brief Creates the ShaderProgram.  Should only be called by ::init.
void
initShaders ();

/// \brief Initializes the Camera.  Should only be called by ::init.
void
initCamera ();

/// \brief Moves geometric objects around using game logic.  This should be
///   called for every frame.
/// \param[in] time The number of seconds since the previous update.
void
updateScene (double time);

/// \brief Draws the Scene onto the window.  This should be called for every
///   frame.
/// \param[in] window The GLFWwindow to draw in.
void
drawScene (GLFWwindow* window);

/// \brief Responds to any user input.  This should be set as a callback.
/// \param[in] window The GLFWwindow the input came from.
/// \param[in] key The key that was pressed or released.
/// \param[in] scanCode The system-specific scancode of the key.
/// \param[in] action What happened to the key.
/// \param[in] modifiers Which modifier keys were depressed.
void
recordKeys (GLFWwindow* window, int key, int scanCode, int action,
	     int modifiers);

void
processKeys (int key);

void
processMouseButtons(GLFWwindow* window, int button, int action, int mods);

void
processCursorPos(GLFWwindow* window, double xPos, double yPos);

void
processScroll(GLFWwindow* window, double xoffset, double yoffset);

void
processResize(GLFWwindow* window, int width, int height);

/// \brief Cleans up all resources as program exits.
void
releaseGlResources ();

/// \brief Prints an error from GLFW.
/// \param[in] error The error code.
/// \param[in] description The error text.
void
outputGlfwError (int error, const char* description);

/******************************************************************/

/// \brief Runs our program.
/// \param[in] argc The number of command-line arguments, which we ignore.
/// \param[in] argv The array of command-line-arguments, which we ignore.
int
main (int argc, char* argv[])
{
  //std::cout << "test 4\n";
  GLFWwindow* window;
  init (window);

  // Game/render loop
  double previousTime = glfwGetTime();
  
  while (!glfwWindowShouldClose (window))
  {
    double currentTime = glfwGetTime ();
    // Compute frame times, which we can use later for frame rate computation,
    //   animation, and physics.
    double deltaTime = currentTime - previousTime;
    previousTime = currentTime;
    updateScene (deltaTime);
    drawScene (window);
    // Process events in the event queue, which results in callbacks
    //   being invoked.
    glfwPollEvents ();
    processKeys();
  }

  releaseGlResources ();
  // Destroying the window destroys the OpenGL context
  glfwDestroyWindow (window);
  glfwTerminate ();

  return EXIT_SUCCESS;
}

/******************************************************************/

void
init (GLFWwindow*& window)
{
  //std::cout << "test 3 \n";
  g_context = new RealOpenGLContext ();
  g_keyBuffer = new KeyBuffer();
  g_mouseBuffer = new MouseBuffer();
  // Always initialize GLFW before GLEW
  initGlfw ();
  initWindow (window);
  initGlew ();
  initShaders ();
  initCamera ();
  initScene ();
}

/******************************************************************/

void
initGlfw ()
{
  glfwSetErrorCallback (outputGlfwError);
  if (!glfwInit ())
  {
    fprintf (stderr, "Failed to init GLFW -- exiting\n");
    exit (EXIT_FAILURE);
  }
}

/******************************************************************/

void
initWindow (GLFWwindow*& window)
{
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef __APPLE__
  // Necessary on MacOS! Was needed in one case on Linux too.
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
  window = glfwCreateWindow (800, 600, "OpenGL Engine", nullptr, nullptr);
  if (window == nullptr)
  {
    fprintf (stderr, "Failed to init the window -- exiting\n");
    glfwTerminate ();
    exit (EXIT_FAILURE);
  }
  glfwSetWindowPos (window, 200, 100);

  glfwMakeContextCurrent (window);
  // Swap buffers after 1 frame
  glfwSwapInterval (1);
  glfwSetKeyCallback (window, recordKeys);
  glfwSetFramebufferSizeCallback (window, resetViewport);
  glfwSetCursorPosCallback(window, processCursorPos);
  glfwSetScrollCallback(window, processScroll);
  glfwSetWindowSizeCallback(window, processResize);
  glfwSetFramebufferSizeCallback (window, resetViewport);

  // Specify background color
  g_context->clearColor (0.0f, 0.0f, 0.0f, 1.0f);

  // Enable depth testing so occluded surfaces aren't drawn
  g_context->enable (GL_DEPTH_TEST);

  // Enable the culling of back-facing triangles
  // A triangle must be wound CCW in window coordinates to be front facing
  // If it is wound CW it is back facing and will NOT be displayed!
  g_context->enable (GL_CULL_FACE);

  // The next two setting are default, but we'll be explicit.
  g_context->frontFace (GL_CCW);
  g_context->cullFace (GL_BACK);

  // Set initial viewport size
  int width, height;
  glfwGetFramebufferSize (window, &width, &height);
  g_context->viewport (0, 0, width, height);
}

/******************************************************************/

void
initGlew ()
{
  GLenum status = glewInit ();
  if (status != GLEW_OK)
  {
    fprintf (stderr, "Failed to initialize GLEW -- exiting"
	     " (%s).\n",
	     glewGetErrorString (status));
    exit (EXIT_FAILURE);
  }
  const GLubyte* version = glewGetString (GLEW_VERSION);
  fprintf (stderr, "Using GLEW version %s.\n", version);
  version = g_context->getString (GL_VERSION);
  fprintf (stderr, "Using OpenGL version %s\n", version);
}

/******************************************************************/

void
resetViewport (GLFWwindow* window, int width, int height)
{
  // Render into entire window
  // Origin for window coordinates is lower-left of window
  g_context->viewport (0, 0, width, height);
}

/******************************************************************/

void
initScene ()
{
  g_scene = new MyScene(g_context, g_shaderProgram, g_normalShaderProgram);

}

/******************************************************************/

void
initShaders ()
{
  // Create shader programs, which consist of linked shaders.
  // No need to use the program until we draw or set uniform variables.
  g_shaderProgram = new ShaderProgram (g_context);
  g_shaderProgram->createVertexShader ("Vec3.vert");
  g_shaderProgram->createFragmentShader ("Vec3.frag");
  g_shaderProgram->link ();

  g_normalShaderProgram = new ShaderProgram (g_context);
  g_normalShaderProgram->createVertexShader ("Vec3Norm.vert");
  g_normalShaderProgram->createFragmentShader ("Vec3.frag");
  g_normalShaderProgram->link ();
}

/******************************************************************/

void
initCamera ()
{
  // g_camera = new Camera ();
  // Define the projection, which will remain constant
  float verticalFov = 50.0f;
  float aspectRatio = 1200.0f / 900;
  // Near plane
  float nearZ = 0.01f;
  // Far plane
  float farZ = 40.0f;
  Vector3 position = Vector3 (0, 0, 12.0f);
  g_camera = new Camera (position, Vector3 (0, 0, 1.0f), nearZ, farZ, aspectRatio, verticalFov);
  // // Enable shader program so we can set uniforms
  g_shaderProgram->enable ();
}

/******************************************************************/

void
updateScene (double time)
{
  // No updates in this simple program.
}

/******************************************************************/

void
drawScene (GLFWwindow* window)
{
  g_context->clear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Iterate over each object in scene and draw it
  // The shader program is already enabled, but we do not want to
  //   make that assumption in general. 

  g_scene->draw (g_shaderProgram, modelView, projectionView);
  
  // Swap the front and back buffers.
  // We draw to the back buffer, which is then swapped with the front
  //   for display.
  glfwSwapBuffers (window);
}

/******************************************************************/


void
recordKeys (GLFWwindow* window, int key, int scanCode, int action,
	     int modifiers)
{
  // Actions are GLFW_PRESS, GLFW_RELEASE, and GLFW_REPEAT
  // Exit if ESC is pressed
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose (window, GL_TRUE);
    return;
  }

  // Notify KeyBuffer if a key has been pressed or released
  if(action == GLFW_PRESS)
  {
    g_keyBuffer->setKeyDown(key);
  }
  else if(action == GLFW_RELEASE)
  {
    g_keyBuffer->setKeyUp(key);
  }

  // Process single actions here to avoid looping issues
  if(g_keyBuffer->isKeyDown(GLFW_KEY_MINUS))
    g_scene->activateNextMesh();
  if(g_keyBuffer->isKeyDown(GLFW_KEY_EQUAL))
    g_scene->activatePreviousMesh();
  if(g_keyBuffer->isKeyDown(GLFW_KEY_P))
  {
    // Projection values
    float nearZ = 0.01f;
    float farZ = 40.0f;
    float verticalFov = 50.0f;
    float aspectRatio = 1200.0f / 900;

    g_camera->setProjectionSymmetricPerspective(verticalFov, aspectRatio, nearZ, farZ);
  }
  if(g_keyBuffer->isKeyDown(GLFW_KEY_LEFT_BRACKET))
  {
    double left = -7.0;
    double right = 5.0;
    double top = -5.0;
    double bottom = 3.0;
    double nearZ = 5.0;
    double farZ = 40.0;

    g_camera->setProjectionAssymetricPerspective (left, right, bottom, top, nearZ, farZ);
  }
  if(g_keyBuffer->isKeyDown(GLFW_KEY_O))
  {
    g_camera->setProjectionOrthographic(-2.0f, 2.0f, -2.0f, 2.0f, 0.01f, 40.0f);
  }
}

void
processKeys ()
{
  // Movement and rotation scales
  const float MOVEMENT_DELTA = .05f;
  const float ROTATION_DELTA = .05f;

  // Keys for moving the camera
  if(g_keyBuffer->isKeyDown(GLFW_KEY_W))
    g_camera->moveBack(-MOVEMENT_DELTA);
  if(g_keyBuffer->isKeyDown(GLFW_KEY_S))
    g_camera->moveBack(MOVEMENT_DELTA);
  if(g_keyBuffer->isKeyDown(GLFW_KEY_A))
    g_camera->moveRight(-MOVEMENT_DELTA);
  if(g_keyBuffer->isKeyDown(GLFW_KEY_D))
    g_camera->moveRight(MOVEMENT_DELTA);
  if(g_keyBuffer->isKeyDown(GLFW_KEY_C))
    g_camera->moveUp(-MOVEMENT_DELTA);
  if(g_keyBuffer->isKeyDown(GLFW_KEY_F))
    g_camera->moveUp(MOVEMENT_DELTA);
  if(g_keyBuffer->isKeyDown(GLFW_KEY_R))
    g_camera->resetPose();

  // Keys for modifying the active mesh
  if(g_keyBuffer->isKeyDown(GLFW_KEY_J))
    g_scene->getActiveMesh()->yaw(ROTATION_DELTA);
  if(g_keyBuffer->isKeyDown(GLFW_KEY_L))
    g_scene->getActiveMesh()->yaw(-ROTATION_DELTA);
  if(g_keyBuffer->isKeyDown(GLFW_KEY_I))
    g_scene->getActiveMesh()->pitch(ROTATION_DELTA);
  if(g_keyBuffer->isKeyDown(GLFW_KEY_K))
    g_scene->getActiveMesh()->pitch(-ROTATION_DELTA);
  if(g_keyBuffer->isKeyDown(GLFW_KEY_N))
    g_scene->getActiveMesh()->roll(ROTATION_DELTA);
  if(g_keyBuffer->isKeyDown(GLFW_KEY_M))
    g_scene->getActiveMesh()->roll(-ROTATION_DELTA);
  if(g_keyBuffer->isKeyDown(GLFW_KEY_1))
    g_scene->getActiveMesh()->moveRight(MOVEMENT_DELTA);
  if(g_keyBuffer->isKeyDown(GLFW_KEY_2))
    g_scene->getActiveMesh()->moveRight(-MOVEMENT_DELTA);
  if(g_keyBuffer->isKeyDown(GLFW_KEY_3))
    g_scene->getActiveMesh()->moveUp(MOVEMENT_DELTA);
  if(g_keyBuffer->isKeyDown(GLFW_KEY_4))
    g_scene->getActiveMesh()->moveUp(-MOVEMENT_DELTA);
  if(g_keyBuffer->isKeyDown(GLFW_KEY_5))
    g_scene->getActiveMesh()->moveBack(MOVEMENT_DELTA);
  if(g_keyBuffer->isKeyDown(GLFW_KEY_6))
    g_scene->getActiveMesh()->moveBack(-MOVEMENT_DELTA);
  if(g_keyBuffer->isKeyDown(GLFW_KEY_7))
    g_scene->getActiveMesh()->scaleLocal(1.01f);
  if(g_keyBuffer->isKeyDown(GLFW_KEY_8))
    g_scene->getActiveMesh()->scaleWorld(0.99f);
}

/******************************************************************/

void
processMouseButtons(GLFWwindow* window, int button, int action, int mods)
{
  if(button == GLFW_MOUSE_BUTTON_LEFT)
    g_mouseBuffer->setLeftButton(action == GLFW_PRESS);
  if(button == GLFW_MOUSE_BUTTON_RIGHT)
    g_mouseBuffer->setRightButton(action == GLFW_PRESS);
}

void
processCursorPos(GLFWwindow* window, double xPos, double yPos)
{
  float angleScale = 0.1f;

  if(g_mouseBuffer->getLeftButton()) 
  {
    double xdiff = xPos - g_mouseBuffer->getX();
    double ydiff = yPos - g_mouseBuffer->getY();
    if(xdiff != 0)
      g_camera->yaw(xdiff * angleScale);
    if(ydiff != 0)
      g_camera->pitch(ydiff * angleScale);
  }

  if(g_mouseBuffer->getRightButton())
  {
    double xdiff = xPos - g_mouseBuffer->getX();
    if(xdiff != 0)
      g_camera->roll(xdiff * angleScale);
  }

  // Update buffer
  g_mouseBuffer->setPosition(xPos, yPos);
}

void
processScroll(GLFWwindow* window, double xoffset, double yoffset)
{
  const float SCALE = 2.0f;
  float nearZ = 0.01f;
  float farZ = 40.0f;
  float verticalFov = 50.0f;
  float aspectRatio = 1200.0f / 900;

  verticalFov += yoffset * SCALE;

  if(verticalFov > 120.0f)
  {
      verticalFov = 120.0f;
  }
  else if(verticalFov < 1.0f)
  {
      verticalFov = 1.0f;
  }

  g_camera->setProjectionSymmetricPerspective(verticalFov, aspectRatio, 0.01f, 40.0f);
}

void
processResize(GLFWwindow* window, int width, int height)
{
  float nearZ = 0.01f;
  float farZ = 40.0f;
  float verticalFov = 50.0f;
  float aspectRatio = 1200.0f / 900;
  // Set aspect ratio
  aspectRatio = (float)width/(float)height;

  g_camera->setProjectionSymmetricPerspective(verticalFov, aspectRatio, 0.01f, 40.0f);
}
/******************************************************************/

void
releaseGlResources ()
{
  // Delete OpenGL resources, particularly important if program will
  //   continue running
  delete g_scene;
  delete g_camera;
  delete g_shaderProgram;
  delete g_context;
  delete g_keyBuffer;
  delete g_mouseBuffer;
}

/******************************************************************/

void
outputGlfwError (int error, const char* description)
{
  fprintf (stderr, "GLFW error: %s, code %d\n", description, error);
}

/******************************************************************/