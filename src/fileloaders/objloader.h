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
#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QString>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "objfileparser.h"
#include "objmesh.h"

#include "utils.h"
using namespace Utils;

/** @defgroup Loaders Data loading software components.
 *  Utilities to load OBJ file format.
 * @author Mathias Paulin <Mathias.Paulin@irit.fr>
 *  @{
 */

/**
  * Data loading software components.
  * This namespace contains classes for loading text files (for shaders) or geometry files.
  */

// =============================================================================
namespace Loaders {
// =============================================================================

/**
  * @ingroup Loaders
  * Wavefront OBJ file format loader.
  * This namespace contains classes for loading .obj and .mtl files.
  */
// =============================================================================
namespace Obj_mtl {
// =============================================================================

/** @defgroup OBJ-MTL Alias/Wavefront OBJ and MTL file format.
 * @ingroup Loaders
 * @author Mathias Paulin <Mathias.Paulin@irit.fr>
 *  @{
 */


/** @ingroup OBJ-MTL
    Class to load geometry in OBJ/MTL format.
    @author Mathias Paulin <Mathias.Paulin@irit.fr>
*/
class ObjLoader {
public:
    ObjLoader();

    ~ObjLoader();

    /// load the file at "filename" into memory of #ObjLoader
    /// use #getObjects() to retreive the meshes.
    /// @param reason : error message if any
    /// @return if loaded correctly or not
    bool load(const QString& filename, QString& reason);

    /// Get the loaded meshes after calling #load().
    ///  An OBJ defines one or several meshes therefore we return a vector
    ///  "meshes"
    void getObjects(std::vector<Loaders::Mesh*>& meshes);

    // sous classes et methodes
private:
    class mtlMaterial;
    typedef std::pair<std::string, mtlMaterial*> NamedMaterial;

    std::string lastParseMessage;

    // table des sommets, normales et coordtextures
    std::vector<glm::vec3> verticesTable;
    int vertices;
    std::vector<glm::vec3> normalsTable;
    int normals;
    std::vector<glm::vec3> texturesTable;
    int textures;

    //folder in which the model is stored (for paths to textures)
    QString mObjDir;

    // triangular faces definition
    enum FaceVertexElement { NORMALS = 0,
                             TEXTURES };
    /** @ingroup OBJ-MTL
                   OBJ vertex.
                */
    class VertexObj {
        friend class ObjLoader;
        glm::vec3 vertex;
        glm::vec3 normal;
        float texcoord[2];
        bool have[2];

    public:
        VertexObj(const ObjLoader& loader, int v)
        {
            vertex = loader.verticesTable[v];
            texcoord[0] = texcoord[1] = 0.0f;
            have[NORMALS] = have[TEXTURES] = false;
        }
        VertexObj(const ObjLoader& loader, int v, int t, bool)
        {
            vertex = loader.verticesTable[v];
            texcoord[0] = loader.texturesTable[t].x;
            texcoord[1] = loader.texturesTable[t].y;
            have[NORMALS] = false;
            have[TEXTURES] = true;
        }
        VertexObj(const ObjLoader& loader, int v, int n)
        {
            vertex = loader.verticesTable[v];
            normal = loader.normalsTable[n];
            texcoord[0] = texcoord[1] = 0.0f;
            have[NORMALS] = true;
            have[TEXTURES] = false;
        }
        VertexObj(const ObjLoader& loader, int v, int t, int n)
        {
            vertex = loader.verticesTable[v];
            normal = loader.normalsTable[n];
            texcoord[0] = loader.texturesTable[t].x;
            texcoord[1] = loader.texturesTable[t].y;
            have[NORMALS] = true;
            have[TEXTURES] = true;
        }
        friend bool operator<(const VertexObj& a, const VertexObj& b)
        {
            if (a == b)
                return false;
            else if (a.vertex == b.vertex) {
                // comparer la normale
                if (a.normal == b.normal) {
                    // comparer les text coord
                    if (a.texcoord[0] < b.texcoord[0])
                        return true;
                    else if (a.texcoord[0] > b.texcoord[0])
                        return false;
                    else
                        return (a.texcoord[1] < b.texcoord[1]);
                }
                else {
                    // comparer les normales
                    if (a.normal.x < b.normal.x)
                        return true;
                    else if (a.normal.x > b.normal.x)
                        return false;
                    else if (a.normal.y < b.normal.y)
                        return true;
                    else if (a.normal.y > b.normal.y)
                        return false;
                    else
                        return (a.normal.z < b.normal.z);
                }
            }
            else {
                if (a.vertex.x < b.vertex.x)
                    return true;
                else if (a.vertex.x > b.vertex.x)
                    return false;
                else if (a.vertex.y < b.vertex.y)
                    return true;
                else if (a.vertex.y > b.vertex.y)
                    return false;
                else
                    return (a.vertex.z < b.vertex.z);
            }
        }

