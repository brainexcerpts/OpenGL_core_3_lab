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
#ifndef OBJFILEPARSER_H
#define OBJFILEPARSER_H
#include <cstddef>
#include <tuple>
#include <fstream>
#include <istream>
#include <string>
#include <cctype>
#include <functional>

// =============================================================================
namespace Loaders {
// =============================================================================

// =============================================================================
namespace Obj_mtl {
// =============================================================================


/** @ingroup OBJ-MTL
  *  @{
  */
typedef std::size_t size_type;
typedef int index_type;
typedef float float_type;
typedef std::tuple<index_type, index_type> index_2_tuple_type;
typedef std::tuple<index_type, index_type, index_type> index_3_tuple_type;
typedef std::tuple<float_type, float_type, float_type> float_3_tuple_type;
/** @} */

/** @ingroup OBJ-MTL
          * Class to parse MTL file.
          */
class mtl_parser {
public:
    typedef std::function<void(std::size_t, const std::string&)> info_callback_type;
    typedef std::function<void(std::size_t, const std::string&)> warning_callback_type;
    typedef std::function<void(std::size_t, const std::string&)> error_callback_type;

    typedef std::function<void(const std::string&)> material_name_callback_type;

    typedef std::function<void(float_type, float_type, float_type)> material_Ka_callback_type;
    typedef std::function<void(float_type, float_type, float_type)> material_Kd_callback_type;
    typedef std::function<void(float_type, float_type, float_type)> material_Ks_callback_type;
    typedef std::function<void(float_type, float_type, float_type)> material_Tf_callback_type;
    typedef std::function<void(index_type)> material_Illum_callback_type;
    typedef std::function<void(const std::string&)> material_dissolve_callback_type;
    typedef std::function<void(float_type)> material_shininess_callback_type;
    typedef std::function<void(float_type)> material_sharpness_callback_type;
    typedef std::function<void(float_type)> material_ior_callback_type;

    typedef std::function<void(const std::string&)> material_map_Ka_callback_type;
    typedef std::function<void(const std::string&)> material_map_Kd_callback_type;
    typedef std::function<void(const std::string&)> material_map_Ks_callback_type;
    typedef std::function<void(const std::string&)> material_map_Ns_callback_type;
    typedef std::function<void(const std::string&)> material_map_d_callback_type;
    typedef std::function<void(const std::string&)> material_dispmap_callback_type;
    typedef std::function<void(const std::string&)> material_decalmap_callback_type;
    typedef std::function<void(const std::string&)> material_bumpmap_callback_type;
    typedef std::function<void(const std::string&)> material_reflmap_callback_type;
    typedef std::function<void(const std::string&)> material_normalmap_callback_type;

    typedef std::function<void(const std::string&)> comment_callback_type;

    typedef int flags_type;

    typedef enum {
        parse_blank_lines_as_comment = 1 << 0,
        texture_filenameonly = 1 << 1
    } ParseOptions;
    mtl_parser(flags_type flags = 0);
    void info_callback(const info_callback_type& info_callback);
    void warning_callback(const warning_callback_type& warning_callback);
    void error_callback(const error_callback_type& error_callback);

    void material_name_callback(const material_name_callback_type& name_callback);

    void material_Ka_callback(const material_Ka_callback_type& Ka_callback);
    void material_Kd_callback(const material_Kd_callback_type& Kd_callback);
    void material_Ks_callback(const material_Ks_callback_type& Ks_callback);
    void material_Tf_callback(const material_Tf_callback_type& Tf_callback);
    void material_Illum_callback(const material_Illum_callback_type& Illum_callback);
    void material_dissolve_callback(const material_dissolve_callback_type& dissolve_callback);
    void material_shininess_callback(const material_shininess_callback_type& shininess_callback);
    void material_sharpness_callback(const material_sharpness_callback_type& sharpness_callback);
    void material_ior_callback(const material_ior_callback_type& ior_callback);

