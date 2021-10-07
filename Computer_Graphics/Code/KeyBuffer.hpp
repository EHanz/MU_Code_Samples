/// \file KeyBuffer.hpp
/// \brief Declaration of KeyBuffer class and any associated global functions.
/// \author Chad Hogg
/// \version A04

#ifndef KEY_BUFFER_HPP
#define KEY_BUFFER_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/// \brief A way to remember the status of each key -- either up or down.
/// Nothing in this class actually controls the keys, it just keeps track of
///   which ones it has been told are down.
class KeyBuffer
{
 public:
  /// \brief Constructs a new KeyBuffer.
  /// \postcondition All keys are assumed to be "up".
  KeyBuffer ();

  /// \brief Records that a key is "down".
  /// \param[in] key The key that was pressed (a GLFW_KEY_? constant).
  /// \postcondition That key is "down".
  void
  setKeyDown (int key);

  /// \brief Sets all keys to "up".
  /// \postcondition All keys are assumed to be "up".
  void
  reset ();

  /// \brief Records that a key is "up".
  /// \param[in] key The key that was released (a GLFW_KEY_?) constant).
  /// \postcondition That key is "up".
  void
  setKeyUp (int key);

  /// \brief Checks whether or not a key "down".
  /// \parm[in] key The key that you want to know about (a GLFW_KEY_? constant).
  /// \return True if that key is "down", otherwise false.
  bool
  isKeyDown (int key) const;

 private:
  /// \brief An array containing the status of each key constant.
  /// True indicates that the key is down, false that it is up.
  bool keyIsPressed[GLFW_KEY_LAST + 1];
};


#endif//KEY_BUFFER_HPP