        friend bool operator==(const VertexObj& a, const VertexObj& b)
        {
            return ((a.vertex == b.vertex) && (a.normal == b.normal) && (a.texcoord[0] == b.texcoord[0]) && (a.texcoord[1] == b.texcoord[1]));
        }

        friend std::ostream& operator<<(std::ostream& os, const VertexObj& a)
        {
            os << "Pos : " << glm::to_string(a.vertex) << " -- Norm : " << glm::to_string(a.normal) << " -- Tex : " << a.texcoord[0] << " " << a.texcoord[1] << " ";
            return os;
        }
    };

    enum FaceType { TRIANGLE = 0,
                    QUAD };
    /** @ingroup OBJ-MTL
                   OBJ face.
                */
    class Face {
        friend class ObjLoader;
        int vertices[4];
        int normals[4];
        int textures[4];
        bool have[2];
        FaceType type;

    public:
        Face(int s0, int s1, int s2)
        {
            vertices[0] = s0;
            vertices[1] = s1;
            vertices[2] = s2;
            have[NORMALS] = have[TEXTURES] = false;
            type = TRIANGLE;
        }
        Face(int s0, int s1, int s2, int s3)
        {
            vertices[0] = s0;
            vertices[1] = s1;
            vertices[2] = s2;
            vertices[3] = s3;
            have[NORMALS] = have[TEXTURES] = false;
            type = QUAD;
        }

        Face(int s0, int s1, int s2, int n0, int n1, int n2)
        {
            vertices[0] = s0;
            vertices[1] = s1;
            vertices[2] = s2;
            normals[0] = n0;
            normals[1] = n1;
            normals[2] = n2;
            have[NORMALS] = true;
            have[TEXTURES] = false;
            type = TRIANGLE;
        }

        Face(int s0, int s1, int s2, int s3, int n0, int n1, int n2, int n3)
        {
            vertices[0] = s0;
            vertices[1] = s1;
            vertices[2] = s2;
            vertices[3] = s3;
            normals[0] = n0;
            normals[1] = n1;
            normals[2] = n2;
            normals[3] = n3;
            have[NORMALS] = true;
            have[TEXTURES] = false;
            type = QUAD;
        }

        Face(int s0, int s1, int s2, int t0, int t1, int t2, bool)
        {
            vertices[0] = s0;
            vertices[1] = s1;
            vertices[2] = s2;
            textures[0] = t0;
            textures[1] = t1;
            textures[2] = t2;
            have[NORMALS] = false;
            have[TEXTURES] = true;
            type = TRIANGLE;
        }

        Face(int s0, int s1, int s2, int s3, int t0, int t1, int t2, int t3, bool)
        {
            vertices[0] = s0;
            vertices[1] = s1;
            vertices[2] = s2;
            vertices[3] = s3;
            textures[0] = t0;
            textures[1] = t1;
            textures[2] = t2;
            textures[3] = t3;
            have[NORMALS] = false;
            have[TEXTURES] = true;
            type = QUAD;
        }

        Face(int s0, int s1, int s2, int n0, int n1, int n2, int t0, int t1, int t2)
        {
            vertices[0] = s0;
            vertices[1] = s1;
            vertices[2] = s2;
            normals[0] = n0;
            normals[1] = n1;
            normals[2] = n2;
            textures[0] = t0;
            textures[1] = t1;
            textures[2] = t2;
            have[NORMALS] = have[TEXTURES] = true;
            type = TRIANGLE;
        }

