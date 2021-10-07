/// \file Geometry.hpp
/// \brief Definitions of global functions for manipulating geometry.
/// \author Chad Hogg
/// \version A08

#include <random>
#include <cassert>
#include <iostream>

#include "Geometry.hpp"

void
indexData (const std::vector<float>& geometry, unsigned int floatsPerVertex,
	   std::vector<float>& data, std::vector<unsigned int>& indices)
{
  const unsigned int VERTICES_PER_TRIANGLE = 3;
  const float EPSILON = 0.00001f;
  assert (geometry.size () % (floatsPerVertex * VERTICES_PER_TRIANGLE) == 0);
  // We must account for each vertex in the geometry vector.
  for (unsigned int geoIndex = 0; geoIndex < geometry.size () / floatsPerVertex; geoIndex++)
  {
    // Try to find a copy of it in the data vector.
    bool found = false;
    for (unsigned int dataIndex = 0; dataIndex < data.size () / floatsPerVertex && !found; dataIndex++)
    {
      bool matches = true;
      for (unsigned int part = 0; part < floatsPerVertex && matches; part++)
      {
	if (fabs (geometry[geoIndex * floatsPerVertex + part] -
		  data[dataIndex * floatsPerVertex + part]) >= EPSILON)
	{
	  matches = false;
	}
      }
      if (matches)
      {
	// Found it, just save that index!
	indices.push_back (dataIndex);
	found = true;
      }
    }
    if(!found)
    {
      // Didn't find it, so copy it to data vector and add new index.
      for (unsigned int part = 0; part < floatsPerVertex; part++)
      {
	data.push_back (geometry[geoIndex * floatsPerVertex + part]);
      }
      indices.push_back (data.size () / floatsPerVertex - 1);
    }
  }
}

std::vector<Vector3>
computeFaceNormals (const std::vector<Triangle>& faces)
{
  std::vector<Vector3> faceNormals;
  for(unsigned int faceIndex = 0; faceIndex < faces.size (); faceIndex++)
  {
    // We learned this algorithm back in Lecture 04!
    Vector3 normal = (faces[faceIndex][1] - faces[faceIndex][0]).cross (faces[faceIndex][2] - faces[faceIndex][0]);
    normal.normalize ();
    faceNormals.push_back (normal);
  }
  return faceNormals;
}

