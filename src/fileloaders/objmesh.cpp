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
#include "objmesh.h"

#include <iostream>

namespace Loaders {
namespace Obj_mtl {

#define EPSILON 1E-14

ObjMesh::ObjMesh (const std::string &name/*, Material *material*/): mName(name) {
        //setMaterial (material);
	mNbVert = 0;
        mNbTri = 0;
}

ObjMesh::~ObjMesh() {
	// detruire les parties
	for (std::vector<SmoothGroup *>::iterator it = parts.begin(); it != parts.end(); ++it)
		delete *it;
	parts.clear();
}

void ObjMesh::printfInfo() const {
	int nbParts = parts.size();
        int totalTriFaces = 0;
	int totalVert = 0;

        std::cerr << mName << "\tParts : " <<  nbParts << std::endl; //"\tMaterial : " << getMaterial()->name() << std::endl;
	for (int i = 0; i < nbParts; i++) {
		parts[i]->printfInfo();
		totalVert += parts[i]->nbVertices();
                totalTriFaces += parts[i]->nbTriangles();
	}
        std::cerr << "Total : " << " -> " << "vert : " << totalVert  << " tri : " << totalTriFaces  << std::endl;

}

Mesh *ObjMesh::compile(){
    int nbParts = parts.size();
    Mesh * result = new Mesh();
    for (int i = 0; i < nbParts; i++)
        *result += *parts[i];
    return result;
}

// -------------------------------------------------------

SmoothGroup::SmoothGroup (const std::vector<float> &vertexBuffer,
                          const std::vector<int> &triangleBuffer,
                          const std::vector<int> &quadBuffer, bool hasNormals, bool hasTextureCoords)
//assume the obj objects are 2D-manifolds
                : Mesh (vertexBuffer, triangleBuffer, quadBuffer, hasNormals, hasTextureCoords) {
}

} // end namespace obj

} // end namespace loaders