        Face(int s0, int s1, int s2, int s3, int n0, int n1, int n2, int n3, int t0, int t1, int t2, int t3)
        {
            vertices[0] = s0;
            vertices[1] = s1;
            vertices[2] = s2;
            vertices[3] = s3;
            normals[0] = n0;
            normals[1] = n1;
            normals[2] = n2;
            normals[3] = n3;
            textures[0] = t0;
            textures[1] = t1;
            textures[2] = t2;
            textures[3] = t3;
            have[NORMALS] = have[TEXTURES] = true;
            type = QUAD;
        }
    };
    typedef std::vector<Face*> FaceList;

    /** @ingroup OBJ-MTL
                   OBJ group.
                */
    class Group {
        friend class ObjLoader;
        std::string name;
        std::string material;
        int smoothGroup;
        std::map<int, FaceList> faces;
        bool empty;

    public:
        Group(std::string n)
            : name(n)
            , material("default")
            , smoothGroup(0)
        {
            empty = true;
        }
        void setSmoothGroup(int s)
        {
            smoothGroup = s;
        }
        void setMaterial(std::string m)
        {
            material = m;
        }
        std::string getMaterial()
        {
            return material;
        }
        void addFace(Face* f)
        {
            empty = false;
            faces[smoothGroup].push_back(f);
        }
        FaceList& getFaces(int s = 0)
        {
            return faces[s];
        }
    };
    Group* currentGroup;
    std::string currentName;
    std::string currentMaterialName;
    std::map<std::string, Group*> allgroups;
    int groupsNumber;

    /** @ingroup OBJ-MTL
                   OBJ material.
                */
    class mtlMaterial {
    public:
        std::string name;

        float Ka[3];
        float Kd[3];
        float Ks[3];
        float Tf[3];

        int illum;
        float shininess;
        float sharpness;
        float dissolve;
        float ior;

        //std::string dissolve;
        std::string map_ka;
        glm::vec3 map_ka_scale;

        std::string map_kd;
        glm::vec3 map_kd_scale;

        std::string map_ks;
        glm::vec3 map_ks_scale;

        std::string map_Ns;
        glm::vec3 map_Ns_scale;

        std::string map_d;
        glm::vec3 map_d_scale;

        std::string dispmap;

        std::string decalmap;

        std::string normalmap;

        std::string bumpmap;
        glm::vec3 bumpmap_scale;

        std::string reflmap;

        mtlMaterial(std::string n)
            : name(n)
        {
            map_kd_scale = glm::vec3(1.0, 1.0, 1.0);
            map_ks_scale = glm::vec3(1.0, 1.0, 1.0);
            map_Ns_scale = glm::vec3(1.0, 1.0, 1.0);
            bumpmap_scale = glm::vec3(1.0, 1.0, 1.0);
            dissolve = 1.;
        }
    };
    mtlMaterial* currentMaterial;
    std::map<std::string, mtlMaterial*> mtllib;
    int materialNumber;

    int faceType(Face* f);

    // -------------------------

private:
    void addVerticeNormalTexturePart(ObjMesh* mesh, FaceList& faces, int num);
    void addVerticeNormalPart(ObjMesh* mesh, FaceList& faces, int num);
    void addVerticeTexturePart(ObjMesh* mesh, FaceList& faces, int num);
    void addVerticePart(ObjMesh* mesh, FaceList& faces, int num);

    void addRawVerticeNormalTexturePart(ObjMesh* mesh, FaceList& faces, int num);
    void addRawVerticeNormalPart(ObjMesh* mesh, FaceList& faces, int num);
    void addRawVerticeTexturePart(ObjMesh* mesh, FaceList& faces, int num);
    void addRawVerticePart(ObjMesh* mesh, FaceList& faces, int num);

protected:
    // Callbacks de log
    void info_callback(const std::string& filename, std::size_t line_number, const std::string& message);
    void warning_callback(const std::string& filename, std::size_t line_number, const std::string& message);
    void error_callback(const std::string& filename, std::size_t line_number, const std::string& message);

    // Callback de sommets
    void vertex_callback(float x, float y, float z)
    {
        verticesTable.push_back(glm::vec3(x, y, z));
        vertices++;
    }
    void normal_callback(float nx, float ny, float nz)
    {
        normalsTable.push_back(glm::vec3(nx, ny, nz));
        normals++;
    }
    void texture_callback(float u, float v)
    {
        texturesTable.push_back(glm::vec3(u, v, 0.0));
        textures++;
    }

