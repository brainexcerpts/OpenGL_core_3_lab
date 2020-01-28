/***************************************************************************
 *   Copyright (C) 2012 by Mathias Paulin                                  *
 *   Mathias.Paulin@irit.fr                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef MESH_H
#define MESH_H


#include <vector>
#include "glm/glm.hpp"

// =============================================================================
namespace Loaders {
// =============================================================================

/**
  *  @ingroup Loaders
  *
  * Represents a triangular mesh as an array of vertex and an array of triangluar faces.
  * In the vertex buffer, datas are interleaved (x,y,z, nx,ny,nz, u,v)
  * when auxiliary data are present ((nx,ny,nz) or (u,v))
  */
class Mesh {
public:

    /// Default constructor.
    /// Creates an empty mesh.
    Mesh();

    /**
      * Constructor from basic geometric data.
      * Creates the triangluar mesh from a vertex array and triangle or quad array.
      * Vertices are described by (x,y,z,nx,ny,nz,u,v), (x,y,z,nx,ny,nz) or (x,y,z)
      * depending of the values of hasNormals and hasTextureCoords.
      */
    Mesh (const std::vector<float> &vertexBuffer,
          const std::vector<int> &triangleBuffer,
          const std::vector<int> &quadBuffer,
          bool hasNormals, bool hasTextureCoords
          );

    /// Copy contructor.
    Mesh(const Mesh &mesh);

    /// Destructor.
    virtual ~Mesh();

    /// Gets the mesh data in raw format.
    void getData( std::vector<float>& vertexBuffer,
                  std::vector<int>& triangleBuffer,
                  bool& parametrized );

    /// Concatenates 2 meshes.
    Mesh & operator+=(const Mesh &m);

    int nbVertices () const { return mNbVertices;  }
    int nbTriangles() const { return mNbTriangles; }

    /// Prints basic information about the mesh on stderr.
    void printfInfo() const;

protected:

    /// Internal vertex representation, there is 3 attributes:
    /// position (vec3), normal (vec3) and texture coordinates (vec2)
    class Vertex {
    public:
        Vertex (void) : position (glm::vec3(0.,0.,0.)), normal (glm::vec3(0.,0.,0.)), texcoord(glm::vec2(0.,0.)) {}
        Vertex (const glm::vec3& pos) : position (pos), normal (glm::vec3(0.,0.,0.)), texcoord(glm::vec2(0.,0.)) {}

        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texcoord;
    };

    /// Internal triangle representation : three integer indices
    class TriangleIndex {
    public:

        unsigned int indexes[3]; ///<  index of vertices in the array of positions

        TriangleIndex (int i0, int i1, int i2) {
            indexes[0] = i0; indexes[1] = i1; indexes[2] = i2;
        }

        TriangleIndex (const int* pIds) {
            indexes[0] = pIds[0]; indexes[1] = pIds[1]; indexes[2] = pIds[2];
        }

        inline unsigned int& operator[] (int k) {
            assert(k<3);
            return indexes[k];
        }
        inline unsigned int operator[] (int k) const {
            assert (k<3);
            return indexes[k];
        }
    };

    typedef std::vector<Vertex> VertexArray;
    typedef std::vector<TriangleIndex> TriangleIndexArray;

    VertexArray mVertices; ///< vector of #Vertex
    int mNbVertices; ///< number of vertices

    TriangleIndexArray mTriangles; ///< vector of #TriangleIndex
    int mNbTriangles; ///< number of triangles

    bool mHasTextureCoords;
    bool mHasNormal;

    /// Compute smothed normals at each vertex.
    void computeNormals (void);

};

} // END namespace loaders =====================================================

#endif // MESH_H
