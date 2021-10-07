/// \file KeyBuffer.cpp
/// \brief Cpp for KeyBuffer Class
/// \author Evan Hanzelman
/// \version A04

#include "KeyBuffer.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

KeyBuffer :: KeyBuffer ()
{
    reset ();
}

void
KeyBuffer :: setKeyDown (int key)
{
    keyIsPressed[key] = true;
}

void
KeyBuffer :: reset ()
{
    int length = sizeof(keyIsPressed)/sizeof(keyIsPressed[0]);
    for (int i = 0; i < length; i++)
    {
        keyIsPressed[i] = false;
    }
}

void
KeyBuffer :: setKeyUp (int key)
{
    keyIsPressed[key] = false;
}

bool
KeyBuffer :: isKeyDown (int key) const
{
    if (keyIsPressed[key])
    {
        return true;
    }
    else
    {
        return false;
    }
    
}