    // Callbacks de faces
    void add_face_T_vertices(int i0, int i1, int i2)
    {
        currentGroup->addFace(new Face(i0 - 1, i1 - 1, i2 - 1));
    }
    void add_face_Q_vertices(int i0, int i1, int i2, int i3)
    {
        currentGroup->addFace(new Face(i0 - 1, i1 - 1, i2 - 1, i3 - 1));
    }

    void add_face_T_vertices_textures(const Obj_mtl::index_2_tuple_type& v1_vt1, const Obj_mtl::index_2_tuple_type& v2_vt2, const Obj_mtl::index_2_tuple_type& v3_vt3)
    {
        currentGroup->addFace(new Face(
            std::get<0>(v1_vt1) - 1,
            std::get<0>(v2_vt2) - 1,
            std::get<0>(v3_vt3) - 1,
            std::get<1>(v1_vt1) - 1,
            std::get<1>(v2_vt2) - 1,
            std::get<1>(v3_vt3) - 1,
            true));
    }

    void add_face_Q_vertices_textures(const Obj_mtl::index_2_tuple_type& v1_vt1, const Obj_mtl::index_2_tuple_type& v2_vt2, const Obj_mtl::index_2_tuple_type& v3_vt3, const Obj_mtl::index_2_tuple_type& v4_vt4)
    {
        currentGroup->addFace(new Face(
            std::get<0>(v1_vt1) - 1,
            std::get<0>(v2_vt2) - 1,
            std::get<0>(v3_vt3) - 1,
            std::get<0>(v4_vt4) - 1,
            std::get<1>(v1_vt1) - 1,
            std::get<1>(v2_vt2) - 1,
            std::get<1>(v3_vt3) - 1,
            std::get<1>(v4_vt4) - 1, true));
    }

    void add_face_T_vertices_normals(const Obj_mtl::index_2_tuple_type& v1_vn1, const Obj_mtl::index_2_tuple_type& v2_vn2, const Obj_mtl::index_2_tuple_type& v3_vn3)
    {
        currentGroup->addFace(new Face(
            std::get<0>(v1_vn1) - 1,
            std::get<0>(v2_vn2) - 1,
            std::get<0>(v3_vn3) - 1,
            std::get<1>(v1_vn1) - 1,
            std::get<1>(v2_vn2) - 1,
            std::get<1>(v3_vn3) - 1));
    }

    void add_face_Q_vertices_normals(const Obj_mtl::index_2_tuple_type& v1_vn1, const Obj_mtl::index_2_tuple_type& v2_vn2, const Obj_mtl::index_2_tuple_type& v3_vn3, const Obj_mtl::index_2_tuple_type& v4_vn4)
    {
        currentGroup->addFace(new Face(
            std::get<0>(v1_vn1) - 1,
            std::get<0>(v2_vn2) - 1,
            std::get<0>(v3_vn3) - 1,
            std::get<0>(v4_vn4) - 1,
            std::get<1>(v1_vn1) - 1,
            std::get<1>(v2_vn2) - 1,
            std::get<1>(v3_vn3) - 1,
            std::get<1>(v4_vn4) - 1));
    }

    void add_face_T_vertices_textures_normals(const Obj_mtl::index_3_tuple_type& v1_vtn1, const Obj_mtl::index_3_tuple_type& v2_vtn2, const Obj_mtl::index_3_tuple_type& v3_vtn3)
    {
        currentGroup->addFace(new Face(
            std::get<0>(v1_vtn1) - 1,
            std::get<0>(v2_vtn2) - 1,
            std::get<0>(v3_vtn3) - 1,
            std::get<2>(v1_vtn1) - 1,
            std::get<2>(v2_vtn2) - 1,
            std::get<2>(v3_vtn3) - 1,
            std::get<1>(v1_vtn1) - 1,
            std::get<1>(v2_vtn2) - 1,
            std::get<1>(v3_vtn3) - 1));
    }