    void material_map_Ka_callback(const material_map_Ka_callback_type& map_Ka_callback);
    void material_map_Kd_callback(const material_map_Kd_callback_type& map_Kd_callback);
    void material_map_Ks_callback(const material_map_Ks_callback_type& map_Ks_callback);
    void material_map_Ns_callback(const material_map_Ns_callback_type& map_Ns_callback);
    void material_map_d_callback(const material_map_d_callback_type& map_d_callback);
    void material_dispmap_callback(const material_dispmap_callback_type& dispmap_callback);
    void material_decalmap_callback(const material_decalmap_callback_type& decalmap_callback);
    void material_bumpmap_callback(const material_bumpmap_callback_type& bumpmap_callback);
    void material_reflmap_callback(const material_reflmap_callback_type& reflmap_callback);
    void material_normalmap_callback(const material_normalmap_callback_type& normalmap_callback);
    void comment_callback(const comment_callback_type& comment_callback);

    bool parse(std::istream& istream);
    bool parse(const std::string& filename);

private:
    flags_type flags_;
    info_callback_type info_callback_;
    warning_callback_type warning_callback_;
    error_callback_type error_callback_;
    material_name_callback_type material_name_callback_;
    material_Ka_callback_type material_Ka_callback_;
    material_Kd_callback_type material_Kd_callback_;
    material_Ks_callback_type material_Ks_callback_;
    material_Tf_callback_type material_Tf_callback_;
    material_Illum_callback_type material_Illum_callback_;
    material_dissolve_callback_type material_dissolve_callback_;
    material_shininess_callback_type material_shininess_callback_;
    material_sharpness_callback_type material_sharpness_callback_;
    material_ior_callback_type material_ior_callback_;
    material_map_Ka_callback_type material_map_Ka_callback_;
    material_map_Kd_callback_type material_map_Kd_callback_;
    material_map_Ks_callback_type material_map_Ks_callback_;
    material_map_Ns_callback_type material_map_Ns_callback_;
    material_map_d_callback_type material_map_d_callback_;
    material_dispmap_callback_type material_dispmap_callback_;
    material_decalmap_callback_type material_decalmap_callback_;
    material_bumpmap_callback_type material_bumpmap_callback_;
    material_reflmap_callback_type material_reflmap_callback_;
    material_normalmap_callback_type material_normalmap_callback_;
    comment_callback_type comment_callback_;
};


/** @ingroup OBJ-MTL
          * Class to parse OBJ file.
          */
class obj_parser {
public:
    typedef std::function<void(std::size_t, const std::string&)> info_callback_type;
    typedef std::function<void(std::size_t, const std::string&)> warning_callback_type;
    typedef std::function<void(std::size_t, const std::string&)> error_callback_type;

    typedef std::function<void(float_type, float_type, float_type)> geometric_vertex_callback_type;
    typedef std::function<void(float_type, float_type)> texture_vertex_callback_type;
    typedef std::function<void(float_type, float_type, float_type)> vertex_normal_callback_type;

    typedef std::function<void(index_type, index_type, index_type)> triangular_face_geometric_vertices_callback_type;
    typedef std::function<void(const index_2_tuple_type&, const index_2_tuple_type&, const index_2_tuple_type&)> triangular_face_geometric_vertices_texture_vertices_callback_type;
    typedef std::function<void(const index_2_tuple_type&, const index_2_tuple_type&, const index_2_tuple_type&)> triangular_face_geometric_vertices_vertex_normals_callback_type;
    typedef std::function<void(const index_3_tuple_type&, const index_3_tuple_type&, const index_3_tuple_type&)> triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback_type;

