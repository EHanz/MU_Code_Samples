/// \file ColorsMesh.hpp
/// \brief Declaration of ColorsMesh class and its global functions.
/// \author Evan Hanzelman
/// \version A08

#ifndef COLORSMESH_HPP
#define COLORSMESH_HPP

#include "OpenGLContext.hpp"
#include "Mesh.hpp"

// Subclass of Mesh that uses color attributes
class ColorsMesh : public Mesh
{
public:

    // Construct a new ColorsMesh
    ColorsMesh(OpenGLContext* context, ShaderProgram* shaderProgram);

    // Deconstructor
    ~ColorsMesh();

    // Overriden getFloatsPerVertex method
    virtual unsigned int
    getFloatsPerVertex();

protected:

    // Overriden enableAttributes method
    virtual void
    enableAttributes();
};

#endif // COLORSMESH_HPP