    void add_face_Q_vertices_textures_normals(const Obj_mtl::index_3_tuple_type& v1_vtn1, const Obj_mtl::index_3_tuple_type& v2_vtn2, const Obj_mtl::index_3_tuple_type& v3_vtn3, const Obj_mtl::index_3_tuple_type& v4_vtn4)
    {
        currentGroup->addFace(new Face(
            std::get<0>(v1_vtn1) - 1,
            std::get<0>(v2_vtn2) - 1,
            std::get<0>(v3_vtn3) - 1,
            std::get<0>(v4_vtn4) - 1,
            std::get<2>(v1_vtn1) - 1,
            std::get<2>(v2_vtn2) - 1,
            std::get<2>(v3_vtn3) - 1,
            std::get<2>(v4_vtn4) - 1,
            std::get<1>(v1_vtn1) - 1,
            std::get<1>(v2_vtn2) - 1,
            std::get<1>(v3_vtn3) - 1,
            std::get<1>(v4_vtn4) - 1));
    }

    // Callback de groupes
    void set_group(const std::string& name)
    {
        std::map<std::string, Group*>::iterator gr = allgroups.find(name);
        if (gr == allgroups.end()) {
            currentGroup = new Group(name);
            currentName = name;
            allgroups[name] = currentGroup;
            groupsNumber++;
        }
        else {
            if (name != "default")
                std::cerr << "WARNING : duplicate group " << name << std::endl;
            currentGroup = gr->second;
            currentName = currentGroup->name;
        }
    }

    // Callback de smoothing-group
    void smooth_group(int num)
    {
        currentGroup->setSmoothGroup(num);
    }
    // Callback de materiau
    void set_material(const std::string& name)
    {
        currentGroup->setMaterial(name);
    }

    void parse_material_library(const std::string& dirname, const std::string& filename);

    // Callback de materiau
    void new_material(const std::string& name)
    {
        std::map<std::string, mtlMaterial*>::iterator mat = mtllib.find(name);
        if (mat == mtllib.end()) {
            currentMaterial = new mtlMaterial(name);
            mtllib[name] = currentMaterial;
            materialNumber++;
        }
        else {
            std::cerr << "WARNING : duplicate material " << name << std::endl;
            currentMaterial = mat->second;
        }
    }

    void material_map_Kd(std::string texture)
    {
        // analyse de la chaine
        if (texture[0] == '-') {
            char minus, arg;
            std::istringstream stringstream(texture);
            stringstream.unsetf(std::ios_base::skipws);
            stringstream >> minus >> arg >> std::ws;
            switch (arg) {
            case 's':
                float sx, sy, sz;
                stringstream >> sx >> std::ws >> sy >> std::ws >> sz >> std::ws;
                currentMaterial->map_kd_scale = glm::vec3(sx, sy, sz);
                break;
            default:
                std::cerr << "material_map_Kd : argument inconnu " << arg << std::endl;
            }
            std::string filename;
            stringstream >> filename;
            currentMaterial->map_kd = filename;
        }
        else {
            std::istringstream stringstream(texture);
            stringstream.unsetf(std::ios_base::skipws);
            std::string filename;
            stringstream >> filename;
            currentMaterial->map_kd = filename;
        }
    }


    void material_map_Ks(std::string texture)
    {
        // analyse de la chaine
        if (texture[0] == '-') {
            char minus, arg;
            std::istringstream stringstream(texture);
            stringstream.unsetf(std::ios_base::skipws);
            stringstream >> minus >> arg >> std::ws;
            switch (arg) {
            case 's':
                float sx, sy, sz;
                stringstream >> sx >> std::ws >> sy >> std::ws >> sz >> std::ws;
                currentMaterial->map_ks_scale = glm::vec3(sx, sy, sz);
                break;
            default:
                std::cerr << "material_map_Ks : argument inconnu " << arg << std::endl;
            }
            std::string filename;
            stringstream >> filename;
            currentMaterial->map_ks = filename;
        }
        else {
            std::istringstream stringstream(texture);
            stringstream.unsetf(std::ios_base::skipws);
            std::string filename;
            stringstream >> filename;
            currentMaterial->map_ks = filename;
        }
    }

