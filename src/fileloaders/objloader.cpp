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
#include "objloader.h"


#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <QFileInfo>


namespace Loaders {
namespace Obj_mtl {


ObjLoader::ObjLoader()
{
    vertices = 0;
    normals = 0;
    textures = 0;
    materialNumber = 0;
    currentGroup = new Group("default");
    allgroups["default"] = currentGroup;
    groupsNumber = 1;
    verticesTable.reserve(100000);
    normalsTable.reserve(100000);
    texturesTable.reserve(100000);
}


ObjLoader::~ObjLoader()
{
    mtllib.erase(mtllib.begin(), mtllib.end());
    //     delete allgroups["default"];
    allgroups.erase(allgroups.begin(), allgroups.end());
}

void ObjLoader::info_callback(const std::string& filename, std::size_t /*line_number*/, const std::string& message)
{
    std::ostringstream info_message;
    info_message << filename << std::endl
                 << message << std::endl;
    lastParseMessage = info_message.str();
}

void ObjLoader::warning_callback(const std::string& filename, std::size_t line_number, const std::string& message)
{
    std::cerr << filename << ":" << line_number << ": "
              << "warning: " << message << std::endl;
}

void ObjLoader::error_callback(const std::string& /*filename*/, std::size_t line_number, const std::string& message)
{
    std::ostringstream info_message;
    info_message << "Error at line " << line_number << std::endl
                 << message << std::endl;
    lastParseMessage = info_message.str();
}

bool ObjLoader::load(const QString& filename, QString& reason)
{
    Obj_mtl::obj_parser* parser = new Obj_mtl::obj_parser(Obj_mtl::obj_parser::translate_negative_indices /*obj_mtl::obj_parser::triangulate_faces*/);

    std::ifstream file(filename.toStdString().c_str());

    /* Association des callbacks */
    parser->info_callback(std::bind(&ObjLoader::info_callback, this, filename.toStdString(), std::placeholders::_1, std::placeholders::_2));
    parser->warning_callback(std::bind(&ObjLoader::warning_callback, this, filename.toStdString(), std::placeholders::_1, std::placeholders::_2));
    parser->error_callback(std::bind(&ObjLoader::error_callback, this, filename.toStdString(), std::placeholders::_1, std::placeholders::_2));


    parser->geometric_vertex_callback(std::bind(&ObjLoader::vertex_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    parser->vertex_normal_callback(std::bind(&ObjLoader::normal_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    parser->texture_vertex_callback(std::bind(&ObjLoader::texture_callback, this, std::placeholders::_1, std::placeholders::_2));

    parser->face_callbacks(
        //triangular_face_geometric_vertices_callback
        std::bind(&ObjLoader::add_face_T_vertices, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        //triangular_face_geometric_vertices_texture_vertices_callback
        std::bind(&ObjLoader::add_face_T_vertices_textures, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        //triangular_face_geometric_vertices_vertex_normals_callback
        std::bind(&ObjLoader::add_face_T_vertices_normals, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        //triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback
        std::bind(&ObjLoader::add_face_T_vertices_textures_normals, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        //quadrilateral_face_geometric_vertices_callback
        std::bind(&ObjLoader::add_face_Q_vertices, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
        //quadrilateral_face_geometric_vertices_texture_vertices_callback
        std::bind(&ObjLoader::add_face_Q_vertices_textures, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
        //quadrilateral_face_geometric_vertices_vertex_normals_callback
        std::bind(&ObjLoader::add_face_Q_vertices_normals, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
        //quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback
        std::bind(&ObjLoader::add_face_Q_vertices_textures_normals, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

    parser->group_name_callback(std::bind(&ObjLoader::set_group, this, std::placeholders::_1));
    parser->smoothing_group_callback(std::bind(&ObjLoader::smooth_group, this, std::placeholders::_1));
    parser->object_name_callback(std::bind(&ObjLoader::set_group, this, std::placeholders::_1));
    parser->material_name_callback(std::bind(&ObjLoader::set_material, this, std::placeholders::_1));

    QString dirname = QFileInfo(filename).absolutePath() + "/";

    mObjDir = dirname;
    parser->material_library_callback(std::bind(&ObjLoader::parse_material_library, this, dirname.toStdString(), std::placeholders::_1));

    /* Parse */
    bool result = parser->parse(file);
    std::cerr << lastParseMessage;
    reason = QString(lastParseMessage.c_str());

    delete parser;
    return result;
}


void ObjLoader::parse_material_library(const std::string& dirname, const std::string& name)
{


    std::string filename;
    filename = dirname + name;
    std::ifstream file(filename.c_str());

    //     std::cerr << "parse_material_library " << filename << std::endl;

    Obj_mtl::mtl_parser* mtlparser = new Obj_mtl::mtl_parser();

    /* Association des callbacks */
    mtlparser->info_callback(std::bind(&ObjLoader::info_callback, this, filename, std::placeholders::_1, std::placeholders::_2));
    mtlparser->warning_callback(std::bind(&ObjLoader::warning_callback, this, filename, std::placeholders::_1, std::placeholders::_2));
    mtlparser->error_callback(std::bind(&ObjLoader::error_callback, this, filename, std::placeholders::_1, std::placeholders::_2));

    mtlparser->material_name_callback(std::bind(&ObjLoader::new_material, this, std::placeholders::_1));

    mtlparser->material_Ka_callback(std::bind(&ObjLoader::material_Ka, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    mtlparser->material_Kd_callback(std::bind(&ObjLoader::material_Kd, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    mtlparser->material_Ks_callback(std::bind(&ObjLoader::material_Ks, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    mtlparser->material_Tf_callback(std::bind(&ObjLoader::material_Tf, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    mtlparser->material_shininess_callback(std::bind(&ObjLoader::material_shininess, this, std::placeholders::_1));
    mtlparser->material_map_Kd_callback(std::bind(&ObjLoader::material_map_Kd, this, std::placeholders::_1));
    mtlparser->material_map_Ks_callback(std::bind(&ObjLoader::material_map_Ks, this, std::placeholders::_1));
    mtlparser->material_map_Ns_callback(std::bind(&ObjLoader::material_map_Ns, this, std::placeholders::_1));
    mtlparser->material_map_d_callback(std::bind(&ObjLoader::material_map_d, this, std::placeholders::_1));
    mtlparser->material_bumpmap_callback(std::bind(&ObjLoader::material_bump, this, std::placeholders::_1));
    mtlparser->material_normalmap_callback(std::bind(&ObjLoader::material_normal, this, std::placeholders::_1));

    mtlparser->material_dissolve_callback(std::bind(&ObjLoader::material_dissolve, this, std::placeholders::_1));

    /* Parse */
    mtlparser->parse(file);
    std::cerr << lastParseMessage;
    delete mtlparser;
}

int ObjLoader::faceType(Face* f)
{
    int type = 4; // 0 -> full, 1 -> normales, 2 -> textures, 3 ->vertex uniquement
    if (f->have[NORMALS] && f->have[TEXTURES])
        type = 0;
    else if (f->have[NORMALS])
        type = 1;
    else if (f->have[TEXTURES])
        type = 2;
    else
        type = 3;
    return type;
}

void ObjLoader::addVerticeNormalTexturePart(ObjMesh* mesh, FaceList& faces, int num)
{
    std::vector<Face*>::iterator it = faces.begin();
    std::map<VertexObj, int> vertexBuffer;
    // Etape 1 : construire la liste des sommets (vert.norm.tex) uniques
    int addedVerticesNumber = 0;
    do {
        VertexObj v1(*this, (*it)->vertices[0], (*it)->textures[0], (*it)->normals[0]);
        vertexBuffer[v1] = ++addedVerticesNumber;
        VertexObj v2(*this, (*it)->vertices[1], (*it)->textures[1], (*it)->normals[1]);
        vertexBuffer[v2] = ++addedVerticesNumber;
        VertexObj v3(*this, (*it)->vertices[2], (*it)->textures[2], (*it)->normals[2]);
        vertexBuffer[v3] = ++addedVerticesNumber;
        if ((*it)->type == QUAD) {
            VertexObj v4(*this, (*it)->vertices[3], (*it)->textures[3], (*it)->normals[3]);
            vertexBuffer[v4] = ++addedVerticesNumber;
        }
    } while (++it != faces.end());

    // Etape 2 : numeroter les sommets
    int newnumber = 0;
    for (std::map<VertexObj, int>::iterator vm = vertexBuffer.begin(); vm != vertexBuffer.end(); ++vm)
        vm->second = newnumber++;

    // Etape 3 : construire la liste des faces
    std::vector<int> triangleBuffer;
    std::vector<int> quadBuffer;

    int reservedSize = faces.size();
    triangleBuffer.reserve(reservedSize);
    quadBuffer.reserve(reservedSize);

    it = faces.begin();
    do {
        VertexObj v1(*this, (*it)->vertices[0], (*it)->textures[0], (*it)->normals[0]);
        VertexObj v2(*this, (*it)->vertices[1], (*it)->textures[1], (*it)->normals[1]);
        VertexObj v3(*this, (*it)->vertices[2], (*it)->textures[2], (*it)->normals[2]);
        int index1 = vertexBuffer.find(v1)->second;
        int index2 = vertexBuffer.find(v2)->second;
        int index3 = vertexBuffer.find(v3)->second;

        if ((*it)->type == TRIANGLE) {
            triangleBuffer.push_back(index1);
            triangleBuffer.push_back(index2);
            triangleBuffer.push_back(index3);
        }
        else {
            VertexObj v4(*this, (*it)->vertices[3], (*it)->textures[3], (*it)->normals[3]);
            int index4 = vertexBuffer.find(v4)->second;
            quadBuffer.push_back(index1);
            quadBuffer.push_back(index2);
            quadBuffer.push_back(index3);
            quadBuffer.push_back(index4);
        }
        delete (*it);
    } while (++it != faces.end());

    // etape4 : construire le tableau de sommet final
    std::vector<float> glVertexBuffer;
    for (std::map<VertexObj, int>::iterator vm = vertexBuffer.begin(); vm != vertexBuffer.end(); ++vm) {
        glVertexBuffer.push_back(vm->first.vertex.x);
        glVertexBuffer.push_back(vm->first.vertex.y);
        glVertexBuffer.push_back(vm->first.vertex.z);
        glVertexBuffer.push_back(vm->first.normal.x);
        glVertexBuffer.push_back(vm->first.normal.y);
        glVertexBuffer.push_back(vm->first.normal.z);
        glVertexBuffer.push_back(vm->first.texcoord[0]);
        glVertexBuffer.push_back(vm->first.texcoord[1]);
    }
    // etape 5 : construire le Mesh pour le renderer
    SmoothGroup* theSmoothGroup = new SmoothGroup(glVertexBuffer, triangleBuffer, quadBuffer, true, true);
    mesh->addSmoothGroup(theSmoothGroup);

    // On nettoie tout !
    glVertexBuffer.erase(glVertexBuffer.begin(), glVertexBuffer.end());
    triangleBuffer.erase(triangleBuffer.begin(), triangleBuffer.end());
    quadBuffer.erase(quadBuffer.begin(), quadBuffer.end());
    vertexBuffer.erase(vertexBuffer.begin(), vertexBuffer.end());
}

void ObjLoader::addVerticeNormalPart(ObjMesh* mesh, FaceList& faces, int num)
{
    std::vector<Face*>::iterator it = faces.begin();
    std::map<VertexObj, int> vertexBuffer;
    // Etape 1 : construire la liste des sommets (vert.norm.tex) uniques
    int addedVerticesNumber = 0;
    do {
        VertexObj v1(*this, (*it)->vertices[0], (*it)->normals[0]);
        vertexBuffer[v1] = ++addedVerticesNumber;
        VertexObj v2(*this, (*it)->vertices[1], (*it)->normals[1]);
        vertexBuffer[v2] = ++addedVerticesNumber;
        VertexObj v3(*this, (*it)->vertices[2], (*it)->normals[2]);
        vertexBuffer[v3] = ++addedVerticesNumber;
        if ((*it)->type == QUAD) {
            VertexObj v4(*this, (*it)->vertices[3], (*it)->normals[3]);
            vertexBuffer[v4] = ++addedVerticesNumber;
        }
    } while (++it != faces.end());

    // Etape 2 : numeroter les sommets
    int newnumber = 0;
    for (std::map<VertexObj, int>::iterator vm = vertexBuffer.begin(); vm != vertexBuffer.end(); ++vm)
        vm->second = newnumber++;

    // Etape 3 : construire la liste des faces
    std::vector<int> triangleBuffer;
    std::vector<int> quadBuffer;

    int reservedSize = faces.size();
    triangleBuffer.reserve(reservedSize);
    quadBuffer.reserve(reservedSize);

    it = faces.begin();
    do {
        VertexObj v1(*this, (*it)->vertices[0], (*it)->normals[0]);
        VertexObj v2(*this, (*it)->vertices[1], (*it)->normals[1]);
        VertexObj v3(*this, (*it)->vertices[2], (*it)->normals[2]);
        int index1 = vertexBuffer.find(v1)->second;
        int index2 = vertexBuffer.find(v2)->second;
        int index3 = vertexBuffer.find(v3)->second;

        if ((*it)->type == TRIANGLE) {
            triangleBuffer.push_back(index1);
            triangleBuffer.push_back(index2);
            triangleBuffer.push_back(index3);
        }
        else {
            VertexObj v4(*this, (*it)->vertices[3], (*it)->normals[3]);
            int index4 = vertexBuffer.find(v4)->second;
            quadBuffer.push_back(index1);
            quadBuffer.push_back(index2);
            quadBuffer.push_back(index3);
            quadBuffer.push_back(index4);
        }
        delete (*it);
    } while (++it != faces.end());

    // etape4 : construire le tableau de sommet final
    std::vector<float> glVertexBuffer;
    for (std::map<VertexObj, int>::iterator vm = vertexBuffer.begin(); vm != vertexBuffer.end(); ++vm) {
        glVertexBuffer.push_back(vm->first.vertex.x);
        glVertexBuffer.push_back(vm->first.vertex.y);
        glVertexBuffer.push_back(vm->first.vertex.z);
        glVertexBuffer.push_back(vm->first.normal.x);
        glVertexBuffer.push_back(vm->first.normal.y);
        glVertexBuffer.push_back(vm->first.normal.z);
    }
    // etape 5 : construire le Mesh pour le renderer
    SmoothGroup* theSmoothGroup = new SmoothGroup(glVertexBuffer, triangleBuffer, quadBuffer, true, false);
    mesh->addSmoothGroup(theSmoothGroup);

    // On nettoie tout !
    glVertexBuffer.erase(glVertexBuffer.begin(), glVertexBuffer.end());
    triangleBuffer.erase(triangleBuffer.begin(), triangleBuffer.end());
    quadBuffer.erase(quadBuffer.begin(), quadBuffer.end());
    vertexBuffer.erase(vertexBuffer.begin(), vertexBuffer.end());
}

void ObjLoader::addVerticeTexturePart(ObjMesh* mesh, FaceList& faces, int num)
{
    std::vector<Face*>::iterator it = faces.begin();
    std::map<VertexObj, int> vertexBuffer;
    // Etape 1 : construire la liste des sommets (vert.tex) uniques
    int addedVerticesNumber = 0;
    do {
        VertexObj v1(*this, (*it)->vertices[0], (*it)->textures[0], true);
        vertexBuffer[v1] = ++addedVerticesNumber;
        VertexObj v2(*this, (*it)->vertices[1], (*it)->textures[1], true);
        vertexBuffer[v2] = ++addedVerticesNumber;
        VertexObj v3(*this, (*it)->vertices[2], (*it)->textures[2], true);
        vertexBuffer[v3] = ++addedVerticesNumber;
        if ((*it)->type == QUAD) {
            VertexObj v4(*this, (*it)->vertices[3], (*it)->textures[3], true);
            vertexBuffer[v4] = ++addedVerticesNumber;
        }
    } while (++it != faces.end());

    // Etape 2 : numeroter les sommets
    int newnumber = 0;
    for (std::map<VertexObj, int>::iterator vm = vertexBuffer.begin(); vm != vertexBuffer.end(); ++vm)
        vm->second = newnumber++;

    // Etape 3 : construire la liste des faces
    std::vector<int> triangleBuffer;
    std::vector<int> quadBuffer;

    int reservedSize = faces.size();
    triangleBuffer.reserve(reservedSize);
    quadBuffer.reserve(reservedSize);

    it = faces.begin();
    do {
        VertexObj v1(*this, (*it)->vertices[0], (*it)->textures[0], true);
        VertexObj v2(*this, (*it)->vertices[1], (*it)->textures[1], true);
        VertexObj v3(*this, (*it)->vertices[2], (*it)->textures[2], true);
        int index1 = vertexBuffer.find(v1)->second;
        int index2 = vertexBuffer.find(v2)->second;
        int index3 = vertexBuffer.find(v3)->second;

        if ((*it)->type == TRIANGLE) {
            triangleBuffer.push_back(index1);
            triangleBuffer.push_back(index2);
            triangleBuffer.push_back(index3);
        }
        else {
            VertexObj v4(*this, (*it)->vertices[3], (*it)->textures[3], true);
            int index4 = vertexBuffer.find(v4)->second;
            quadBuffer.push_back(index1);
            quadBuffer.push_back(index2);
            quadBuffer.push_back(index3);
            quadBuffer.push_back(index4);
        }
        delete (*it);
    } while (++it != faces.end());

    // etape4 : construire le tableau de sommet final
    std::vector<float> glVertexBuffer;
    for (std::map<VertexObj, int>::iterator vm = vertexBuffer.begin(); vm != vertexBuffer.end(); ++vm) {
        glVertexBuffer.push_back(vm->first.vertex.x);
        glVertexBuffer.push_back(vm->first.vertex.y);
        glVertexBuffer.push_back(vm->first.vertex.z);
        glVertexBuffer.push_back(vm->first.texcoord[0]);
        glVertexBuffer.push_back(vm->first.texcoord[1]);
    }
    // etape 5 : construire le Mesh pour le renderer
    SmoothGroup* theSmoothGroup = new SmoothGroup(glVertexBuffer, triangleBuffer, quadBuffer, false, true);
    mesh->addSmoothGroup(theSmoothGroup);

    // On nettoie tout !
    glVertexBuffer.erase(glVertexBuffer.begin(), glVertexBuffer.end());
    triangleBuffer.erase(triangleBuffer.begin(), triangleBuffer.end());
    quadBuffer.erase(quadBuffer.begin(), quadBuffer.end());
    vertexBuffer.erase(vertexBuffer.begin(), vertexBuffer.end());
}

void ObjLoader::addVerticePart(ObjMesh* mesh, FaceList& faces, int num)
{
    std::vector<Face*>::iterator it = faces.begin();
    std::map<VertexObj, int> vertexBuffer;
    // Etape 1 : construire la liste des sommets (vert) uniques
    int addedVerticesNumber = 0;
    do {
        VertexObj v1(*this, (*it)->vertices[0]);
        vertexBuffer[v1] = ++addedVerticesNumber;
        VertexObj v2(*this, (*it)->vertices[1]);
        vertexBuffer[v2] = ++addedVerticesNumber;
        VertexObj v3(*this, (*it)->vertices[2]);
        vertexBuffer[v3] = ++addedVerticesNumber;

        if ((*it)->type == QUAD) {
            VertexObj v4(*this, (*it)->vertices[3]);
            vertexBuffer[v4] = ++addedVerticesNumber;
        }
    } while (++it != faces.end());

    // Etape 2 : numeroter les sommets
    int newnumber = 0;
    for (std::map<VertexObj, int>::iterator vm = vertexBuffer.begin(); vm != vertexBuffer.end(); ++vm)
        vm->second = newnumber++;

    // Etape 3 : construire la liste des faces
    std::vector<int> triangleBuffer;
    std::vector<int> quadBuffer;

    int reservedSize = faces.size();
    triangleBuffer.reserve(reservedSize);
    quadBuffer.reserve(reservedSize);

    it = faces.begin();
    do {
        VertexObj v1(*this, (*it)->vertices[0]);
        VertexObj v2(*this, (*it)->vertices[1]);
        VertexObj v3(*this, (*it)->vertices[2]);
        int index1 = vertexBuffer.find(v1)->second;
        int index2 = vertexBuffer.find(v2)->second;
        int index3 = vertexBuffer.find(v3)->second;

        if ((*it)->type == TRIANGLE) {
            triangleBuffer.push_back(index1);
            triangleBuffer.push_back(index2);
            triangleBuffer.push_back(index3);
        }
        else {
            VertexObj v4(*this, (*it)->vertices[3]);
            int index4 = vertexBuffer.find(v4)->second;
            quadBuffer.push_back(index1);
            quadBuffer.push_back(index2);
            quadBuffer.push_back(index3);
            quadBuffer.push_back(index4);
        }
        delete (*it);
    } while (++it != faces.end());

    // etape4 : construire le tableau de sommet final
    std::vector<float> glVertexBuffer;
    for (std::map<VertexObj, int>::iterator vm = vertexBuffer.begin(); vm != vertexBuffer.end(); ++vm) {
        glVertexBuffer.push_back(vm->first.vertex.x);
        glVertexBuffer.push_back(vm->first.vertex.y);
        glVertexBuffer.push_back(vm->first.vertex.z);
    }
    // etape 5 : construire le Mesh pour le renderer
    SmoothGroup* theSmoothGroup = new SmoothGroup(glVertexBuffer, triangleBuffer, quadBuffer, false, false);
    mesh->addSmoothGroup(theSmoothGroup);

    // On nettoie tout !
    glVertexBuffer.erase(glVertexBuffer.begin(), glVertexBuffer.end());
    triangleBuffer.erase(triangleBuffer.begin(), triangleBuffer.end());
    quadBuffer.erase(quadBuffer.begin(), quadBuffer.end());
    vertexBuffer.erase(vertexBuffer.begin(), vertexBuffer.end());
}

void ObjLoader::addRawVerticeNormalTexturePart(ObjMesh* mesh, FaceList& faces, int num)
{
    std::vector<Face*>::iterator it = faces.begin();
    // Etape 1 : construire la liste des faces
    std::vector<int> triangleBuffer;
    std::vector<int> quadBuffer;
    std::vector<float> glVertexBuffer;

    int reservedSize = faces.size();
    triangleBuffer.reserve(reservedSize);
    quadBuffer.reserve(reservedSize);
    glVertexBuffer.reserve(3. * reservedSize);

    int indexVertex = 0;
    it = faces.begin();
    do {
        VertexObj v1(*this, (*it)->vertices[0], (*it)->textures[0], (*it)->normals[0]);
        VertexObj v2(*this, (*it)->vertices[1], (*it)->textures[1], (*it)->normals[1]);
        VertexObj v3(*this, (*it)->vertices[2], (*it)->textures[2], (*it)->normals[2]);

        glVertexBuffer.push_back(v1.vertex.x);
        glVertexBuffer.push_back(v1.vertex.y);
        glVertexBuffer.push_back(v1.vertex.z);
        glVertexBuffer.push_back(v1.normal.x);
        glVertexBuffer.push_back(v1.normal.y);
        glVertexBuffer.push_back(v1.normal.z);
        glVertexBuffer.push_back(v1.texcoord[0]);
        glVertexBuffer.push_back(v1.texcoord[1]);

        glVertexBuffer.push_back(v2.vertex.x);
        glVertexBuffer.push_back(v2.vertex.y);
        glVertexBuffer.push_back(v2.vertex.z);
        glVertexBuffer.push_back(v2.normal.x);
        glVertexBuffer.push_back(v2.normal.y);
        glVertexBuffer.push_back(v2.normal.z);
        glVertexBuffer.push_back(v2.texcoord[0]);
        glVertexBuffer.push_back(v2.texcoord[1]);

        glVertexBuffer.push_back(v3.vertex.x);
        glVertexBuffer.push_back(v3.vertex.y);
        glVertexBuffer.push_back(v3.vertex.z);
        glVertexBuffer.push_back(v3.normal.x);
        glVertexBuffer.push_back(v3.normal.y);
        glVertexBuffer.push_back(v3.normal.z);
        glVertexBuffer.push_back(v3.texcoord[0]);
        glVertexBuffer.push_back(v3.texcoord[1]);

        int index1 = indexVertex++;
        int index2 = indexVertex++;
        int index3 = indexVertex++;

        if ((*it)->type == TRIANGLE) {
            triangleBuffer.push_back(index1);
            triangleBuffer.push_back(index2);
            triangleBuffer.push_back(index3);
        }
        else {
            VertexObj v4(*this, (*it)->vertices[3], (*it)->textures[3], (*it)->normals[3]);
            glVertexBuffer.push_back(v4.vertex.x);
            glVertexBuffer.push_back(v4.vertex.y);
            glVertexBuffer.push_back(v4.vertex.z);
            glVertexBuffer.push_back(v4.normal.x);
            glVertexBuffer.push_back(v4.normal.y);
            glVertexBuffer.push_back(v4.normal.z);
            glVertexBuffer.push_back(v4.texcoord[0]);
            glVertexBuffer.push_back(v4.texcoord[1]);

            int index4 = indexVertex++;

            quadBuffer.push_back(index1);
            quadBuffer.push_back(index2);
            quadBuffer.push_back(index3);
            quadBuffer.push_back(index4);
        }
        delete (*it);
    } while (++it != faces.end());
    // etape 2 : construire le Mesh pour le renderer
    SmoothGroup* theSmoothGroup = new SmoothGroup(glVertexBuffer, triangleBuffer, quadBuffer, true, true);
    mesh->addSmoothGroup(theSmoothGroup);
    // On nettoie tout !
    glVertexBuffer.erase(glVertexBuffer.begin(), glVertexBuffer.end());
    triangleBuffer.erase(triangleBuffer.begin(), triangleBuffer.end());
    quadBuffer.erase(quadBuffer.begin(), quadBuffer.end());
}

void ObjLoader::addRawVerticeNormalPart(ObjMesh* mesh, FaceList& faces, int num)
{
    std::vector<Face*>::iterator it = faces.begin();
    // Etape 1 : construire la liste des faces
    std::vector<int> triangleBuffer;
    std::vector<int> quadBuffer;
    std::vector<float> glVertexBuffer;

    int reservedSize = faces.size();
    triangleBuffer.reserve(reservedSize);
    quadBuffer.reserve(reservedSize);
    glVertexBuffer.reserve(3. * reservedSize);

    int indexVertex = 0;
    it = faces.begin();
    do {
        VertexObj v1(*this, (*it)->vertices[0], (*it)->normals[0]);
        VertexObj v2(*this, (*it)->vertices[1], (*it)->normals[1]);
        VertexObj v3(*this, (*it)->vertices[2], (*it)->normals[2]);

        glVertexBuffer.push_back(v1.vertex.x);
        glVertexBuffer.push_back(v1.vertex.y);
        glVertexBuffer.push_back(v1.vertex.z);
        glVertexBuffer.push_back(v1.normal.x);
        glVertexBuffer.push_back(v1.normal.y);
        glVertexBuffer.push_back(v1.normal.z);

        glVertexBuffer.push_back(v2.vertex.x);
        glVertexBuffer.push_back(v2.vertex.y);
        glVertexBuffer.push_back(v2.vertex.z);
        glVertexBuffer.push_back(v2.normal.x);
        glVertexBuffer.push_back(v2.normal.y);
        glVertexBuffer.push_back(v2.normal.z);

        glVertexBuffer.push_back(v3.vertex.x);
        glVertexBuffer.push_back(v3.vertex.y);
        glVertexBuffer.push_back(v3.vertex.z);
        glVertexBuffer.push_back(v3.normal.x);
        glVertexBuffer.push_back(v3.normal.y);
        glVertexBuffer.push_back(v3.normal.z);

        int index1 = indexVertex++;
        int index2 = indexVertex++;
        int index3 = indexVertex++;

        if ((*it)->type == TRIANGLE) {
            triangleBuffer.push_back(index1);
            triangleBuffer.push_back(index2);
            triangleBuffer.push_back(index3);
        }
        else {
            VertexObj v4(*this, (*it)->vertices[3], (*it)->normals[3]);
            glVertexBuffer.push_back(v4.vertex.x);
            glVertexBuffer.push_back(v4.vertex.y);
            glVertexBuffer.push_back(v4.vertex.z);
            glVertexBuffer.push_back(v4.normal.x);
            glVertexBuffer.push_back(v4.normal.y);
            glVertexBuffer.push_back(v4.normal.z);

            int index4 = indexVertex++;

            quadBuffer.push_back(index1);
            quadBuffer.push_back(index2);
            quadBuffer.push_back(index3);
            quadBuffer.push_back(index4);
        }
        delete (*it);
    } while (++it != faces.end());
    // etape 2 : construire le Mesh pour le renderer
    SmoothGroup* theSmoothGroup = new SmoothGroup(glVertexBuffer, triangleBuffer, quadBuffer, true, false);
    mesh->addSmoothGroup(theSmoothGroup);
    // On nettoie tout !
    glVertexBuffer.erase(glVertexBuffer.begin(), glVertexBuffer.end());
    triangleBuffer.erase(triangleBuffer.begin(), triangleBuffer.end());
    quadBuffer.erase(quadBuffer.begin(), quadBuffer.end());
}

void ObjLoader::addRawVerticeTexturePart(ObjMesh* mesh, FaceList& faces, int num)
{
    std::vector<Face*>::iterator it = faces.begin();
    // Etape 1 : construire la liste des faces
    std::vector<int> triangleBuffer;
    std::vector<int> quadBuffer;
    std::vector<float> glVertexBuffer;

    int reservedSize = faces.size();
    triangleBuffer.reserve(reservedSize);
    quadBuffer.reserve(reservedSize);
    glVertexBuffer.reserve(3. * reservedSize);

    int indexVertex = 0;
    it = faces.begin();
    do {
        VertexObj v1(*this, (*it)->vertices[0], (*it)->textures[0], true);
        VertexObj v2(*this, (*it)->vertices[1], (*it)->textures[1], true);
        VertexObj v3(*this, (*it)->vertices[2], (*it)->textures[2], true);

        glVertexBuffer.push_back(v1.vertex.x);
        glVertexBuffer.push_back(v1.vertex.y);
        glVertexBuffer.push_back(v1.vertex.z);
        glVertexBuffer.push_back(v1.texcoord[0]);
        glVertexBuffer.push_back(v1.texcoord[1]);

        glVertexBuffer.push_back(v2.vertex.x);
        glVertexBuffer.push_back(v2.vertex.y);
        glVertexBuffer.push_back(v2.vertex.z);
        glVertexBuffer.push_back(v2.texcoord[0]);
        glVertexBuffer.push_back(v2.texcoord[1]);

        glVertexBuffer.push_back(v3.vertex.x);
        glVertexBuffer.push_back(v3.vertex.y);
        glVertexBuffer.push_back(v3.vertex.z);
        glVertexBuffer.push_back(v3.texcoord[0]);
        glVertexBuffer.push_back(v3.texcoord[1]);

        int index1 = indexVertex++;
        int index2 = indexVertex++;
        int index3 = indexVertex++;

        if ((*it)->type == TRIANGLE) {
            triangleBuffer.push_back(index1);
            triangleBuffer.push_back(index2);
            triangleBuffer.push_back(index3);
        }
        else {
            VertexObj v4(*this, (*it)->vertices[3], (*it)->textures[3], true);
            glVertexBuffer.push_back(v4.vertex.x);
            glVertexBuffer.push_back(v4.vertex.y);
            glVertexBuffer.push_back(v4.vertex.z);
            glVertexBuffer.push_back(v4.texcoord[0]);
            glVertexBuffer.push_back(v4.texcoord[1]);

            int index4 = indexVertex++;

            quadBuffer.push_back(index1);
            quadBuffer.push_back(index2);
            quadBuffer.push_back(index3);
            quadBuffer.push_back(index4);
        }
        delete (*it);
    } while (++it != faces.end());
    // etape 2 : construire le Mesh pour le renderer
    SmoothGroup* theSmoothGroup = new SmoothGroup(glVertexBuffer, triangleBuffer, quadBuffer, false, true);
    mesh->addSmoothGroup(theSmoothGroup);
    // On nettoie tout !
    glVertexBuffer.erase(glVertexBuffer.begin(), glVertexBuffer.end());
    triangleBuffer.erase(triangleBuffer.begin(), triangleBuffer.end());
    quadBuffer.erase(quadBuffer.begin(), quadBuffer.end());
}

void ObjLoader::addRawVerticePart(ObjMesh* mesh, FaceList& faces, int num)
{
    std::vector<Face*>::iterator it = faces.begin();
    // Etape 1 : construire la liste des faces
    std::vector<int> triangleBuffer;
    std::vector<int> quadBuffer;
    std::vector<float> glVertexBuffer;

    int reservedSize = faces.size();
    triangleBuffer.reserve(reservedSize);
    quadBuffer.reserve(reservedSize);
    glVertexBuffer.reserve(3. * reservedSize);

    int indexVertex = 0;
    it = faces.begin();
    do {
        VertexObj v1(*this, (*it)->vertices[0]);
        VertexObj v2(*this, (*it)->vertices[1]);
        VertexObj v3(*this, (*it)->vertices[2]);

        glVertexBuffer.push_back(v1.vertex.x);
        glVertexBuffer.push_back(v1.vertex.y);
        glVertexBuffer.push_back(v1.vertex.z);
        glVertexBuffer.push_back(v2.vertex.x);
        glVertexBuffer.push_back(v2.vertex.y);
        glVertexBuffer.push_back(v2.vertex.z);
        glVertexBuffer.push_back(v3.vertex.x);
        glVertexBuffer.push_back(v3.vertex.y);
        glVertexBuffer.push_back(v3.vertex.z);

        int index1 = indexVertex++;
        int index2 = indexVertex++;
        int index3 = indexVertex++;
        if ((*it)->type == TRIANGLE) {
            triangleBuffer.push_back(index1);
            triangleBuffer.push_back(index2);
            triangleBuffer.push_back(index3);
        }
        else {
            VertexObj v4(*this, (*it)->vertices[3]);

            glVertexBuffer.push_back(v4.vertex.x);
            glVertexBuffer.push_back(v4.vertex.y);
            glVertexBuffer.push_back(v4.vertex.z);

            int index4 = indexVertex++;

            quadBuffer.push_back(index1);
            quadBuffer.push_back(index2);
            quadBuffer.push_back(index3);
            quadBuffer.push_back(index4);
        }
        delete (*it);
    } while (++it != faces.end());
    // etape 2 : construire le Mesh pour le renderer
    SmoothGroup* theSmoothGroup = new SmoothGroup(glVertexBuffer, triangleBuffer, quadBuffer, false, false);
    mesh->addSmoothGroup(theSmoothGroup);
    // On nettoie tout !
    glVertexBuffer.erase(glVertexBuffer.begin(), glVertexBuffer.end());
    triangleBuffer.erase(triangleBuffer.begin(), triangleBuffer.end());
    quadBuffer.erase(quadBuffer.begin(), quadBuffer.end());
}


// TODO : Ecrire la transformation des objets en table de sommets/table de triangle
/*
  Pour chaque objet, construire une liste unique de sommets et de triangles, récupérer le matériau et appeler un callback pour mettre l'objet dans la scène.
 */
void ObjLoader::getObjects(std::vector<Loaders::Mesh*>& meshes)
{
    /*
void ObjLoader::addEntities (Scene * theScene, const Transform& transform) {
// add materials to the scene
	{
		for (std::map<std::string, mtlMaterial *>::iterator it = mtllib.begin(); it != mtllib.end(); ++it)
			materialAddFunction (*it, theScene);
	}
*/
    //add geometries to the scene
    {
        // add geometries
        for (std::map<std::string, Group*>::iterator group = allgroups.begin(); group != allgroups.end(); ++group) {
            Group* theGroup = group->second;
            if (!theGroup->empty) {
                ObjMesh* theMesh;
                // 				std::cerr << "Material name : " << theGroup->getMaterial() << std::endl;
                theMesh = new ObjMesh(theGroup->name /*, theScene->getMaterialByName (theGroup->getMaterial())*/);
                for (std::map<int, FaceList>::iterator sg = theGroup->faces.begin(); sg != theGroup->faces.end(); ++sg) {
                    //                 std::cerr << "Traitement de " << theGroup->name << " smooth group " << sg->first << std::endl;
                    std::vector<Face*>::iterator it = sg->second.begin();
                    if (it != sg->second.end()) {
                        // le groupe n'est pas vide !
                        int type = faceType(*it);
                        switch (type) {
                        case 0: // group with complete faces
                            if (sg->first == 0) {
                                // pas de lissage sur ce smoothgroup
                                // ajout des sommets sans reorganisation
                                addRawVerticeNormalTexturePart(theMesh, sg->second, sg->first);
                            }
                            else {
                                addVerticeNormalTexturePart(theMesh, sg->second, sg->first);
                            }
                            break;
                        case 1: // groupe avec faces vertex+normales
                            if (sg->first == 0) {
                                // pas de lissage sur ce smoothgroup
                                // ajout des sommets sans reorganisation
                                addRawVerticeNormalPart(theMesh, sg->second, sg->first);
                            }
                            else {
                                addVerticeNormalPart(theMesh, sg->second, sg->first);
                            }
                            break;
                        case 2: // groupe avec faces vertex+textures
                            if (sg->first == 0) {
                                // pas de lissage sur ce smoothgroup
                                // ajout des sommets sans reorganisation
                                addRawVerticeTexturePart(theMesh, sg->second, sg->first);
                            }
                            else {
                                addVerticeTexturePart(theMesh, sg->second, sg->first);
                            }
                            break;
                        case 3: // groupe avec faces vertex uniquement
                            if (sg->first == 0) {
                                // pas de lissage sur ce smoothgroup
                                // ajout des sommets sans reorganisation
                                addRawVerticePart(theMesh, sg->second, sg->first);
                            }
                            else {
                                addVerticePart(theMesh, sg->second, sg->first);
                            }
                            break;
                        default:
                            std::cerr << "Cas normalement impossible !" << std::endl;
                        }
                    }
                }

                meshes.push_back(theMesh->compile());
                delete theMesh;
            }
            delete theGroup;
        }
    }
}

} // end namespace obj

} // end namespace loaders
