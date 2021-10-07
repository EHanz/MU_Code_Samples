/// \file Geometry.hpp
/// \brief Declarations of global functions for manipulaing geometry.
/// \author Chad Hogg
/// \version A08

#include <vector>
#include <array>

#include "Vector3.hpp"

// A triangle consists of exactly 3 Vector3s (the coordinates of the vertices).
using Triangle = std::array<Vector3, 3>;

/// \brief Indexes some geometry.
/// \param[in] geometry A collection containing floats defining some vertices.
/// \param[in] floatsPerVertex The number of floats used for each vertex.
/// \param[out] data A collection into which unique vertex data can be written.
/// \param[out] indices A collection into which vector indexes for each
///   triangle can be written.
/// \post Each unique vertex in geometry has its data copied to data.
/// \post indices contains the correct indices for each vertex to build
///   triangles.
/// This uses the two out parameters simply because we can't return two tihngs.
void
indexData (const std::vector<float>& geometry, unsigned int floatsPerVertex,
	   std::vector<float>& data, std::vector<unsigned int>& indices);

/// \brief Computes a normal vector for each face of a mesh.
/// \param[in] faces A collection of faces that are part of the mesh.
/// \return A collection containing one normal vector per face.
std::vector<Vector3>
computeFaceNormals (const std::vector<Triangle>& faces);

/// \brief Computes a vertex normal for each vertex of a mesh.
/// \param[in] faces A collection of faces that are part of the mesh.
/// \param[in] faceNormals A collection of normal vectors for each face.
/// \return A collection of normal vectors for each vertex.  This is the
///   average of the face normals for each face that meets at that vertex,
///   weighted by both the area of the face (larger faces have a larger weight)
///   and by the angle at which the face meets the verrtex (larger angles have
///   a larger weight).
/// The same value will appear multiple times in the output collection, because
///   there are (presumably) several faces meeting at the same vertex, and we
///   are outputting a normal for each of the three vertices of each face.
///   During indexing these will all be collapsed.
std::vector<Vector3>
computeVertexNormals (const std::vector<Triangle>& faces,
		      const std::vector<Vector3>& faceNormals);

/// \brief Assigns a random color to each face of a mesh.
/// \param[in] faces A collection of faces that are part of the mesh.
/// \return A collection containing one color (R,G,B) per face.
std::vector<Vector3>
generateRandomFaceColors (const std::vector<Triangle>& faces);

/// \brief Assigns a random color to each vertex of a mesh.
/// \param[in] A collection of faces that are part of the mesh.
/// \return A collection containing three colors per face.  When the same
///   vertex is shared by multiple faces, each copy of the vertex will be
///   assigned the same random color.
std::vector<Vector3>
generateRandomVertexColors (const std::vector<Triangle>& faces);

/// \brief Produces a collection of interleaved position / color data from
///   faces and face colors.
/// \param[in] faces A collection of faces that are part of the mesh.
/// \param[in] faceColors A collection of colors, one per face.
/// \return A collection containing interleaved position / color data that is
///   ready to be indexed / added to a Mesh.
std::vector<float>
dataWithFaceColors (const std::vector<Triangle>& faces,
		    const std::vector<Vector3>& faceColors);

/// \brief Produces a collection of interleaved position / color data from
///   faces and vertex colors.
/// \param[in] faces A collection of faces that are part of the mesh.
/// \param[in] vertexColors A collection of colors, three per face.
/// \return A collection containing interleaved position / color data that is
///   ready to be indexed / added to a Mesh.
std::vector<float>
dataWithVertexColors (const std::vector<Triangle>& faces,
		      const std::vector<Vector3>& vertexColors);

/// \brief Produces a collection of interleaved position / normal data from
///   faces and face normals.
/// \param[in] faces A collection of faces that are part of the mesh.
/// \param[in] faceNormals A collection of normals, one per face.
/// \return A collection containing interleaved position / normal data that is
///   ready to be indexed / added to a Mesh.
std::vector<float>
dataWithFaceNormals (const std::vector<Triangle>& faces,
		     const std::vector<Vector3>& faceNormals);

/// \brief Produces a collection of interleaved position / normal data from
///   faces and vertex normals.
/// \param[in] faces A collection of faces that are part of the mesh.
/// \param[in] vertexNormals A collection of normals, three per face.
/// \return A collection containing interleaved position / normal data that is
///   ready to be indexed / added to a Mesh.
std::vector<float>
dataWithVertexNormals (const std::vector<Triangle>& faces,
		       const std::vector<Vector3>& vertexNormals);

/// \brief Creates a collection of triangles in a unit cube.
/// \return A collection of triangles in a unit cube, centered on the origin.
std::vector<Triangle>
buildCube ();
