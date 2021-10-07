/// \file Mesh.cpp
/// \brief Definitions of Mesh class member and associated global
///   functions.
/// \author Evan Hanzelman
/// \version A08

#include "Mesh.hpp"

Mesh::Mesh(OpenGLContext* context, ShaderProgram* shaderProgram)
{
    // Store context
    m_context = context;

    // Store shader program
    m_shaderProgram = shaderProgram;

    // Setup VAO, VBO, and IBO
    m_context->genVertexArrays(1, &m_vao);
    m_context->genBuffers(1, &m_vbo);
    m_context->genBuffers(1, &m_ibo);

    m_prepared = false;
}

// Destructor
Mesh::~Mesh()
{
    // Delete VAO
    m_context->deleteVertexArrays(1, &m_vao);

    // Delete VBO
    m_context->deleteBuffers(1, &m_vbo);
}

void
Mesh::addGeometry(const std::vector<float>& geometry)
{
    // Copy input geometry
    m_data.resize(geometry.size());
    std::copy(geometry.begin(), geometry.end(), m_data.begin());
}

void
Mesh::prepareVao()
{
    
    m_context->bindVertexArray(m_vao);
    m_context->bindBuffer(GL_ARRAY_BUFFER, m_vbo);

    m_context->bufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(float), 
        m_data.data(), GL_STATIC_DRAW);

    m_context->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    m_context->bufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
        m_indices.data(), GL_STATIC_DRAW);

    enableAttributes();

    m_context->bindVertexArray(0);

    m_prepared = true;
}

void
Mesh::draw(const Transform& viewMatrix, const Matrix4& projectionMatrix)
{
    // Get model-view matrix
    Transform modelView (viewMatrix);
    modelView.combine(m_world);

    // Enable shader program
    m_shaderProgram->enable();
    m_shaderProgram->setUniformMatrix("uModelView", modelView.getTransform());
    m_shaderProgram->setUniformMatrix("uProjection", projectionMatrix);

    // Bind VAO and draw
    m_context->bindVertexArray(m_vao);
    m_context->drawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
        reinterpret_cast<void*> (0));
    m_context->bindVertexArray(0);

    // Disable shader program
    m_shaderProgram->disable();
}

Transform
Mesh::getWorld() const
{
    return m_world;
}

void
Mesh::moveRight(float distance)
{
    m_world.moveRight(distance);
}

void
Mesh::moveUp(float distance)
{
    m_world.moveUp(distance);
}

void
Mesh::moveBack(float distance)
{
    m_world.moveBack(distance);
}

void
Mesh::moveLocal(float distance, const Vector3& localDirection)
{
    m_world.moveLocal(distance, localDirection);
}

void
Mesh::moveWorld(float distance, const Vector3& worldDirection)
{
    m_world.moveWorld(distance, worldDirection);
}

void
Mesh::pitch(float angleDegrees)
{
    m_world.pitch(angleDegrees);
}

void
Mesh::yaw(float angleDegrees)
{
    m_world.yaw(angleDegrees);
}

void
Mesh::roll(float angleDegrees)
{
    m_world.roll(angleDegrees);
}

void
Mesh::rotateLocal(float angleDegrees, const Vector3& axis)
{
    m_world.rotateLocal(angleDegrees, axis);
}

void
Mesh::alignWithWorldY()
{
    m_world.alignWithWorldY();
}

void
Mesh::scaleLocal(float scale)
{
    m_world.scaleLocal(scale);
}

void
Mesh::scaleLocal(float scaleX, float scaleY, float scaleZ)
{
    m_world.scaleLocal(scaleX, scaleY, scaleZ);
}

void
Mesh::scaleWorld(float scale)
{
    m_world.scaleWorld(scale);
}

void
Mesh::scaleWorld(float scaleX, float scaleY, float scaleZ)
{
    m_world.scaleWorld(scaleX, scaleY, scaleZ);
}

void
Mesh::shearLocalXByYz(float shearY, float shearZ)
{
    m_world.shearLocalXByYz(shearY, shearZ);
}

void
Mesh::shearLocalYByXz(float shearX, float shearZ)
{
    m_world.shearLocalYByXz(shearX, shearZ);
}

void
Mesh::shearLocalZByXy(float shearX, float shearZ)
{
    m_world.shearLocalZByXy(shearX, shearZ);
}

void
Mesh::addIndices (const std::vector<unsigned int>& indices)
{
    // Add indices to m_indices
    for (unsigned int i = 0; i < indices.size(); ++i)
        m_indices.push_back(indices[i]);
}

unsigned int
Mesh::getFloatsPerVertex () const
{
    return 6;
}

void
Mesh::enableAttributes()
{
    m_context->enableVertexAttribArray(POSITION_ATTRIB_INDEX);
    m_context->vertexAttribPointer(POSITION_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(GL_FLOAT), reinterpret_cast<void*> (0));
}