    typedef std::function<void(index_type, index_type, index_type, index_type)> quadrilateral_face_geometric_vertices_callback_type;
    typedef std::function<void(const index_2_tuple_type&, const index_2_tuple_type&, const index_2_tuple_type&, const index_2_tuple_type&)> quadrilateral_face_geometric_vertices_texture_vertices_callback_type;
    typedef std::function<void(const index_2_tuple_type&, const index_2_tuple_type&, const index_2_tuple_type&, const index_2_tuple_type&)> quadrilateral_face_geometric_vertices_vertex_normals_callback_type;
    typedef std::function<void(const index_3_tuple_type&, const index_3_tuple_type&, const index_3_tuple_type&, const index_3_tuple_type&)> quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback_type;

    typedef std::function<void(index_type, index_type, index_type)> polygonal_face_geometric_vertices_begin_callback_type;
    typedef std::function<void(index_type)> polygonal_face_geometric_vertices_vertex_callback_type;
    typedef std::function<void()> polygonal_face_geometric_vertices_end_callback_type;
    typedef std::function<void(const index_2_tuple_type&, const index_2_tuple_type&, const index_2_tuple_type&)> polygonal_face_geometric_vertices_texture_vertices_begin_callback_type;
    typedef std::function<void(const index_2_tuple_type&)> polygonal_face_geometric_vertices_texture_vertices_vertex_callback_type;
    typedef std::function<void()> polygonal_face_geometric_vertices_texture_vertices_end_callback_type;
    typedef std::function<void(const index_2_tuple_type&, const index_2_tuple_type&, const index_2_tuple_type&)> polygonal_face_geometric_vertices_vertex_normals_begin_callback_type;
    typedef std::function<void(const index_2_tuple_type&)> polygonal_face_geometric_vertices_vertex_normals_vertex_callback_type;
    typedef std::function<void()> polygonal_face_geometric_vertices_vertex_normals_end_callback_type;
    typedef std::function<void(const index_3_tuple_type&, const index_3_tuple_type&, const index_3_tuple_type&)> polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback_type;
    typedef std::function<void(const index_3_tuple_type&)> polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback_type;
    typedef std::function<void()> polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback_type;

    typedef std::function<void(const std::string&)> group_name_callback_type;
    typedef std::function<void(size_type)> smoothing_group_callback_type;
    typedef std::function<void(const std::string&)> object_name_callback_type;
    typedef std::function<void(const std::string&)> material_library_callback_type;
    typedef std::function<void(const std::string&)> material_name_callback_type;
    typedef std::function<void(const std::string&)> comment_callback_type;

    typedef int flags_type;

    typedef enum {
        parse_blank_lines_as_comment = 1 << 0,
        triangulate_faces = 1 << 1,
        translate_negative_indices = 1 << 2
    } ParseOptions;
    obj_parser(flags_type flags = 0);
    void info_callback(const info_callback_type& info_callback);
    void warning_callback(const warning_callback_type& warning_callback);
    void error_callback(const error_callback_type& error_callback);
    void geometric_vertex_callback(const geometric_vertex_callback_type& geometric_vertex_callback);
    void texture_vertex_callback(const texture_vertex_callback_type& texture_vertex_callback);
    void vertex_normal_callback(const vertex_normal_callback_type& vertex_normal_callback);
    void face_callbacks(const triangular_face_geometric_vertices_callback_type& triangular_face_geometric_vertices_callback, const triangular_face_geometric_vertices_texture_vertices_callback_type& triangular_face_geometric_vertices_texture_vertices_callback, const triangular_face_geometric_vertices_vertex_normals_callback_type& triangular_face_geometric_vertices_vertex_normals_callback, const triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback_type& triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback, const quadrilateral_face_geometric_vertices_callback_type& quadrilateral_face_geometric_vertices_callback, const quadrilateral_face_geometric_vertices_texture_vertices_callback_type& quadrilateral_face_geometric_vertices_texture_vertices_callback, const quadrilateral_face_geometric_vertices_vertex_normals_callback_type& quadrilateral_face_geometric_vertices_vertex_normals_callback, const quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback_type& quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback); /*, const polygonal_face_geometric_vertices_begin_callback_type& polygonal_face_geometric_vertices_begin_callback, const polygonal_face_geometric_vertices_vertex_callback_type& polygonal_face_geometric_vertices_vertex_callback, const polygonal_face_geometric_vertices_end_callback_type& polygonal_face_geometric_vertices_end_callback, const polygonal_face_geometric_vertices_texture_vertices_begin_callback_type& polygonal_face_geometric_vertices_texture_vertices_begin_callback, const polygonal_face_geometric_vertices_texture_vertices_vertex_callback_type& polygonal_face_geometric_vertices_texture_vertices_vertex_callback, const polygonal_face_geometric_vertices_texture_vertices_end_callback_type& polygonal_face_geometric_vertices_texture_vertices_end_callback, const polygonal_face_geometric_vertices_vertex_normals_begin_callback_type& polygonal_face_geometric_vertices_vertex_normals_begin_callback, const polygonal_face_geometric_vertices_vertex_normals_vertex_callback_type& polygonal_face_geometric_vertices_vertex_normals_vertex_callback, const polygonal_face_geometric_vertices_vertex_normals_end_callback_type& polygonal_face_geometric_vertices_vertex_normals_end_callback, const polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback_type& polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback, const polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback_type& polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback, const polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback_type& polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback);*/

