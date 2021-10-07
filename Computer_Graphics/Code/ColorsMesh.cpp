/// \file ColorsMesh.cpp
/// \brief Definitions of ColorsMesh class member and its global functions.
/// \author Evan Hanzelman
/// \version A08

#include "ColorsMesh.hpp"

// Constructor to pass parameters to Mesh constructor
ColorsMesh::ColorsMesh(OpenGLContext* context, ShaderProgram* shaderProgram)
: Mesh(context, shaderProgram)
{}

// Don't need to do anything in destructor
ColorsMesh::~ColorsMesh()
{}

// Override getFloatsPerVertex function
unsigned int
ColorsMesh::getFloatsPerVertex()
{
    return 6;
}

// Override enableAttributes function
void
ColorsMesh::enableAttributes()
{
    Mesh::enableAttributes();

    // Inform shaders how color data is stored
    m_context->enableVertexAttribArray(COLOR_ATTRIB_INDEX);
    // Color data is stored in sets of three for r,g,b. Starts at 4th element in array.
    //      Interweaved with position data, so start of next color data is every 6 elements
    m_context->vertexAttribPointer(COLOR_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(GL_FLOAT), reinterpret_cast<void*> (3 * sizeof(float)));
}