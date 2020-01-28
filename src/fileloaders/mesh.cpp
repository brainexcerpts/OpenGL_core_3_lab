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
#include "mesh.h"
#include "utils.h"
#include <iostream>

namespace Loaders {
using namespace Utils;
Mesh::Mesh (): mNbVertices(0), mNbTriangles(0), mHasTextureCoords (true), mHasNormal (true) {

}

Mesh::Mesh (const std::vector<float> &vertexBuffer, const std::vector<int> &triangleBuffer, const std::vector<int> &quadBuffer, bool hasNormal, bool hasTextureCoords) : mHasTextureCoords (hasTextureCoords), mHasNormal (hasNormal) {
    // Construction de la liste des sommets et BBox
    mNbVertices = 0;
    std::vector<float>::const_iterator it = vertexBuffer.begin();
    while (it != vertexBuffer.end()) {
        float x = *it; ++it;
        float y = *it; ++it;
        float z = *it; ++it;
                Vertex v (glm::vec3(x, y, z));
        if (hasNormal) {
            float nx = *it; ++it;
            float ny = *it; ++it;
            float nz = *it; ++it;
                        v.normal = glm::vec3 (nx, ny, nz);
                        glm::normalize(v.normal);
        }
        if (hasTextureCoords) {
            v.texcoord[0] = *it; ++it;
                        v.texcoord[1] = *it; ++it;
        }
        mVertices.push_back (v);
        ++mNbVertices;
    }

    // construction liste des faces triangulaires
    std::vector<int>::const_iterator fit = triangleBuffer.begin();
    mNbTriangles = 0;
    while (fit != triangleBuffer.end()) {
        int index1 = *fit; ++fit;
        assert (index1 < mNbVertices);
        int index2 = *fit; ++fit;
        assert (index2 < mNbVertices);
        int index3 = *fit; ++fit;
        assert (index3 < mNbVertices);
        ++mNbTriangles;
        TriangleIndex f (index1, index2, index3);
        mTriangles.push_back (f);
    }

        //   construction liste des faces quads
    std::vector<int>::const_iterator fiq = quadBuffer.begin();
    while (fiq != quadBuffer.end()) {
        int index1 = *fiq; ++fiq;
        assert (index1 < mNbVertices);
        int index2 = *fiq; ++fiq;
        assert (index2 < mNbVertices);
        int index3 = *fiq; ++fiq;
        assert (index3 < mNbVertices);
        int index4 = *fiq; ++fiq;
        assert (index4 < mNbVertices);

                ++mNbTriangles;
                TriangleIndex f1 (index1, index2, index3);
                mTriangles.push_back (f1);

                ++mNbTriangles;
                TriangleIndex f2 (index1, index3, index4);
                mTriangles.push_back (f2);

    }

    if (!hasNormal){
                computeNormals();
                hasNormal = true;
        }

}

Mesh::Mesh(const Mesh &mesh)
{
    mVertices = mesh.mVertices;
    mTriangles = mesh.mTriangles;
    mNbVertices = mesh.mNbVertices;
    mNbTriangles = mesh.mNbTriangles;
    mHasTextureCoords = mesh.mHasTextureCoords;
    mHasNormal = mesh.mHasNormal;
}

Mesh::~Mesh() {

}

void Mesh::printfInfo() const {
        std::cout << "\tMesh -> " << "vert : " << mNbVertices
                  << " tri : " << mNbTriangles << " "
                  << mHasNormal << " "
                  << mHasTextureCoords
                  << std::endl;
}

void Mesh::computeNormals (void) {
    for (TriangleIndexArray::iterator f_iter = mTriangles.begin() ; f_iter != mTriangles.end() ; ++f_iter) {
        Vertex& v0 = mVertices[f_iter->indexes[0]];
        Vertex& v1 = mVertices[f_iter->indexes[1]];
        Vertex& v2 = mVertices[f_iter->indexes[2]];
                glm::vec3 p0, p1, p2;
        p0 = v0.position;
        p1 = v1.position;
        p2 = v2.position;

                // compute normals
                glm::vec3 f_normal;
                f_normal = glm::cross((p1 - p0), (p2 - p0));
                glm::normalize(f_normal);
        v0.normal += f_normal;
        v1.normal += f_normal;
        v2.normal += f_normal;
    }
    for (VertexArray::iterator v_iter = mVertices.begin() ; v_iter != mVertices.end() ; ++v_iter) {
                glm::normalize(v_iter->normal);
    }
}

void Mesh::getData ( std::vector<float> &vertexBuffer, std::vector<int> &triangleBuffer, bool &parametrized ){
    parametrized = true;

    for (VertexArray::iterator v_iter = mVertices.begin() ; v_iter != mVertices.end() ; ++v_iter) {
        vertexBuffer.push_back(v_iter->position[0]);
        vertexBuffer.push_back(v_iter->position[1]);
        vertexBuffer.push_back(v_iter->position[2]);
        vertexBuffer.push_back(v_iter->normal[0]);
        vertexBuffer.push_back(v_iter->normal[1]);
        vertexBuffer.push_back(v_iter->normal[2]);
        if (mHasTextureCoords){
            vertexBuffer.push_back(v_iter->texcoord[0]);
            vertexBuffer.push_back(v_iter->texcoord[1]);
        } else {
            vertexBuffer.push_back(v_iter->position[0]);
            vertexBuffer.push_back(v_iter->position[1]);
        }
    }
    for (TriangleIndexArray::iterator f_iter = mTriangles.begin() ; f_iter != mTriangles.end() ; ++f_iter) {
        triangleBuffer.push_back(f_iter->indexes[0]);
        triangleBuffer.push_back(f_iter->indexes[1]);
        triangleBuffer.push_back(f_iter->indexes[2]);
    }
}

Mesh & Mesh::operator+=(const Mesh &m){
    for (VertexArray::const_iterator v_iter = m.mVertices.begin() ; v_iter != m.mVertices.end() ; ++v_iter) {
        mVertices.push_back(*v_iter);
    }
    for (TriangleIndexArray::const_iterator f_iter = m.mTriangles.begin() ; f_iter != m.mTriangles.end() ; ++f_iter) {
        TriangleIndex f (f_iter->indexes[0]+mNbVertices,
                         f_iter->indexes[1]+mNbVertices,
                         f_iter->indexes[2]+mNbVertices);
        mTriangles.push_back(f);
    }
    mNbVertices+=m.mNbVertices;
    mNbTriangles+=m.mNbTriangles;
    return *this;
}

} // namespace loaders