    void group_name_callback(const group_name_callback_type& group_name_callback);
    void smoothing_group_callback(const smoothing_group_callback_type& smoothing_group_callback);
    void object_name_callback(const object_name_callback_type& object_name_callback);
    void material_library_callback(const material_library_callback_type& material_library_callback);
    void material_name_callback(const material_name_callback_type& material_name_callback);
    void comment_callback(const comment_callback_type& comment_callback);
    bool parse(std::istream& istream);
    bool parse(const std::string& filename);

private:
    flags_type flags_;
    info_callback_type info_callback_;
    warning_callback_type warning_callback_;
    error_callback_type error_callback_;
    geometric_vertex_callback_type geometric_vertex_callback_;
    texture_vertex_callback_type texture_vertex_callback_;
    vertex_normal_callback_type vertex_normal_callback_;
    triangular_face_geometric_vertices_callback_type triangular_face_geometric_vertices_callback_;
    triangular_face_geometric_vertices_texture_vertices_callback_type triangular_face_geometric_vertices_texture_vertices_callback_;
    triangular_face_geometric_vertices_vertex_normals_callback_type triangular_face_geometric_vertices_vertex_normals_callback_;
    triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback_type triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback_;
    quadrilateral_face_geometric_vertices_callback_type quadrilateral_face_geometric_vertices_callback_;
    quadrilateral_face_geometric_vertices_texture_vertices_callback_type quadrilateral_face_geometric_vertices_texture_vertices_callback_;
    quadrilateral_face_geometric_vertices_vertex_normals_callback_type quadrilateral_face_geometric_vertices_vertex_normals_callback_;
    quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback_type quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback_;
    polygonal_face_geometric_vertices_begin_callback_type polygonal_face_geometric_vertices_begin_callback_;
    polygonal_face_geometric_vertices_vertex_callback_type polygonal_face_geometric_vertices_vertex_callback_;
    polygonal_face_geometric_vertices_end_callback_type polygonal_face_geometric_vertices_end_callback_;
    polygonal_face_geometric_vertices_texture_vertices_begin_callback_type polygonal_face_geometric_vertices_texture_vertices_begin_callback_;
    polygonal_face_geometric_vertices_texture_vertices_vertex_callback_type polygonal_face_geometric_vertices_texture_vertices_vertex_callback_;
    polygonal_face_geometric_vertices_texture_vertices_end_callback_type polygonal_face_geometric_vertices_texture_vertices_end_callback_;
    polygonal_face_geometric_vertices_vertex_normals_begin_callback_type polygonal_face_geometric_vertices_vertex_normals_begin_callback_;
    polygonal_face_geometric_vertices_vertex_normals_vertex_callback_type polygonal_face_geometric_vertices_vertex_normals_vertex_callback_;
    polygonal_face_geometric_vertices_vertex_normals_end_callback_type polygonal_face_geometric_vertices_vertex_normals_end_callback_;
    polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback_type polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback_;
    polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback_type polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback_;
    polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback_type polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback_;
    group_name_callback_type group_name_callback_;
    smoothing_group_callback_type smoothing_group_callback_;
    object_name_callback_type object_name_callback_;
    material_library_callback_type material_library_callback_;
    material_name_callback_type material_name_callback_;
    comment_callback_type comment_callback_;
};

} // END namespace obj =========================================================

inline Obj_mtl::obj_parser::obj_parser(flags_type flags)
    : flags_(flags)
{
}

inline bool Obj_mtl::obj_parser::parse(const std::string& filename)
{
    std::ifstream ifstream(filename.c_str());
    return parse(ifstream);
}

inline void Obj_mtl::obj_parser::info_callback(const info_callback_type& info_callback)
{
    info_callback_ = info_callback;
}

inline void Obj_mtl::obj_parser::warning_callback(const warning_callback_type& warning_callback)
{
    warning_callback_ = warning_callback;
}

inline void Obj_mtl::obj_parser::error_callback(const error_callback_type& error_callback)
{
    error_callback_ = error_callback;
}

inline void Obj_mtl::obj_parser::geometric_vertex_callback(const geometric_vertex_callback_type& geometric_vertex_callback)
{
    geometric_vertex_callback_ = geometric_vertex_callback;
}

inline void Obj_mtl::obj_parser::texture_vertex_callback(const texture_vertex_callback_type& texture_vertex_callback)
{
    texture_vertex_callback_ = texture_vertex_callback;
}

inline void Obj_mtl::obj_parser::vertex_normal_callback(const vertex_normal_callback_type& vertex_normal_callback)
{
    vertex_normal_callback_ = vertex_normal_callback;
}

inline void Obj_mtl::obj_parser::face_callbacks(const triangular_face_geometric_vertices_callback_type& triangular_face_geometric_vertices_callback, const triangular_face_geometric_vertices_texture_vertices_callback_type& triangular_face_geometric_vertices_texture_vertices_callback, const triangular_face_geometric_vertices_vertex_normals_callback_type& triangular_face_geometric_vertices_vertex_normals_callback, const triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback_type& triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback, const quadrilateral_face_geometric_vertices_callback_type& quadrilateral_face_geometric_vertices_callback, const quadrilateral_face_geometric_vertices_texture_vertices_callback_type& quadrilateral_face_geometric_vertices_texture_vertices_callback, const quadrilateral_face_geometric_vertices_vertex_normals_callback_type& quadrilateral_face_geometric_vertices_vertex_normals_callback, const quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback_type& quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback /*, const polygonal_face_geometric_vertices_begin_callback_type& polygonal_face_geometric_vertices_begin_callback, const polygonal_face_geometric_vertices_vertex_callback_type& polygonal_face_geometric_vertices_vertex_callback, const polygonal_face_geometric_vertices_end_callback_type& polygonal_face_geometric_vertices_end_callback, const polygonal_face_geometric_vertices_texture_vertices_begin_callback_type& polygonal_face_geometric_vertices_texture_vertices_begin_callback, const polygonal_face_geometric_vertices_texture_vertices_vertex_callback_type& polygonal_face_geometric_vertices_texture_vertices_vertex_callback, const polygonal_face_geometric_vertices_texture_vertices_end_callback_type& polygonal_face_geometric_vertices_texture_vertices_end_callback, const polygonal_face_geometric_vertices_vertex_normals_begin_callback_type& polygonal_face_geometric_vertices_vertex_normals_begin_callback, const polygonal_face_geometric_vertices_vertex_normals_vertex_callback_type& polygonal_face_geometric_vertices_vertex_normals_vertex_callback, const polygonal_face_geometric_vertices_vertex_normals_end_callback_type& polygonal_face_geometric_vertices_vertex_normals_end_callback, const polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback_type& polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback, const polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback_type& polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback, const polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback_type& polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback*/)
{
    triangular_face_geometric_vertices_callback_ = triangular_face_geometric_vertices_callback;
    triangular_face_geometric_vertices_texture_vertices_callback_ = triangular_face_geometric_vertices_texture_vertices_callback;
    triangular_face_geometric_vertices_vertex_normals_callback_ = triangular_face_geometric_vertices_vertex_normals_callback;
    triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback_ = triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback;
    quadrilateral_face_geometric_vertices_callback_ = quadrilateral_face_geometric_vertices_callback;
    quadrilateral_face_geometric_vertices_texture_vertices_callback_ = quadrilateral_face_geometric_vertices_texture_vertices_callback;
    quadrilateral_face_geometric_vertices_vertex_normals_callback_ = quadrilateral_face_geometric_vertices_vertex_normals_callback;
    quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback_ = quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback;
    /*    polygonal_face_geometric_vertices_begin_callback_ = polygonal_face_geometric_vertices_begin_callback;
          polygonal_face_geometric_vertices_vertex_callback_ = polygonal_face_geometric_vertices_vertex_callback;
          polygonal_face_geometric_vertices_end_callback_ = polygonal_face_geometric_vertices_end_callback;
          polygonal_face_geometric_vertices_texture_vertices_begin_callback_ = polygonal_face_geometric_vertices_texture_vertices_begin_callback;
          polygonal_face_geometric_vertices_texture_vertices_vertex_callback_ = polygonal_face_geometric_vertices_texture_vertices_vertex_callback;
          polygonal_face_geometric_vertices_texture_vertices_end_callback_ = polygonal_face_geometric_vertices_texture_vertices_end_callback;
          polygonal_face_geometric_vertices_vertex_normals_begin_callback_ = polygonal_face_geometric_vertices_vertex_normals_begin_callback;
          polygonal_face_geometric_vertices_vertex_normals_vertex_callback_ = polygonal_face_geometric_vertices_vertex_normals_vertex_callback;
          polygonal_face_geometric_vertices_vertex_normals_end_callback_ = polygonal_face_geometric_vertices_vertex_normals_end_callback;
          polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback_ = polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback;
          polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback_ = polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback;
          polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback_ = polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback;*/
}

inline void Obj_mtl::obj_parser::group_name_callback(const group_name_callback_type& group_name_callback)
{
    group_name_callback_ = group_name_callback;
}

inline void Obj_mtl::obj_parser::smoothing_group_callback(const smoothing_group_callback_type& smoothing_group_callback)
{
    smoothing_group_callback_ = smoothing_group_callback;
}

inline void Obj_mtl::obj_parser::object_name_callback(const object_name_callback_type& object_name_callback)
{
    object_name_callback_ = object_name_callback;
}

inline void Obj_mtl::obj_parser::material_library_callback(const material_library_callback_type& material_library_callback)
{
    material_library_callback_ = material_library_callback;
}

inline void Obj_mtl::obj_parser::material_name_callback(const material_name_callback_type& material_name_callback)
{
    material_name_callback_ = material_name_callback;
}

inline void Obj_mtl::obj_parser::comment_callback(const comment_callback_type& comment_callback)
{
    comment_callback_ = comment_callback;
}

inline void Obj_mtl::mtl_parser::info_callback(const info_callback_type& info_callback)
{
    info_callback_ = info_callback;
}

inline void Obj_mtl::mtl_parser::warning_callback(const warning_callback_type& warning_callback)
{
    warning_callback_ = warning_callback;
}

inline void Obj_mtl::mtl_parser::error_callback(const error_callback_type& error_callback)
{
    error_callback_ = error_callback;
}

inline void Obj_mtl::mtl_parser::material_name_callback(const material_name_callback_type& name_callback)
{
    material_name_callback_ = name_callback;
}

inline void Obj_mtl::mtl_parser::material_Ka_callback(const material_Ka_callback_type& Ka_callback)
{
    material_Ka_callback_ = Ka_callback;
}

inline void Obj_mtl::mtl_parser::material_Kd_callback(const material_Kd_callback_type& Kd_callback)
{
    material_Kd_callback_ = Kd_callback;
}

inline void Obj_mtl::mtl_parser::material_Ks_callback(const material_Ks_callback_type& Ks_callback)
{
    material_Ks_callback_ = Ks_callback;
}

inline void Obj_mtl::mtl_parser::material_Tf_callback(const material_Tf_callback_type& Tf_callback)
{
    material_Tf_callback_ = Tf_callback;
}

inline void Obj_mtl::mtl_parser::material_Illum_callback(const material_Illum_callback_type& Illum_callback)
{
    material_Illum_callback_ = Illum_callback;
}

inline void Obj_mtl::mtl_parser::material_dissolve_callback(const material_dissolve_callback_type& dissolve_callback)
{
    material_dissolve_callback_ = dissolve_callback;
}

inline void Obj_mtl::mtl_parser::material_shininess_callback(const material_shininess_callback_type& shininess_callback)
{
    material_shininess_callback_ = shininess_callback;
}

inline void Obj_mtl::mtl_parser::material_sharpness_callback(const material_sharpness_callback_type& sharpness_callback)
{
    material_sharpness_callback_ = sharpness_callback;
}

inline void Obj_mtl::mtl_parser::material_ior_callback(const material_ior_callback_type& ior_callback)
{
    material_ior_callback_ = ior_callback;
}


inline void Obj_mtl::mtl_parser::material_map_Ka_callback(const material_map_Ka_callback_type& map_Ka_callback)
{
    material_map_Ka_callback_ = map_Ka_callback;
}

inline void Obj_mtl::mtl_parser::material_map_Kd_callback(const material_map_Kd_callback_type& map_Kd_callback)
{
    material_map_Kd_callback_ = map_Kd_callback;
}

inline void Obj_mtl::mtl_parser::material_map_Ks_callback(const material_map_Ks_callback_type& map_Ks_callback)
{
    material_map_Ks_callback_ = map_Ks_callback;
}

inline void Obj_mtl::mtl_parser::material_map_Ns_callback(const material_map_Ns_callback_type& map_Ns_callback)
{
    material_map_Ns_callback_ = map_Ns_callback;
}

inline void Obj_mtl::mtl_parser::material_map_d_callback(const material_map_d_callback_type& map_d_callback)
{
    material_map_d_callback_ = map_d_callback;
}

inline void Obj_mtl::mtl_parser::material_dispmap_callback(const material_dispmap_callback_type& dispmap_callback)
{
    material_dispmap_callback_ = dispmap_callback;
}

inline void Obj_mtl::mtl_parser::material_decalmap_callback(const material_decalmap_callback_type& decalmap_callback)
{
    material_decalmap_callback_ = decalmap_callback;
}

inline void Obj_mtl::mtl_parser::material_bumpmap_callback(const material_bumpmap_callback_type& bumpmap_callback)
{
    material_bumpmap_callback_ = bumpmap_callback;
}

inline void Obj_mtl::mtl_parser::material_reflmap_callback(const material_reflmap_callback_type& reflmap_callback)
{
    material_reflmap_callback_ = reflmap_callback;
}

inline void Obj_mtl::mtl_parser::material_normalmap_callback(const material_normalmap_callback_type& normalmap_callback)
{
    material_normalmap_callback_ = normalmap_callback;
}

inline bool Obj_mtl::mtl_parser::parse(const std::string& filename)
{
    std::ifstream ifstream(filename.c_str());
    return parse(ifstream);
}

inline Obj_mtl::mtl_parser::mtl_parser(flags_type flags)
    : flags_(flags)
{
}


} // END namespace loaders =====================================================


#endif // OBJFILEPARSER_H