    void material_map_Ns(std::string texture)
    {
        // analyse de la chaine
        if (texture[0] == '-') {
            char minus, arg;
            std::istringstream stringstream(texture);
            stringstream.unsetf(std::ios_base::skipws);
            stringstream >> minus >> arg >> std::ws;
            switch (arg) {
            case 's':
                float sx, sy, sz;
                stringstream >> sx >> std::ws >> sy >> std::ws >> sz >> std::ws;
                currentMaterial->map_Ns_scale = glm::vec3(sx, sy, sz);
                break;
            default:
                std::cerr << "material_map_Ns : argument inconnu " << arg << std::endl;
            }
            std::string filename;
            stringstream >> filename;
            currentMaterial->map_Ns = filename;
        }
        else {
            std::istringstream stringstream(texture);
            stringstream.unsetf(std::ios_base::skipws);
            std::string filename;
            stringstream >> filename;
            currentMaterial->map_Ns = filename;
        }
    }


    void material_map_d(std::string texture)
    {
        // analyse de la chaine
        if (texture[0] == '-') {
            char minus, arg;
            std::istringstream stringstream(texture);
            stringstream.unsetf(std::ios_base::skipws);
            stringstream >> minus >> arg >> std::ws;
            switch (arg) {
            case 's':
                float sx, sy, sz;
                stringstream >> sx >> std::ws >> sy >> std::ws >> sz >> std::ws;
                currentMaterial->map_d_scale = glm::vec3(sx, sy, sz);
                break;
            default:
                std::cerr << "material_map_d : argument inconnu " << arg << std::endl;
            }
            std::string filename;
            stringstream >> filename;
            currentMaterial->map_d = filename;
        }
        else {
            std::istringstream stringstream(texture);
            stringstream.unsetf(std::ios_base::skipws);
            std::string filename;
            stringstream >> filename;
            currentMaterial->map_d = filename;
        }
    }


    void material_bump(std::string texture)
    {
        // analyse de la chaine
        if (texture[0] == '-') {
            char minus, arg;
            std::istringstream stringstream(texture);
            stringstream.unsetf(std::ios_base::skipws);
            stringstream >> minus >> arg >> std::ws;
            switch (arg) {
            case 's':
                float sx, sy, sz;
                stringstream >> sx >> std::ws >> sy >> std::ws >> sz >> std::ws;
                currentMaterial->bumpmap_scale = glm::vec3(sx, sy, sz);
                break;
            default:
                std::cerr << "material_bump : argument inconnu " << arg << std::endl;
            }
            std::string filename;
            stringstream >> filename;
            currentMaterial->bumpmap = filename;
        }
        else {
            std::istringstream stringstream(texture);
            stringstream.unsetf(std::ios_base::skipws);
            std::string filename;
            stringstream >> filename;
            currentMaterial->bumpmap = filename;
        }
    }

    void material_normal(std::string texture)
    {
        std::cerr << "material_normal : " << texture << std::endl;
        std::istringstream stringstream(texture);
        stringstream.unsetf(std::ios_base::skipws);
        std::string filename;
        stringstream >> filename;
        currentMaterial->normalmap = filename;
    }


    void material_Ka(float r, float g, float b)
    {
        currentMaterial->Ka[0] = r;
        currentMaterial->Ka[1] = g;
        currentMaterial->Ka[2] = b;
    }
    void material_Kd(float r, float g, float b)
    {
        currentMaterial->Kd[0] = r;
        currentMaterial->Kd[1] = g;
        currentMaterial->Kd[2] = b;
    }
    void material_Ks(float r, float g, float b)
    {
        currentMaterial->Ks[0] = r;
        currentMaterial->Ks[1] = g;
        currentMaterial->Ks[2] = b;
    }
    void material_Tf(float r, float g, float b)
    {
        currentMaterial->Tf[0] = r;
        currentMaterial->Tf[1] = g;
        currentMaterial->Tf[2] = b;
    }
    void material_shininess(float n)
    {
        currentMaterial->shininess = n;
    }

    void material_dissolve(std::string d)
    {
        if (d[0] != '-') {
            float value;
            std::istringstream stringstream(d);
            stringstream >> value;
            currentMaterial->dissolve = value;
        }
        else {
            std::cerr << "TODO : manage parameterized dissolve" << std::endl;
        }
    }
};

} // END namespace obj =========================================================

/** @} */ // end of OBJ-MTL group

} // END namespace loaders =====================================================

/** @} */ // end of Loaders group

#endif // OBJLOADER_H