std::vector<Vector3>
computeVertexNormals (const std::vector<Triangle>& faces,
		      const std::vector<Vector3>& faceNormals)
{
  assert (faces.size () == faceNormals.size ());
  std::vector<Vector3> vertexNormals;
  for (unsigned int faceIndex = 0; faceIndex < faces.size (); faceIndex++)
  {
    for (unsigned int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
    {
      // We need to find *every* vertex in any triangle that is at this
      //   position and average their face normals.
      Vector3 vertexNormal (0.0f, 0.0f, 0.0f);
      for (unsigned int otherFaceIndex = 0; otherFaceIndex < faces.size (); otherFaceIndex++)
      {
	for (unsigned int otherVertexIndex = 0; otherVertexIndex < 3; otherVertexIndex++ )
	{
	  if (faces[faceIndex][vertexIndex] == faces[otherFaceIndex][otherVertexIndex])
	  {
	    // Hey, we derived this formula in Lecture 04!
	    float area = 0.5f * ((faces[otherFaceIndex][1] - faces[otherFaceIndex][0]).cross (faces[otherFaceIndex][2] - faces[otherFaceIndex][0])).length ();
	    unsigned int oppositeIndexA = (otherVertexIndex + 1) % 3;
	    unsigned int oppositeIndexB = (otherVertexIndex + 2) % 3;
	    float angle = (faces[otherFaceIndex][oppositeIndexA] - faces[otherFaceIndex][otherVertexIndex]).angleBetween (faces[otherFaceIndex][oppositeIndexB] - faces[otherFaceIndex][otherVertexIndex]);
	    // Weighting the average by area makes it so that lots of smaller
	    //   faces don't overwhelm a few larger faces.
	    // Weighting the average by angle makes it so that points where
	    //   two 45 degree angles and points where one 90 degree angle meet
	    //   get the same treatment.
	    vertexNormal += faceNormals[otherFaceIndex] * fabs (area) * fabs (angle);
	  }
	}
      }
      vertexNormal.normalize ();
      vertexNormals.push_back (vertexNormal);
    }
  }
  return vertexNormals;
}

std::vector<Vector3>
generateRandomFaceColors (const std::vector<Triangle>& faces)
{
  std::default_random_engine generator;
  std::uniform_real_distribution<float> distribution(0.0f,1.0f);
  std::vector<Vector3> faceColors;
  for (unsigned int faceIndex = 0; faceIndex < faces.size (); faceIndex++)
  {
    faceColors.push_back (Vector3 (distribution (generator), distribution (generator), distribution (generator)));
  }
  return faceColors;
}

std::vector<Vector3>
generateRandomVertexColors (const std::vector<Triangle>& faces)
{
  std::default_random_engine generator;
  std::uniform_real_distribution<float> distribution(0.0f,1.0f);
  std::vector<Vector3> vertexColors;
  for (unsigned int faceIndex = 0; faceIndex < faces.size (); faceIndex++)
  {
    for (unsigned int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
    {
      // If we already assigned a color to that position, we need to copy it.
      bool foundMatch = false;
      for (unsigned int otherFaceIndex = 0; otherFaceIndex < faceIndex && !foundMatch; otherFaceIndex++)
      {
	for (unsigned int otherVertexIndex = 0; otherVertexIndex < 3; otherVertexIndex++)
	{
	  if(faces[faceIndex][vertexIndex] == faces[otherFaceIndex][otherVertexIndex])
	  {
	    vertexColors.push_back (vertexColors[otherFaceIndex * 3 + otherVertexIndex]);
	    foundMatch = true;
	  }
	}
      }
      // If we never saw this position before, generate a new random color.
      if(!foundMatch)
      {
	vertexColors.push_back (Vector3 (distribution (generator), distribution (generator), distribution (generator)));
      }
    }
  }
  return vertexColors;
}

std::vector<float>
dataWithFaceColors (const std::vector<Triangle>& faces,
		    const std::vector<Vector3>& faceColors)
{
  assert (faces.size () == faceColors.size ());
  std::vector<float> data;
  for(unsigned int faceIndex = 0; faceIndex < faces.size (); faceIndex++)
  {
    for(unsigned int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
    {
      data.push_back (faces[faceIndex][vertexIndex].m_x);
      data.push_back (faces[faceIndex][vertexIndex].m_y);
      data.push_back (faces[faceIndex][vertexIndex].m_z);
      data.push_back (faceColors[faceIndex].m_x);
      data.push_back (faceColors[faceIndex].m_y);
      data.push_back (faceColors[faceIndex].m_z);
    }
  }
  return data;
}

std::vector<float>
dataWithVertexColors (const std::vector<Triangle>& faces,
		      const std::vector<Vector3>& vertexColors)
{
  assert (faces.size () * 3 == vertexColors.size ());
  std::vector<float> data;
  for(unsigned int faceIndex = 0; faceIndex < faces.size (); faceIndex++)
  {
    for(unsigned int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
    {
      data.push_back (faces[faceIndex][vertexIndex].m_x);
      data.push_back (faces[faceIndex][vertexIndex].m_y);
      data.push_back (faces[faceIndex][vertexIndex].m_z);
      data.push_back (vertexColors[faceIndex * 3 + vertexIndex].m_x);
      data.push_back (vertexColors[faceIndex * 3 + vertexIndex].m_y);
      data.push_back (vertexColors[faceIndex * 3 + vertexIndex].m_z);
    }
  }
  return data;
}

std::vector<float>
dataWithFaceNormals (const std::vector<Triangle>& faces,
		     const std::vector<Vector3>& faceNormals)
{
  assert (faces.size () == faceNormals.size ());
  std::vector<float> data;
  for(unsigned int faceIndex = 0; faceIndex < faces.size (); faceIndex++)
  {
    for(unsigned int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
    {
      data.push_back (faces[faceIndex][vertexIndex].m_x);
      data.push_back (faces[faceIndex][vertexIndex].m_y);
      data.push_back (faces[faceIndex][vertexIndex].m_z);
      data.push_back (faceNormals[faceIndex].m_x);
      data.push_back (faceNormals[faceIndex].m_y);
      data.push_back (faceNormals[faceIndex].m_z);
    }
  }
  return data;
}

std::vector<float>
dataWithVertexNormals (const std::vector<Triangle>& faces,
		       const std::vector<Vector3>& vertexNormals)
{
  assert (faces.size () * 3 == vertexNormals.size ());
  std::vector<float> data;
  for(unsigned int faceIndex = 0; faceIndex < faces.size (); faceIndex++)
  {
    for(unsigned int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
    {
      data.push_back (faces[faceIndex][vertexIndex].m_x);
      data.push_back (faces[faceIndex][vertexIndex].m_y);
      data.push_back (faces[faceIndex][vertexIndex].m_z);
      data.push_back (vertexNormals[faceIndex * 3 + vertexIndex].m_x);
      data.push_back (vertexNormals[faceIndex * 3 + vertexIndex].m_y);
      data.push_back (vertexNormals[faceIndex * 3 + vertexIndex].m_z);
    }
  }
  return data;
}

std::vector<Triangle>
buildCube ()
{
  std::vector<Triangle> triangles;
  // Front side (upper-left tri)
  triangles.push_back ((Triangle){Vector3 (-0.5f, 0.5f, 0.5f), Vector3 (-0.5f, -0.5f, 0.5f), Vector3 (0.5f, 0.5f, 0.5f)});
  // Front side (lower-right tri)
  triangles.push_back ((Triangle){Vector3 (0.5f, -0.5f, 0.5f), Vector3 (0.5f, 0.5f, 0.5f), Vector3 (-0.5f, -0.5f, 0.5f)});
  // Right side (upper-left tri)
  triangles.push_back ((Triangle){Vector3 (0.5f, 0.5f, 0.5f), Vector3 (0.5f, -0.5f, 0.5f), Vector3 (0.5f, 0.5f, -0.5f)});
  // Right side (lower-right tri)
  triangles.push_back ((Triangle){Vector3 (0.5f, -0.5f, -0.5f), Vector3 (0.5f, 0.5f, -0.5f), Vector3 (0.5f, -0.5f, 0.5f)});
  // Back side (upper-left tri)
  triangles.push_back ((Triangle){Vector3 (0.5f, 0.5f, -0.5f), Vector3 (0.5f, -0.5f, -0.5f), Vector3 (-0.5f, 0.5f, -0.5f)});
  // Back side (lower-right tri)
  triangles.push_back ((Triangle){Vector3 (-0.5f, -0.5f, -0.5f), Vector3 (-0.5f, 0.5f, -0.5f), Vector3 (0.5f, -0.5f, -0.5f)});
  // Left side (upper-left tri)
  triangles.push_back ((Triangle){Vector3 (-0.5f, 0.5f, -0.5f), Vector3 (-0.5f, -0.5f, -0.5f), Vector3 (-0.5f, 0.5f, 0.5f)});
  // Left side (lower-right tri)
  triangles.push_back ((Triangle){Vector3 (-0.5f, -0.5f, 0.5f), Vector3 (-0.5f, 0.5f, 0.5f), Vector3 (-0.5f, -0.5f, -0.5f)});
  // Top side (upper-left tri)
  triangles.push_back ((Triangle){Vector3 (-0.5f, 0.5f, -0.5f), Vector3 (-0.5f, 0.5f, 0.5f), Vector3 (0.5f, 0.5f, -0.5f)});
  // Top side (lower-right tri)
  triangles.push_back ((Triangle){Vector3 (0.5f, 0.5f, 0.5f), Vector3 (0.5f, 0.5f, -0.5f), Vector3 (-0.5f, 0.5f, 0.5f)});
  // Bottom side (upper-left tri)
  triangles.push_back ((Triangle){Vector3 (-0.5f, -0.5f, 0.5f), Vector3 (-0.5f, -0.5f, -0.5f), Vector3 (0.5f, -0.5f, 0.5f)});
  // Bottom side (lower-right tri)
  triangles.push_back ((Triangle){Vector3 (0.5f, -0.5f, -0.5f), Vector3 (0.5f, -0.5f, 0.5f), Vector3 (-0.5f, -0.5f, -0.5f)});
  return triangles;
}
