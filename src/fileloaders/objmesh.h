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
#ifndef OBJMESH_H
#define OBJMESH_H

#include "mesh.h"
#include <vector>
#include <string>

namespace Loaders {
namespace Obj_mtl {



/** @ingroup OBJ-MTL
*   OBJ smooth group.
*/
class SmoothGroup : public Loaders::Mesh {
public:
    SmoothGroup (
            const std::vector<float> &vertexBuffer,
            const std::vector<int> &triangleBuffer,
            const std::vector<int> &quadBuffer,
            bool hasNormals, bool hasTextureCoords
            );

};


/** @ingroup OBJ-MTL
*   OBJ mesh.
*/
class ObjMesh {
public:
    ObjMesh (const std::string &name/*, Material *material*/);
    ~ObjMesh();

    void addSmoothGroup (SmoothGroup *sg) {
        parts.push_back (sg);
        mNbVert += sg->nbVertices();
        mNbTri += sg->nbTriangles();
    }

    void printfInfo() const;

    void getStats (int &nbVertices, int &nbTriangles) {
        nbVertices = mNbVert;
        nbTriangles = mNbTri;
    }

    Mesh * compile();

    std::string getName(){ return mName; }


private :
    std::string mName;
    std::vector<SmoothGroup *> parts;
    int mNbVert;
    int mNbTri;
};

} // end namespace obj

} // end namespace loaders
#endif
