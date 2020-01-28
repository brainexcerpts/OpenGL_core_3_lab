
/***************************************************************************
 *   Author: Rodolphe Vaillant                                             *
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

#include "gldirect_draw.h"

#include <iostream>
#include <cmath>

#include <cassert>

// GL_ASSERT ###################################################################

#ifndef NDEBUG
#include <iostream>
#include <cassert>

/**
 *  @file glassert.h
 *  @brief debugging tool for opengl calls
 *
 *  @def glAssert

    This file provide macros in order to check easily openGl API calls.
    use it on every call to ensure better correctness of your programm :
    @code
    #include "glassert.h"

    glAssert( glmyAPICall() )
    @endcode

    @warning never use a glAssert() on a begin() directive.
    glGetError is not allowed inside a glBegin gEnd block !

    On can track down opengl errors with GL_CHECK_ERRORS(); which is usefull if
    the project don't use glAssert() everywhere. In this case a glAssert() can
    be triggered by previous error and won't give valuable insight on where the
    problem is located and from what opengl's primitive call.
*/

#define __TO_STR(x) __EVAL_STR(x)
#define __EVAL_STR(x) #x

#if 0
#define glAssert(code)                                                                         \
    do {                                                                                       \
        code;                                                                                  \
        int l = __LINE__;                                                                      \
        GLuint err = glGetError();                                                             \
        if (err != GL_NO_ERROR) {                                                              \
            std::cerr << "OpenGL error : " << __FILE__ << "\n";                                \
            std::cerr << "line : " << l << "\n";                                               \
            std::cerr << "Source code : " << __TO_STR(code) << "\n";                           \
            std::cerr << "Message : " << (const char*)gluErrorString(err) << "(" << err << ")" \
                      << "\n";                                                                 \
            assert(false);                                                                     \
        }                                                                                      \
    } while (false)
#endif

// -----------------------------------------------------------------------------

#define GL_CHECK_ERRORS()                                                                      \
    do {                                                                                       \
        GLuint err = glGetError();                                                             \
        if (err != GL_NO_ERROR) {                                                              \
            std::cerr << "OpenGL error : " << __FILE__ << "\n";                                \
            std::cerr << "line : " << __LINE__ << "\n";                                        \
            std::cerr << "Source code : " << __TO_STR(code) << "\n";                           \
            std::cerr << "Message : " << (const char*)gluErrorString(err) << "(" << err << ")" \
                      << "\n";                                                                 \
            assert(false);                                                                     \
        }                                                                                      \
    } while (false)

#else

#define GL_CHECK_ERRORS()

//#define glAssert(code) code

#endif

// END GL_ASSERT ###############################################################

// VBO #########################################################################

/** Class of OpenGL buffer objects.
    The type is either GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER or
    GL_PIXEL_[UN]PACK_BUFFER (for PBOs)
*/

struct GlBuffer_obj {

    /// OpenGL context has to be created before this constructor is called,
    /// hence the "explicit"
    /// @param type : GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER,
    /// GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER
    explicit GlBuffer_obj(int type);

    /// @param mode  GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY,
    /// GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY,
    /// GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY
    explicit GlBuffer_obj(int nb_elt, int type, GLenum mode = GL_STREAM_DRAW);

    ~GlBuffer_obj();

    /// Bind the buffer object
    void bind() const;

    /// Unbind the buffer object
    void unbind() const;

    /// Allocate and upload data for the buffer object
    /// @param data the pointer to the data to be copied or null if no data is
    /// to be copied (allocation only)
    /// @param mode  GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY,
    /// GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY,
    /// GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY
    void set_data(int nb_elt,
                  const GLvoid* data,
                  GLenum mode = GL_STREAM_DRAW);

    /// Download data from the buffer object
    void get_data(int offset,
                  int nb_elt,
                  GLvoid* data) const;


    /// Map the buffer object to some pointer on host memory
    ///  @param var the pointer in host memory mapping to the vbo
    ///  @param access_mode can be : GL_READ_ONLY, GL_WRITE_ONLY or GL_READ_WRITE.
    template <class T>
    inline void map_to(T*& var, GLenum access_mode) const
    {
        bind();
        assert(_size_buffer > 0);
        var = (T*)glMapBuffer(_type, access_mode);
        GL_CHECK_ERRORS();
    }

    /// Unmap the buffer object
    bool unmap() const;

    // =========================================================================
    /// @name Getter & Setters
    // =========================================================================

    /// @return buffer object ID OpenGL
    inline int get_id() const
    {
        return _buffer_id;
    }

    /// @return number of elements inside the BO
    inline int size() const
    {
        return _size_buffer;
    }

private:
    int guess_data_ratio(int type)
    {
        return (type == GL_ARRAY_BUFFER) ? sizeof(GLfloat) : (type == GL_ELEMENT_ARRAY_BUFFER) || (type == GL_PIXEL_PACK_BUFFER) || (type == GL_PIXEL_UNPACK_BUFFER) ? sizeof(GLuint) : 1;
    }

    // =========================================================================
    /// @name Attributes
    // =========================================================================

    GLuint _buffer_id; ///< OpenGl id of the buffer object
    int _size_buffer;  ///< number of elements in the buffer
    int _data_ratio;   ///< Byte size of a buffer element

    /// buffer type: GL_ARRAY_BUFFER (usually for vertex attribs such as
    /// position, normals etc.); GL_ELEMENT_ARRAY_BUFFER (usually describe
    /// faces). GL_PIXEL_UNPACK_BUFFER and GL_PIXEL_PACK_BUFFER for pixel
    /// buffer objects (PBO)
    int _type;
};
// =============================================================================

GlBuffer_obj::GlBuffer_obj(int type)
    : _buffer_id(0)
    , _size_buffer(0)
    , _type(type)
{
    _data_ratio = guess_data_ratio(_type);
    glAssert(glGenBuffers(1, &_buffer_id));
}

// -----------------------------------------------------------------------------

GlBuffer_obj::GlBuffer_obj(int nb_elt, int type, GLenum mode)
    : _buffer_id(0)
    , _size_buffer(nb_elt)
    , _type(type)
{
    _data_ratio = guess_data_ratio(type);
    glAssert(glGenBuffers(1, &_buffer_id));
    glAssert(glBindBuffer(_type, _buffer_id));
    glAssert(glBufferData(_type, nb_elt * _data_ratio, NULL, mode));
    glAssert(glBindBuffer(_type, 0));
}

// -----------------------------------------------------------------------------

GlBuffer_obj::~GlBuffer_obj()
{
    glAssert(glDeleteBuffers(1, &_buffer_id));
}

// -----------------------------------------------------------------------------

void GlBuffer_obj::bind() const
{
    glAssert(glBindBuffer(_type, _buffer_id));
}

// -----------------------------------------------------------------------------

void GlBuffer_obj::unbind() const
{
    glAssert(glBindBuffer(_type, 0));
}

// -----------------------------------------------------------------------------

void GlBuffer_obj::set_data(int nb_elt,
                            const GLvoid* data,
                            GLenum mode)
{
    bind();
    _size_buffer = nb_elt;
    glAssert(glBufferData(_type, nb_elt * _data_ratio, data, mode));
    unbind();
}

// -----------------------------------------------------------------------------

void GlBuffer_obj::get_data(int offset,
                            int nb_elt,
                            GLvoid* data) const
{
    bind();
    glAssert(glGetBufferSubData(_type, offset, nb_elt * _data_ratio, data));
    unbind();
}

// -----------------------------------------------------------------------------

bool GlBuffer_obj::unmap() const
{
    bind();
    bool state = glUnmapBuffer(_type) ? true : false;
    GL_CHECK_ERRORS();
    return state;
}

// END VBO #####################################################################

// SHADERS #####################################################################

#include <string>
#include <map>

#ifndef GL_GEOMETRY_SHADER
#define GL_GEOMETRY_SHADER (GL_GEOMETRY_SHADER_EXT)
#endif

/** @class Shader

*/
class Shader {
public:
    /// basic constructor
    /// @param shaderType either GL_VERTEX_SHADER, GL_GEOMETRY_SHADER or
    /// GL_FRAGMENT_SHADER
    Shader(GLuint shaderType);

    /// compile shader from a text source file
    Shader(const std::string& source_path, GLuint shaderType);

    /// adds a macro definition to the shader programm. If the symbol already
    /// exists it will be overwriten by the last entry
    /// @warning must be done before calling load_source otherwise it won't
    /// be effective.
    void add_define(const std::string& name, const std::string& value);

    /// load and compile source file from a text file
    int load_file(const std::string& source_path) const;

    /// load and compile source file from a raw string
    int load_source(const char* source) const;

    /// get compilation status
    int get_status() const;

    /// Check the current status and print error message if needed
    int check_status(const std::string& name = "") const;

    /// get shader's id
    GLuint get_type() const
    {
        return _type;
    }
    operator GLuint() const;
    ~Shader();

private:
    GLuint _id;
    GLuint _type;
    typedef std::map<std::string, std::string> Macro_list;
    Macro_list _symbols;
};

// =============================================================================


struct Shader_prog {
public:
    /// basic constructor
    Shader_prog();

    /// creation from 2 shaders (compiled)
    Shader_prog(const Shader& vs, const Shader& fs);

    ~Shader_prog();

    /// set both vertex & fragment shaders (compiled)
    /// @return true if all went well
    int set_shaders(const Shader& vs, const Shader& fs);

    /// set any compiled shader
    /// @return true if all went well
    int set_shader(const Shader& vs);

    /// get linkage status
    int get_status() const;

    /// link the 2 shaders
    int link() const;

    /// use the program
    int use() const;

    /// do not use the program
    static int unuse();

    /// @return programm id currently used or 0
    static int currently_used();

    /// maximum number of attributes per vertex
    static int get_max_attributes();

    // -------------------------------------------------------------------------
    /// @name set a uniform variable
    // -------------------------------------------------------------------------
    /// @{
    //int set_uniform(const char* name, unsigned v0) const;

    int set_uniform(const char* name, int v0) const;
    int set_uniform(const char* name, int v0, int v1) const;
    int set_uniform(const char* name, int v0, int v1, int v2) const;
    int set_uniform(const char* name, int v0, int v1, int v2, int v3) const;
    int set_uniform(const char* name, GLsizei count, int* values) const;

    int set_uniform(const char* name, float v0) const;
    int set_uniform(const char* name, float v0, float v1) const;
    int set_uniform(const char* name, float v0, float v1, float v2) const;
    int set_uniform(const char* name, float v0, float v1, float v2, float v3) const;
    int set_uniform(const char* name, GLsizei count, float* values) const;

    int set_tex_unit(const char* name, int idx) const
    {
        return set_uniform(name, idx);
    }

    /// Sets a uniform Matrix4x4
    /// @param is_row_major : does the matrices are row major. (by default
    /// opengl matrices are column major)
    int set_mat4x4(const char* name, const float* values, GLsizei count = 1, bool is_row_major = false) const;
    /// @}

    /// Bind a vertex attribute to a specific index.
    /// @warning this will take effects after the next linking with link().
    /// Usually when linking an index is automatically associated to the
    /// attributes names in the vertex shader. There is no reason to use this
    /// method unless you specify by hand every attributes included the built-in
    /// gl_Position, gl_Normal, glColor and co.
    void bind_attribute(const char* name, int index_attr);

    /// @return index of the attribute named 'name'. If -1 is returned it means
    /// that the atribute is not used by the shaders.
    int get_attribute_location(const char* name);

    int get_id() const
    {
        return _id;
    }

private:
    GLuint _id;
    GLuint _vs_id;
    GLuint _gs_id;
    GLuint _fs_id;
};
// =============================================================================

#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdlib>

// Private tools ===============================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* text_file_read(const char* fn)
{
    FILE* fp;
    char* content = NULL;
    int count = 0;
    if (fn == NULL)
        return 0;

    fp = fopen(fn, "rt");
    if (fp == NULL)
        return 0;

    fseek(fp, 0, SEEK_END);
    count = ftell(fp);
    rewind(fp);

    if (count > 0) {
        content = (char*)malloc(sizeof(char) * (count + 1));
        count = fread(content, sizeof(char), count, fp);
        content[count] = '\0';
    }

    fclose(fp);

    return content;
}

// End private tools ===========================================================

// SHADER CLASS IMPLEMENTATION =================================================

Shader::Shader(GLuint shaderType)
{
    _id = glCreateShader(shaderType);
    _type = shaderType;
    GL_CHECK_ERRORS();
}

// -----------------------------------------------------------------------------

Shader::Shader(const std::string& source_name, GLuint shaderType)
{
    _id = glCreateShader(shaderType);
    _type = shaderType;
    GL_CHECK_ERRORS();
    load_file(source_name);
}

// -----------------------------------------------------------------------------

void Shader::add_define(const std::string& name, const std::string& value)
{
    _symbols[name] = value;
}

// -----------------------------------------------------------------------------

int Shader::load_file(const std::string& source_name) const
{
    char* source = text_file_read(source_name.c_str());
    if (source == 0) {
        std::cerr << "error : could not read shader file :" << source_name << std::endl;
        return 0;
    }
    int s = load_source(source);
    free(source);

    return s;
}

// -----------------------------------------------------------------------------

int Shader::load_source(const char* source) const
{
    std::string defines;

    for (Macro_list::const_iterator it = _symbols.begin(); it != _symbols.end(); ++it)
        defines += "#define " + it->first + " " + it->second + "\n";

    const char* file[2] = { defines.c_str(), (const char*)source };
    glAssert(glShaderSource(_id, 2, file, 0));
    glAssert(glCompileShader(_id));

    return check_status();
}

// -----------------------------------------------------------------------------

int Shader::check_status(const std::string& name) const
{
    int len;
    if (get_status() != GL_TRUE) {
        std::cerr << "error : shader compilation :" << name << std::endl;
        const int maxloglen = 4096;
        char log[maxloglen];
        glAssert(glGetShaderInfoLog(_id, maxloglen, &len, log));
        std::cerr << log << std::endl;
        fflush(stderr);
        return 0;
    }
    return 1;
}

// -----------------------------------------------------------------------------


int Shader::get_status() const
{
    int status;
    glAssert(glGetShaderiv(_id, GL_COMPILE_STATUS, &status));
    return status;
}

// -----------------------------------------------------------------------------

Shader::operator GLuint() const
{
    return _id;
}

// -----------------------------------------------------------------------------

Shader::~Shader()
{
    glAssert(glDeleteShader(_id));
}

// END SHADER CLASS IMPLEMENTATION =============================================

// SHADER_PROGRAMM CLASS IMPLEMENTATION ========================================

Shader_prog::Shader_prog()
    : _id(glCreateProgram())
    , _vs_id(0)
    , _gs_id(0)
    , _fs_id(0)
{
}

// -----------------------------------------------------------------------------

Shader_prog::Shader_prog(const Shader& vs, const Shader& fs)
    : _id(glCreateProgram())
    , _gs_id(0)
{
    if (vs.get_type() != GL_VERTEX_SHADER) {
        std::cerr << "Expected a vertex shader !" << std::endl;
        assert(vs.get_type() == GL_VERTEX_SHADER);
    }

    if (fs.get_type() != GL_FRAGMENT_SHADER) {
        std::cerr << "Expected a fragment shader !" << std::endl;
        assert(fs.get_type() == GL_FRAGMENT_SHADER);
    }

    set_shader(vs);
    set_shader(fs);
}

// -----------------------------------------------------------------------------

int Shader_prog::set_shaders(const Shader& vs, const Shader& fs)
{
    return set_shader(vs) & set_shader(fs);
}

// -----------------------------------------------------------------------------

int Shader_prog::set_shader(const Shader& sh)
{
    if (sh.get_status() == GL_TRUE) {
        switch (sh.get_type()) {
        case GL_VERTEX_SHADER:
            _vs_id = sh;
            break;
        case GL_GEOMETRY_SHADER:
            _gs_id = sh;
            break;
        case GL_FRAGMENT_SHADER:
            _fs_id = sh;
            break;
        };
        glAssert(glAttachShader(_id, sh));
        return 1;
    }

    switch (sh.get_type()) {
    case GL_VERTEX_SHADER:
        _vs_id = 0;
        break;
    case GL_GEOMETRY_SHADER:
        _gs_id = 0;
        break;
    case GL_FRAGMENT_SHADER:
        _fs_id = 0;
        break;
    };

    return 0;
}

// -----------------------------------------------------------------------------

int Shader_prog::get_status() const
{
    int status;
    glAssert(glGetProgramiv(_id, GL_LINK_STATUS, &status));
    return status;
}

// -----------------------------------------------------------------------------

int Shader_prog::link() const
{
    if (_vs_id != 0 || _fs_id != 0) {
        glAssert(glLinkProgram(_id));
        if (get_status() != GL_TRUE) {
            int len;
            fprintf(stderr, "error : linking program \n");
            const int maxloglen = 4096;
            char log[maxloglen];
            glAssert(glGetProgramInfoLog(_id, maxloglen, &len, log));
            fprintf(stderr, "%s\n", log);
            fflush(stderr);
            return 0;
        }
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------------

int Shader_prog::use() const
{
    if (get_status() == GL_TRUE) {
        glAssert(glUseProgram(_id));
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------------

int Shader_prog::unuse()
{
    glAssert(glUseProgram(0));
    return 1;
}

// -----------------------------------------------------------------------------

int Shader_prog::currently_used()
{
    int id;
    glAssert(glGetIntegerv(GL_CURRENT_PROGRAM, &id));
    return id;
}

// -----------------------------------------------------------------------------

int Shader_prog::get_max_attributes()
{
    int n;
    glAssert(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n));
    return n;
}

// -----------------------------------------------------------------------------
/*
int Shader_prog::set_uniform(const char* name, unsigned v0) const
{
    assert(Shader_prog::currently_used() == (int)_id);
    int res = glGetUniformLocation(_id, name);
    GL_CHECK_ERRORS();
    if(res != -1)
    {
        glAssert( glUniform1ui(res, v0) );
        return 1;
    }
    return 0;
}
*/
// -----------------------------------------------------------------------------

int Shader_prog::set_uniform(const char* name, int v0) const
{
    assert(Shader_prog::currently_used() == (int)_id);
    int res = glGetUniformLocation(_id, name);
    GL_CHECK_ERRORS();
    if (res != -1) {
        glAssert(glUniform1i(res, v0));
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------------

int Shader_prog::set_uniform(const char* name, int v0, int v1) const
{
    assert(Shader_prog::currently_used() == (int)_id);
    int res = glGetUniformLocation(_id, name);
    GL_CHECK_ERRORS();
    if (res != -1) {
        glAssert(glUniform2i(res, v0, v1));
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------------

int Shader_prog::set_uniform(const char* name, int v0, int v1, int v2) const
{
    assert(Shader_prog::currently_used() == (int)_id);
    int res = glGetUniformLocation(_id, name);
    GL_CHECK_ERRORS();
    if (res != -1) {
        glAssert(glUniform3i(res, v0, v1, v2));
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------------

int Shader_prog::set_uniform(const char* name, int v0, int v1, int v2, int v3) const
{
    assert(Shader_prog::currently_used() == (int)_id);
    int res = glGetUniformLocation(_id, name);
    GL_CHECK_ERRORS();
    if (res != -1) {
        glAssert(glUniform4i(res, v0, v1, v2, v3));
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------------

int Shader_prog::set_uniform(const char* name, GLsizei count, int* values) const
{
    assert(Shader_prog::currently_used() == (int)_id);
    int res = glGetUniformLocation(_id, name);
    GL_CHECK_ERRORS();
    if (res != -1) {
        glAssert(glUniform1iv(res, count, values));
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------------

int Shader_prog::set_uniform(const char* name, float v0) const
{
    assert(Shader_prog::currently_used() == (int)_id);
    int res = glGetUniformLocation(_id, name);
    GL_CHECK_ERRORS();
    if (res != -1) {
        glAssert(glUniform1f(res, v0));
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------------

int Shader_prog::set_uniform(const char* name, float v0, float v1) const
{
    assert(Shader_prog::currently_used() == (int)_id);
    int res = glGetUniformLocation(_id, name);
    GL_CHECK_ERRORS();
    if (res != -1) {
        glAssert(glUniform2f(res, v0, v1));
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------------

int Shader_prog::set_uniform(const char* name, float v0, float v1, float v2) const
{
    assert(Shader_prog::currently_used() == (int)_id);
    int res = glGetUniformLocation(_id, name);
    GL_CHECK_ERRORS();
    if (res != -1) {
        glAssert(glUniform3f(res, v0, v1, v2));
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------------

int Shader_prog::set_uniform(const char* name, float v0, float v1, float v2, float v3) const
{
    assert(Shader_prog::currently_used() == (int)_id);
    int res = glGetUniformLocation(_id, name);
    GL_CHECK_ERRORS();
    if (res != -1) {
        glAssert(glUniform4f(res, v0, v1, v2, v3));
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------------

int Shader_prog::set_uniform(const char* name, GLsizei count, float* values) const
{
    assert(Shader_prog::currently_used() == (int)_id);
    int res = glGetUniformLocation(_id, name);
    GL_CHECK_ERRORS();
    if (res != -1) {
        glAssert(glUniform1fv(res, count, values));
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------------

int Shader_prog::set_mat4x4(const char* name, const float* values, GLsizei count, bool is_row_major) const
{
    assert(Shader_prog::currently_used() == (int)_id);
    int res = glGetUniformLocation(_id, name);
    GL_CHECK_ERRORS();
    if (res != -1) {
        glAssert(glUniformMatrix4fv(res, count, is_row_major, values));
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------------

void Shader_prog::bind_attribute(const char* name, int index_attr)
{
    glAssert(glBindAttribLocation(_id, index_attr, name));
}

// -----------------------------------------------------------------------------

int Shader_prog::get_attribute_location(const char* name)
{
    int attr_idx = -1;
    attr_idx = glGetAttribLocation(_id, name);
    GL_CHECK_ERRORS();
    return attr_idx;
}

// -----------------------------------------------------------------------------

Shader_prog::~Shader_prog()
{
    if (_vs_id != 0)
        glAssert(glDetachShader(_id, _vs_id));
    if (_fs_id != 0)
        glAssert(glDetachShader(_id, _fs_id));

    glAssert(glDeleteProgram(_id));
}

// END SHADER_PROGRAMM CLASS IMPLEMENTATION ====================================

// END SHADERS #################################################################


// VAO #########################################################################

class GlVao {
public:
    GlVao()
    {
        glAssert(glGenVertexArrays(1, &_id));
    }

    ~GlVao()
    {
        glDeleteVertexArrays(1, &_id);
    }

    void bind()
    {
        glAssert(glBindVertexArray(_id));
    }

    static void unbind()
    {
        glBindVertexArray(0);
    }

    /// @param vbo_id : opengl identifier of the vertex object buffer to
    /// be recorded by the VAO
    /// @param attr_idx : index of the attribute
    /// @param nb_components : number of components (x, y, z ...) of the attribute
    /// @warning you must bind the vao buffer using this method
    void record_attr(GLuint vbo_id, int attr_idx, int nb_components)
    {
        glAssert(glBindBuffer(GL_ARRAY_BUFFER, vbo_id));
        glAssert(glVertexAttribPointer(attr_idx, nb_components, GL_FLOAT, GL_FALSE, 0, 0));
        glAssert(glEnableVertexAttribArray(attr_idx));
    }

    /// @warning you must bind the vao buffer using this method
    void record_elt(GLuint vbo_elts_id)
    {
        glAssert(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elts_id));
    }

private:
    GLuint _id;
};

// END VAO #####################################################################

#include <cassert>

/// @def assert_msg
/// @brief asserts and print a message if triggered
#ifndef NDEBUG
#define assert_msg(a, str)                   \
    do {                                     \
        if (!(a)) {                          \
            std::cerr << (str) << std::endl; \
            assert((a));                     \
        }                                    \
    } while (0)

#else
#define assert_msg(a, str)
#endif

// -----------------------------------------------------------------------------

// A 3d vector with basic arithmetic operations
struct Vec3 {
    float x, y, z;

    Vec3()
    {
        x = 0.f;
        y = 0.f;
        z = 0.f;
    }
    Vec3(float x_, float y_, float z_)
    {
        x = x_;
        y = y_;
        z = z_;
    }
    static inline Vec3 zero()
    {
        return Vec3(0.f, 0.f, 0.f);
    }

    Vec3 operator+(const Vec3& v_) const
    {
        return Vec3(x + v_.x, y + v_.y, z + v_.z);
    }
    Vec3 operator*(const Vec3& v_) const
    {
        return Vec3(x * v_.x, y * v_.y, z * v_.z);
    }
    bool operator!=(const Vec3& v_) const
    {
        return (x != v_.x) | (y != v_.y) | (z != v_.z);
    }
    Vec3 operator+(float f_) const
    {
        return Vec3(x + f_, y + f_, z + f_);
    }
    Vec3 operator-(const Vec3& v_) const
    {
        return Vec3(x - v_.x, y - v_.y, z - v_.z);
    }
    Vec3 operator*(const float d_) const
    {
        return Vec3(x * d_, y * d_, z * d_);
    }
    Vec3 operator/(const float d_) const
    {
        return Vec3(x / d_, y / d_, z / d_);
    }
    Vec3 operator-() const
    {
        return Vec3(-x, -y, -z);
    }

    Vec3& operator+=(const Vec3& v_)
    {
        x += v_.x;
        y += v_.y;
        z += v_.z;
        return *this;
    }
    Vec3& operator*=(const Vec3& d_)
    {
        x *= d_.x;
        y *= d_.y;
        z *= d_.z;
        return *this;
    }

    Vec3& operator+=(float f_)
    {
        x += f_;
        y += f_;
        z += f_;
        return *this;
    }
    Vec3& operator/=(float d_)
    {
        x /= d_;
        y /= d_;
        z /= d_;
        return *this;
    }
    Vec3& operator*=(float d_)
    {
        x *= d_;
        y *= d_;
        z *= d_;
        return *this;
    }

    Vec3 cross(const Vec3& v_) const
    {
        return Vec3(y * v_.z - z * v_.y, z * v_.x - x * v_.z, x * v_.y - y * v_.x);
    }
    float dot(const Vec3& v_) const
    {
        return x * v_.x + y * v_.y + z * v_.z;
    }

    float norm_squared() const
    {
        return dot(*this);
    }
    Vec3 normalized() const
    {
        return (*this) * (1.f / sqrtf(norm_squared()));
    }

    float normalize()
    {
        float l = sqrtf(norm_squared());
        float f = 1.f / l;
        x *= f;
        y *= f;
        z *= f;
        return l;
    }

    float norm() const
    {
        return sqrtf(norm_squared());
    }
};

// -----------------------------------------------------------------------------

// Copy/paste procedure from the MESA inplem of glu
// and added the template
template <typename T>
static bool invertMatrix(const T m[16], T invOut[16])
{
    T inv[16], det;
    int i;

    inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
    inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
    inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
    inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
    inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
    inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
    inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
    inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
    inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
    inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
    inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
    inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
    inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
    inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
    inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
    inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return false;

    det = (T)1 / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    return true;
}

// -----------------------------------------------------------------------------

// Copy/paste procedure from the MESA inplem of glu
// and added the template
template <typename T>
static void multMatrices(const T a[16],
                         const T b[16],
                         T r[16])
{
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            r[i * 4 + j] = a[i * 4 + 0] * b[0 * 4 + j] + a[i * 4 + 1] * b[1 * 4 + j] + a[i * 4 + 2] * b[2 * 4 + j] + a[i * 4 + 3] * b[3 * 4 + j];
        }
    }
}

// -----------------------------------------------------------------------------

template <typename T>
static void transposeMatrix(const T a[16], T r[16])
{
    T tr[16];
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            tr[i * 4 + j] = a[j * 4 + i];
        }
    }

    for (i = 0; i < 16; ++i)
        r[i] = tr[i];
}


// -----------------------------------------------------------------------------

/**
  @namespace Shader_dd
  @brief Encapsulate internal shaders used by GlDirect_draw

  Simple hard coded phong shader used to draw with GlDirect_draw.
  Handles a single light attached to the camera
*/
// =============================================================================
namespace Shader_dd {
// =============================================================================

Shader_prog* phong_shader = 0;
int acc = 0;

// -----------------------------------------------------------------------------

// Hard coded phong vertex shader
const char* src_vert = "#version 150\n"
                       "#extension GL_EXT_gpu_shader4 : enable\n"
                       "// Matrices\n"
                       "uniform mat4 modelViewMatrix;\n"
                       "uniform mat4 projectionMatrix;\n"
                       "uniform mat4 MVP;\n"
                       "uniform mat4 normalMatrix;\n"
                       "in vec3 inPosition;\n"
                       "in vec3 inNormal;\n"
                       "in vec4 inTexCoord;\n"
                       "in vec4 inColor;\n"
                       "// Sommet\n"
                       "flat out vec3 varNormal;\n"
                       "out vec4 varTexCoord;\n"
                       "out vec4 varColor;\n"
                       "// Eclairage\n"
                       "flat out vec3 lightDirInView;\n"
                       "flat out vec3 halfVecInView;\n"
                       "void computeLightingVectorsInView(in vec3 posInView, in vec3 lightPosition, out vec3 lightDir, out vec3 halfVec){\n"
                       "   lightDir = normalize(lightPosition-posInView);\n"
                       "   vec3 viewDir = normalize(vec3(0.0, 0.0, 0.0) - posInView);\n"
                       "   halfVec = normalize (lightDir + viewDir);\n"
                       "}\n"
                       "void main(void) {\n"
                       "   vec3 posInView = vec3( modelViewMatrix * vec4(inPosition.xyz, 1.0));\n"
                       "   computeLightingVectorsInView(posInView, vec3(0,0,0), lightDirInView, halfVecInView);\n"
                       "   varNormal = normalize((normalMatrix * vec4(inNormal,0.0)).xyz);\n"
                       "   varTexCoord = inTexCoord;\n"
                       "   gl_Position = MVP*vec4(inPosition.xyz, 1.0);\n"
                       "   varColor = inColor;\n"
                       "}\n\0";

// -----------------------------------------------------------------------------

// Hard coded phong fragment shader
const char* src_frag = "#version 150\n"
                       "#extension GL_EXT_gpu_shader4 : enable\n"
                       "// material\n"
                       "uniform vec3 materialKd;\n"
                       "uniform vec3 materialKs;\n"
                       "uniform float materialNs;\n"
                       "uniform int is_lighting;\n"
                       "// fragment\n"
                       "flat in vec3 varNormal;\n"
                       "in vec4 varTexCoord;\n"
                       "in vec4 varColor;\n"
                       "// lights\n"
                       "flat in vec3 lightDirInView;\n"
                       "flat in vec3 halfVecInView;\n"
                       "// material\n"
                       "uniform vec3 lightColor;\n"
                       "// resultat\n"
                       "out vec4 outColor;\n"
                       "vec3 blinnPhongLighting (in vec3 kd, in vec3 ks, in float ns, in vec3 normal, in vec3 lightVector, in vec3 halfVector){\n"
                       "   float costetha = abs(dot(normal, lightVector));\n"
                       "   float cosalphan = pow( clamp( dot(halfVector,normal), 0., 1.), ns );\n"
                       "   vec3 result = (kd + (ks* cosalphan)) * vec3(costetha);\n"
                       "   return result;\n"
                       "}\n"
                       "void main(void) {\n"
                       "   if( is_lighting == 1 ){ \n"
                       "      vec3 fragColor = vec3(0.0, 0.0, 0.0);\n"
                       "      //fragColor = lightColor * blinnPhongLighting(materialKd, materialKs, materialNs, normalize(varNormal), normalize(lightDirInView), normalize(halfVecInView));\n"
                       "      fragColor = blinnPhongLighting(varColor.xyz, varColor.xyz, 5.f, normalize(varNormal), normalize(lightDirInView), normalize(halfVecInView));\n"
                       "      outColor = vec4( fragColor*0.9, 1.);\n"
                       "   } else { \n"
                       "       outColor = varColor;\n"
                       "   } \n"
                       "}\n\0";

// -----------------------------------------------------------------------------

void init()
{
    acc++;
    if (acc > 1)
        return;

    Shader vertex(GL_VERTEX_SHADER);
    Shader frag(GL_FRAGMENT_SHADER);

    //frag.add_define("LIGHTING_ENABLE", "");
    if (!vertex.load_source(src_vert))
        assert(false);
    if (!frag.load_source(src_frag))
        assert(false);

    phong_shader = new Shader_prog(vertex, frag);
    phong_shader->bind_attribute("inPosition", GlDirectDraw::ATTR_POSITION);
    phong_shader->bind_attribute("inNormal", GlDirectDraw::ATTR_NORMAL);
    phong_shader->bind_attribute("inTexCoord", GlDirectDraw::ATTR_TEX_COORD);
    phong_shader->bind_attribute("inColor", GlDirectDraw::ATTR_COLOR);

    phong_shader->link();

    if (!phong_shader->get_status())
        assert(false);
}

// -----------------------------------------------------------------------------

void clear()
{
    acc--;
    if (acc > 0)
        return;
    delete phong_shader;
    phong_shader = 0;
}

} // END SHADER_DD ==============================================================

// -----------------------------------------------------------------------------

GlDirectDraw::GlDirectDraw(bool use_internal_shader, GLenum buffer_mode)
    : _buffer_mode(buffer_mode)
    , _provoke_mode_last(true)
    , _use_int_shader(use_internal_shader)
    , _is_mat_set(false)
    , _is_begin(false)
    , _is_update(false)
    , _auto_normals(false)
    , _auto_normalize(false)
    , _enable_lighting(false)
    , _curr_mode(MODE_NONE)
{
    for (int i = 0; i < ATTR_SIZE; ++i)
        _attrs_index[i] = -1;

    // Init attributes component size
    _attributes[ATTR_POSITION].size = 3;  // x, y, z
    _attributes[ATTR_NORMAL].size = 3;    // x, y, z
    _attributes[ATTR_TEX_COORD].size = 2; // u, v
    _attributes[ATTR_COLOR].size = 4;     // r, g, b, a

    // Map openGL enums to ours
    _gl_mode_to_our[GL_POINTS] = MODE_POINTS;
    _gl_mode_to_our[GL_LINE_STRIP] = MODE_LINE_STRIP;
    _gl_mode_to_our[GL_LINE_LOOP] = MODE_LINE_LOOP;
    _gl_mode_to_our[GL_LINES] = MODE_LINES;
    _gl_mode_to_our[GL_TRIANGLE_FAN] = MODE_TRIANGLE_FAN;
    _gl_mode_to_our[GL_TRIANGLES] = MODE_TRIANGLES;
    _gl_mode_to_our[GL_TRIANGLE_STRIP] = MODE_TRIANGLE_STRIP;
    _gl_mode_to_our[GL_QUADS] = MODE_QUADS;
    _gl_mode_to_our[GL_QUAD_STRIP] = MODE_QUAD_STRIP;

    assert_msg(_gl_mode_to_our.size() == MODE_SIZE, "Mmmh. forget to update '_gl_mode_to_our' map ?");

    Shader_dd::init();
}

// -----------------------------------------------------------------------------

void GlDirectDraw::clear()
{
    /* DEBUG to be deleted
    glAssert(glBindBuffer(GL_ARRAY_BUFFER, 0));
    glAssert(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    glAssert(glBindBuffer(GL_PIXEL_PACK_BUFFER, 0));
    glAssert(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0));
    */

    for (int attr_t = 0; attr_t < ATTR_SIZE; ++attr_t) {
        for (int mode_t = 0; mode_t < MODE_SIZE; ++mode_t) {
            int size = (int)_gpu_buffers[attr_t][mode_t].size();
            for (int ith_buffer = 0; ith_buffer < size; ++ith_buffer)
                delete _gpu_buffers[attr_t][mode_t][ith_buffer];

            _gpu_buffers[attr_t][mode_t].clear();
            _gpu_maps[attr_t][mode_t].clear();
            _cpu_buffers[attr_t][mode_t].clear();
        }
    }

    // Delete Vaos:
    for (int mode_t = 0; mode_t < MODE_SIZE; ++mode_t) {
        int size = (int)_vaos[mode_t].size();
        for (int ith_vao = 0; ith_vao < size; ++ith_vao)
            delete _vaos[mode_t][ith_vao];
        _vaos[mode_t].clear();
    }
}

// -----------------------------------------------------------------------------

void GlDirectDraw::init_shader()
{
    Shader_dd::init();
}
void GlDirectDraw::erase_shader()
{
    Shader_dd::clear();
}

// -----------------------------------------------------------------------------

GlDirectDraw::~GlDirectDraw()
{
    Shader_dd::clear();
    clear();
}

// -----------------------------------------------------------------------------

void GlDirectDraw::begin(GLenum gl_mode)
{
    assert_msg(!_is_update, "ERROR: can't be called inside begin_update() end_update() calls");
    assert_msg(!_is_begin, "ERROR: imbricated begin() end() are forbidden");
    _is_begin = true;

    std::map<GLenum, Mode_t>::iterator it = _gl_mode_to_our.find(gl_mode);
    assert_msg(it != _gl_mode_to_our.end(), "ERROR: unsupported drawing mode");
    _curr_mode = it->second;

    for (int attr_t = 0; attr_t < ATTR_SIZE; ++attr_t) {
        _cpu_buffers[attr_t][_curr_mode].push_back(std::vector<float>());
        _cpu_buffers[attr_t][_curr_mode][_cpu_buffers[attr_t][_curr_mode].size() - 1].reserve(128 * 4); ////////////////////DEBUG
        _gpu_buffers[attr_t][_curr_mode].push_back(new GlBuffer_obj(GL_ARRAY_BUFFER));
        _gpu_maps[attr_t][_curr_mode].push_back(0);
    }
    _vaos[_curr_mode].push_back(new GlVao());
}

// -----------------------------------------------------------------------------

GlDirectDraw::Attr_id GlDirectDraw::vertex3f(GLfloat x, GLfloat y, GLfloat z)
{
    assert_msg(!_is_update, "ERROR: can't be called inside begin_update() end_update() calls");
    assert_msg(_is_begin, "ERROR: vertex3f() must be called between begin() end() calls");

    _attributes[ATTR_POSITION].set(x, y, z, 1.f);

    int plast = (int)_cpu_buffers[ATTR_POSITION][_curr_mode].size() - 1;

    for (int attr_t = 0; attr_t < ATTR_SIZE; ++attr_t) {
        int last = (int)_cpu_buffers[attr_t][_curr_mode].size() - 1;
        assert(last >= 0);
        assert(last == plast);

        std::vector<float>& buff = _cpu_buffers[attr_t][_curr_mode][last];

        // Copy components (x, y, z ...)
        for (int comp = 0; comp < _attributes[attr_t].size; ++comp)
            buff.push_back(_attributes[attr_t][comp]);
    }

    int idx = (_cpu_buffers[ATTR_POSITION][_curr_mode][plast].size() - 1) / _attributes[ATTR_POSITION].size;
    Attr_id id = { _curr_mode, plast, idx };
    return id;
}

// -----------------------------------------------------------------------------

void GlDirectDraw::color3f(GLfloat r, GLfloat g, GLfloat b)
{
    _attributes[ATTR_COLOR].set(r, g, b, 1.f);
}

// -----------------------------------------------------------------------------

void GlDirectDraw::color4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    _attributes[ATTR_COLOR].set(r, g, b, a);
}

// -----------------------------------------------------------------------------

void GlDirectDraw::normal3f(GLfloat x, GLfloat y, GLfloat z)
{
    if (_auto_normalize) {
        float n = sqrtf(x * x + y * y + z * z);
        x /= n;
        y /= n;
        z /= n;
    }
    _attributes[ATTR_NORMAL].set(x, y, z, 0.f);
}

// -----------------------------------------------------------------------------

void GlDirectDraw::texCoords2f(GLfloat u, GLfloat v)
{
    _attributes[ATTR_COLOR].set(u, v, 0.f, 0.f);
}

// -----------------------------------------------------------------------------

static void update_normals_quads(const std::vector<float>& verts,
                                 std::vector<float>& normals,
                                 bool is_vert_provok_mode_last)
{
    // look up
    for (int i = 0; i < ((int)verts.size() / 3 / 4); ++i) {
        // retrieve face index
        int f[] = { i * 4, i * 4 + 1, i * 4 + 2, i * 4 + 3 };
        // retrieve vertices of the face 'f'
        Vec3 v[4];
        for (int i = 0; i < 4; ++i) {
            v[i] = Vec3(verts[f[i] * 3],
                        verts[f[i] * 3 + 1],
                        verts[f[i] * 3 + 2]);
        }
        // Average normals in case the quad is not planar
        Vec3 n0 = (v[3] - v[0]).cross(v[1] - v[0]);
        Vec3 n1 = (v[1] - v[2]).cross(v[3] - v[2]);
        Vec3 n = (n0 + n1).normalized();

        // Every vertices with same normals
        // compatible with first/last provoking vertex
        for (int i = 0; i < 4; ++i) {
            normals[f[i] * 3] = n.x;
            normals[f[i] * 3 + 1] = n.y;
            normals[f[i] * 3 + 2] = n.z;
        }
    }
}

// -----------------------------------------------------------------------------

static void update_normals_triangle_fan(const std::vector<float>& verts,
                                        std::vector<float>& normals,
                                        bool is_vert_provok_mode_last)
{
    if (verts.size() / 3 >= 3) // at least one triangle
    {
        // central vertex
        Vec3 c(verts[0], verts[1], verts[2]);
        // starting vertex at border of the 'fan'
        Vec3 prev(verts[3], verts[4], verts[5]);

        Vec3 n, n0;
        for (unsigned i = 2; i < verts.size() / 3; ++i) {
            Vec3 next(verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]);
            n = (prev - c).cross(next - c);
            if (i == 2)
                n0 = n;
            n.normalize();

            int j = is_vert_provok_mode_last ? i : i - 1;
            normals[j * 3] = n.x;
            normals[j * 3 + 1] = n.y;
            normals[j * 3 + 2] = n.z;

            prev = next;
        }
    }
}

// -----------------------------------------------------------------------------

static void update_normals_quad_strip(const std::vector<float>& verts,
                                      std::vector<float>& normals,
                                      bool is_vert_provok_mode_last)
{
    if (verts.size() / 3 >= 4) // at least one quad
    {
        // First vertex coords
        Vec3 prev0(verts[0], verts[1], verts[2]);
        // Second vertex coords
        Vec3 prev1(verts[3], verts[4], verts[5]);

        // look up pairs of vertices
        for (unsigned i = 1; i < verts.size() / 3 / 2; ++i) {
            int idx0 = i * 2 + 0;
            int idx1 = i * 2 + 1;
            Vec3 next0(verts[idx0 * 3], verts[idx0 * 3 + 1], verts[idx0 * 3 + 2]);
            Vec3 next1(verts[idx1 * 3], verts[idx1 * 3 + 1], verts[idx1 * 3 + 2]);

            Vec3 n0 = (next1 - prev0).cross(prev1 - prev0);
            Vec3 n1 = (next1 - next0).cross(prev0 - next0);
            Vec3 n = (n0 + n1).normalized();

            int j = is_vert_provok_mode_last ? idx1 : idx0 - 2;
            normals[j * 3] = n.x;
            normals[j * 3 + 1] = n.y;
            normals[j * 3 + 2] = n.z;

            prev0 = next0;
            prev1 = next1;
        }
    }
}

// -----------------------------------------------------------------------------

static void update_normals_triangles(const std::vector<float>& verts,
                                     std::vector<float>& normals,
                                     bool /*is_vert_provok_mode_last*/)
{
    for (int i = 0; i < (int)verts.size() / 3 / 3; ++i) {
        // retrieve face index
        int f[] = { i * 3, i * 3 + 1, i * 3 + 2 };
        // retrieve vertices of the face 'f'
        Vec3 v[3];
        for (int i = 0; i < 3; ++i) {
            v[i] = Vec3(verts[f[i] * 3],
                        verts[f[i] * 3 + 1],
                        verts[f[i] * 3 + 2]);
        }
        Vec3 n = (v[0] - v[1]).cross(v[2] - v[1]);
        n.normalize();

        // Every vertices with same normals
        // (compatible first/last provoking vert)
        for (int i = 0; i < 3; ++i) {
            normals[f[i] * 3] = n.x;
            normals[f[i] * 3 + 1] = n.y;
            normals[f[i] * 3 + 2] = n.z;
        }
    }
}

// -----------------------------------------------------------------------------

static void update_normals_triangle_strip(const std::vector<float>& verts,
                                          std::vector<float>& normals,
                                          bool is_vert_provok_mode_last)
{
    if (verts.size() / 3 >= 3) // at least one triangle
    {
        // First vertex coords
        Vec3 prev0(verts[0], verts[1], verts[2]);
        // Second vertex coords
        Vec3 prev1(verts[3], verts[4], verts[5]);

        // look up vertices starting from the third
        for (unsigned i = 2; i < verts.size() / 3; ++i) {
            Vec3 next(verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]);

            Vec3 n = (next - prev0).cross(prev1 - prev0);
            n.normalize();

            int j = is_vert_provok_mode_last ? i : i - 2;
            normals[j * 3] = n.x;
            normals[j * 3 + 1] = n.y;
            normals[j * 3 + 2] = n.z;

            if ((i % 2) == 0)
                prev0 = next;
            else
                prev1 = next;
        }
    }
}

// -----------------------------------------------------------------------------

void GlDirectDraw::update_normals(Mode_t mode,
                                  const std::vector<float>& verts,
                                  std::vector<float>& normals)
{
    // We compute normals for flat shading and attribute it according
    // to the provoking index convention
    // c.f. opengl doc of glProvokingVertex(GLenum provokeMode);

    const bool prov_mode = _provoke_mode_last;
    switch (mode) {
    case MODE_QUADS:
        update_normals_quads(verts, normals, prov_mode);
        break;
    case MODE_TRIANGLE_FAN:
        update_normals_triangle_fan(verts, normals, prov_mode);
        break;
    case MODE_QUAD_STRIP:
        update_normals_quad_strip(verts, normals, prov_mode);
        break;
    case MODE_TRIANGLES:
        update_normals_triangles(verts, normals, prov_mode);
        break;
    case MODE_TRIANGLE_STRIP:
        update_normals_triangle_strip(verts, normals, prov_mode);
        break;
    default:
        break;
    }
}

// -----------------------------------------------------------------------------

static void push(int vert_i,
                 int attr_size,
                 const std::vector<float>& quad_attr,
                 std::vector<float>& tri_attr)
{
    for (int comp = 0; comp < attr_size; ++comp)
        tri_attr.push_back(quad_attr[vert_i * attr_size + comp]);
}

// -----------------------------------------------------------------------------

static void quads_to_tris(int attr_size,
                          const std::vector<float>& quad_attr,
                          std::vector<float>& tri_attr)
{
    // look up every quads
    tri_attr.reserve(quad_attr.size() * 2);
    for (int i = 0; i < ((int)quad_attr.size() / attr_size / 4); ++i) {
        int f[] = { i * 4, i * 4 + 1, i * 4 + 2, i * 4 + 3 };

        // Convert attribute to triangles
        push(f[0], attr_size, quad_attr, tri_attr);
        push(f[1], attr_size, quad_attr, tri_attr);
        push(f[2], attr_size, quad_attr, tri_attr);

        push(f[0], attr_size, quad_attr, tri_attr);
        push(f[2], attr_size, quad_attr, tri_attr);
        push(f[3], attr_size, quad_attr, tri_attr);
    }
}

// -----------------------------------------------------------------------------

static void quad_strip_to_tri_strip(int attr_size,
                                    const std::vector<float>& quad_attr,
                                    std::vector<float>& tri_attr)
{
    // look up every vertices
    tri_attr.reserve(quad_attr.size());
    for (unsigned i = 0; i < (quad_attr.size() / attr_size); ++i) {
        // Convert attribute to triangles
        push((i % 2 == 0) ? i + 1 : i - 1, attr_size, quad_attr, tri_attr);
    }
}

// -----------------------------------------------------------------------------

void GlDirectDraw::convert_prim(
    int buff_id,
    Mode_t old_mode,
    Mode_t new_mode,
    void (*conv_func)(int attr_size, const std::vector<float>& quad_attr, std::vector<float>& tri_attr))
{
    for (int attr_t = 0; attr_t < ATTR_SIZE; ++attr_t) {
        std::vector<float>& quad_attr = _cpu_buffers[attr_t][old_mode][buff_id];
        std::vector<float> tri_attr;

        const int attr_size = _attributes[attr_t].size;

        conv_func(attr_size, quad_attr, tri_attr);

        // Delete quad cpu
        _cpu_buffers[attr_t][old_mode].erase(_cpu_buffers[attr_t][old_mode].begin() + buff_id);
        // Create tri cpu
        _cpu_buffers[attr_t][new_mode].push_back(tri_attr);
        // Delete quad VBO and mapping
        delete _gpu_buffers[attr_t][old_mode][buff_id];
        _gpu_buffers[attr_t][old_mode].erase(_gpu_buffers[attr_t][old_mode].begin() + buff_id);
        _gpu_maps[attr_t][old_mode].erase(_gpu_maps[attr_t][old_mode].begin() + buff_id);
        // Create triangle VBO and mapping
        _gpu_buffers[attr_t][new_mode].push_back(new GlBuffer_obj(GL_ARRAY_BUFFER));
        _gpu_maps[attr_t][new_mode].push_back(0);
    }

    // Delete quad VAO
    delete _vaos[old_mode][buff_id];
    _vaos[old_mode].erase(_vaos[old_mode].begin() + buff_id);
    // Create triangle VAO
    _vaos[new_mode].push_back(new GlVao());
}


// -----------------------------------------------------------------------------

void GlDirectDraw::convert_to_triangles(int buff_id)
{
    convert_prim(buff_id, MODE_QUADS, MODE_TRIANGLES, quads_to_tris);
}

// -----------------------------------------------------------------------------

void GlDirectDraw::convert_to_triangle_strip(int buff_id)
{
    convert_prim(buff_id, MODE_QUAD_STRIP, MODE_TRIANGLE_STRIP, quad_strip_to_tri_strip);
}

// -----------------------------------------------------------------------------

void GlDirectDraw::end(bool direct_draw)
{
    assert_msg(!_is_update, "ERROR: can't be called inside begin_update() end_update() calls");
    assert_msg(_is_begin, "ERROR: imbricated begin() end() are forbidden");
    _is_begin = false;


    int plast = (int)_cpu_buffers[ATTR_POSITION][_curr_mode].size() - 1;
    int last = (int)_vaos[_curr_mode].size() - 1;

    // In opengl 3.1 QUADS are not supported any more we have to convert them to
    // triangles
#ifndef USE_GL_LEGACY
    if (_curr_mode == MODE_QUADS) {
        convert_to_triangles(last);
        _curr_mode = MODE_TRIANGLES;
    }
    else if (_curr_mode == MODE_QUAD_STRIP) {
        convert_to_triangle_strip(last);
        _curr_mode = MODE_TRIANGLE_STRIP;
    }
#endif

    if (_auto_normals) {
        // Automatically compute normals for flat shading
        update_normals(_curr_mode,
                       _cpu_buffers[ATTR_POSITION][_curr_mode][last],
                       _cpu_buffers[ATTR_NORMAL][_curr_mode][last]);
    }

    _vaos[_curr_mode][last]->bind();
    assert(plast == last);
    // Upload to GPU:
    for (int attr_t = 0; attr_t < ATTR_SIZE; ++attr_t) {
        int last = (int)_cpu_buffers[attr_t][_curr_mode].size() - 1;
        assert(last >= 0);
        assert(last >= plast); // <- why not equal

        std::vector<float>& in = _cpu_buffers[attr_t][_curr_mode][last];
        GlBuffer_obj* out = _gpu_buffers[attr_t][_curr_mode][last];

        out->set_data(in.size(), &(in[0]), GL_STATIC_DRAW);
        int idx_attr = _use_int_shader ? attr_t : _attrs_index[attr_t];
        if (idx_attr > -1)
            _vaos[_curr_mode][last]->record_attr(out->get_id(), idx_attr, _attributes[attr_t].size);
    }
    _vaos[_curr_mode][last]->unbind();

    if (direct_draw) {
        begin_shader();
        draw_buffer(_curr_mode, plast);
        end_shader();
    }

    _curr_mode = MODE_NONE;
}

// -----------------------------------------------------------------------------

void GlDirectDraw::begin_update(GLenum gl_mode)
{
    assert_msg(!_is_begin, "ERROR: can't be called inside begin() end() calls");
    assert_msg(!_is_update, "ERROR: imbricated begin_update() end_update() are forbidden");
    _is_update = true;

    int start_mode = 0;
    int end_mode = MODE_SIZE;

    if (gl_mode == GL_FALSE) {
        // Map every modes
        _curr_mode = MODE_ALL;
    }
    else {
        // Only map the activated mode
        std::map<GLenum, Mode_t>::iterator it = _gl_mode_to_our.find(gl_mode);
        assert_msg(it != _gl_mode_to_our.end(), "ERROR: unsupported drawing mode");
        _curr_mode = it->second;
        start_mode = _curr_mode;
        end_mode = start_mode + 1;
    }

    for (int attr_t = 0; attr_t < ATTR_SIZE; ++attr_t) {
        for (int mode_t = start_mode; mode_t < end_mode; ++mode_t) {
            int size = (int)_gpu_buffers[attr_t][mode_t].size();
            for (int ith_buffer = 0; ith_buffer < size; ++ith_buffer) {
                float* m = 0;
                _gpu_buffers[attr_t][mode_t][ith_buffer]->map_to(m, GL_WRITE_ONLY);
                assert(m != 0); // Can't map the vbo apparently
                _gpu_maps[attr_t][mode_t][ith_buffer] = m;
            }
        }
    }
}

// -----------------------------------------------------------------------------

void GlDirectDraw::set(const Attr_id& v, Attr_t type, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    assert_msg(!_is_begin, "ERROR: can't be called inside begin() end() calls");
    assert_msg(_is_update,
               "ERROR: set() must be called between begin_update() end_update() calls");
    assert_msg(_curr_mode == MODE_ALL || v.mode_t == _curr_mode,
               "ERROR: trying to update an attribute with a drawing mode different from the current one.");

    int attr_t = 0;
    int end_attr = ATTR_SIZE;
    if (type != ATTR_CURRENTS) {
        attr_t = type;
        end_attr = type + 1;
        _attributes[type].set(x, y, z, w);
    }
    else
        _attributes[ATTR_POSITION].set(x, y, z, 1.f);

    for (; attr_t < end_attr; ++attr_t) {
        for (int i = 0; i < _attributes[attr_t].size; ++i) {
            const int idx = v.idx * _attributes[attr_t].size + i;
            assert(_gpu_maps[attr_t][v.mode_t][v.buff_t] != 0); // The VBO is not mapped ?
            _gpu_maps[attr_t][v.mode_t][v.buff_t][idx] = _attributes[attr_t][i];
        }
    }
}

// -----------------------------------------------------------------------------

void GlDirectDraw::end_update()
{
    assert_msg(!_is_begin, "ERROR: can't be called inside begin() end() calls");
    assert_msg(_is_update, "ERROR: imbricated begin_update() end_update() are forbidden");
    _is_update = false;

    int start_mode = 0;
    int end_mode = MODE_SIZE;

    if (_curr_mode != MODE_ALL) {
        // Only unmap the activated mode
        start_mode = _curr_mode;
        end_mode = start_mode + 1;
    }

    for (int attr_t = 0; attr_t < ATTR_SIZE; ++attr_t) {
        for (int mode_t = start_mode; mode_t < end_mode; ++mode_t) {
            int size = (int)_gpu_buffers[attr_t][mode_t].size();
            for (int ith_buffer = 0; ith_buffer < size; ++ith_buffer) {
                _gpu_buffers[attr_t][mode_t][ith_buffer]->unmap();
                _gpu_maps[attr_t][mode_t][ith_buffer] = 0;
            }
        }
    }

    // unbind vbos
    glAssert(glBindBuffer(GL_ARRAY_BUFFER, 0));

    _curr_mode = MODE_NONE;
}

// -----------------------------------------------------------------------------

void GlDirectDraw::draw()
{
    assert_msg(!_is_begin, "ERROR: can't draw inside begin() end() calls");
    assert_msg(!_is_update, "ERROR: can't be called inside begin_update() end_update() calls");

    begin_shader();

    // for each mode (GL_TRIANGLES, GL_LINE_STRIP etc.)
    for (int mode_t = 0; mode_t < MODE_SIZE; ++mode_t) { // Look up associated buffers
        int s = (int)_gpu_buffers[ATTR_POSITION][mode_t].size();
        for (int i = 0; i < s; ++i)
            draw_buffer((Mode_t)mode_t, i);
    }

    end_shader();
}

// -----------------------------------------------------------------------------

void GlDirectDraw::set_matrix(const float model_view[16],
                              const float proj[16])
{
    float MVP[16];
    float normal_mat[16];

    multMatrices(model_view, proj, MVP);
    invertMatrix(model_view, normal_mat);
    transposeMatrix(normal_mat, normal_mat);

    GLint sh_id = 0;
    glAssert(glGetIntegerv(GL_CURRENT_PROGRAM, &sh_id));
    Shader_dd::phong_shader->use();

    Shader_dd::phong_shader->set_mat4x4("MVP", MVP);
    Shader_dd::phong_shader->set_mat4x4("normalMatrix", normal_mat);
    Shader_dd::phong_shader->set_mat4x4("modelViewMatrix", model_view);
    Shader_dd::phong_shader->set_mat4x4("projectionMatrix", proj);

    if (sh_id >= 0)
        glAssert(glUseProgram(sh_id));
    _is_mat_set = true;
}

// -----------------------------------------------------------------------------

void GlDirectDraw::enable_lighting(bool state)
{
    _enable_lighting = state;
}

// -----------------------------------------------------------------------------

void GlDirectDraw::draw_buffer(Mode_t mode_t, int i)
{
    // empty buffer skip it
    if (_gpu_buffers[ATTR_POSITION][mode_t][i]->size() == 0) {
        std::cerr << "WARNING: empty vbo, maybe you didn't put";
        std::cerr << " a vertex3f() between begin() end() calls";
        std::cerr << std::endl;
        return;
    }

#ifndef USE_GL_LEGACY
    assert_msg(_is_mat_set || !_use_int_shader, "ERROR: you forgot to setup your transformation matrices with set_matrix().");
    // Opengl 3.1 and superior drawing
    _vaos[mode_t][i]->bind();

    ///////////////////
    // OpenGl draw call
    unsigned nb_elts = _cpu_buffers[ATTR_POSITION][mode_t][i].size();
    unsigned size_comp = (int)_attributes[ATTR_POSITION].size;
    assert(nb_elts % size_comp == 0);

    GLenum gl_mode = our_mode_to_gl_mode((Mode_t)mode_t);
    int nb_verts = (nb_elts / size_comp);
    glAssert(glDrawArrays(gl_mode, 0, nb_verts));

    _vaos[mode_t][i]->unbind();
#else
    // Opengl legacy (2.1) drawing
    // Activate each attribute in the current buffer

    ////////////////////////
    // Enable client states

    // Enable position
    _gpu_buffers[ATTR_POSITION][mode_t][i]->bind();
    glAssert(glEnableClientState(GL_VERTEX_ARRAY));
    glAssert(glVertexPointer(_attributes[ATTR_POSITION].size, GL_FLOAT, 0, 0));

    // Enable normal
    _gpu_buffers[ATTR_NORMAL][mode_t][i]->bind();
    glAssert(glEnableClientState(GL_NORMAL_ARRAY));
    glAssert(glNormalPointer(GL_FLOAT, 0, 0));

    // Enable texture coordinates
    _gpu_buffers[ATTR_TEX_COORD][mode_t][i]->bind();
    glAssert(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
    glAssert(glTexCoordPointer(_attributes[ATTR_TEX_COORD].size, GL_FLOAT, 0, 0));

    // Enable color
    _gpu_buffers[ATTR_COLOR][mode_t][i]->bind();
    glAssert(glEnableClientState(GL_COLOR_ARRAY));
    glAssert(glColorPointer(_attributes[ATTR_COLOR].size, GL_FLOAT, 0, 0));

    ///////////////////
    // OpenGl draw call
    unsigned nb_elts = _cpu_buffers[ATTR_POSITION][mode_t][i].size();
    unsigned size_comp = (int)_attributes[ATTR_POSITION].size;
    assert(nb_elts % size_comp == 0);

    GLenum gl_mode = our_mode_to_gl_mode((Mode_t)mode_t);
    int nb_verts = (nb_elts / size_comp);
    glAssert(glDrawArrays(gl_mode, 0, nb_verts));

    ////////////////////////
    // Disable client states
    glAssert(glBindBuffer(GL_ARRAY_BUFFER, 0));
    glAssert(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    glAssert(glDisableClientState(GL_VERTEX_ARRAY));
    glAssert(glDisableClientState(GL_NORMAL_ARRAY));
    glAssert(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
    glAssert(glDisableClientState(GL_COLOR_ARRAY));

    glAssert(glVertexPointer(_attributes[ATTR_POSITION].size, GL_FLOAT, 0, 0));
    glAssert(glNormalPointer(GL_FLOAT, 0, 0));
    glAssert(glTexCoordPointer(_attributes[ATTR_TEX_COORD].size, GL_FLOAT, 0, 0));
    glAssert(glColorPointer(_attributes[ATTR_COLOR].size, GL_FLOAT, 0, 0));
#endif
}


// -----------------------------------------------------------------------------

GLenum GlDirectDraw::our_mode_to_gl_mode(Mode_t mode)
{
    std::map<GLenum, Mode_t>::iterator it = _gl_mode_to_our.begin();
    for (; it != _gl_mode_to_our.end(); ++it) {
        if (it->second == mode)
            return it->first;
    }
    return -1;
}

// -----------------------------------------------------------------------------

void GlDirectDraw::begin_shader()
{
#ifndef USE_GL_LEGACY
    if (!_use_int_shader) {
        assert_msg(_attrs_index[ATTR_POSITION] > -1, "ERROR: you forgot to setup the attribute index for the custom shader");
        return;
    }
    glAssert(glGetIntegerv(GL_CURRENT_PROGRAM, &_prev_shader));
    Shader_dd::phong_shader->use();

    Shader_dd::phong_shader->set_uniform("materialKd", 1.f, 1.f, 1.f);
    Shader_dd::phong_shader->set_uniform("materialKs", 0.f, 0.f, 0.f);
    Shader_dd::phong_shader->set_uniform("materialNs", 1.f);
    Shader_dd::phong_shader->set_uniform("lightColor", 1.f, 1.f, 1.f);
    Shader_dd::phong_shader->set_uniform("is_lighting", (int)_enable_lighting);
#endif
}

// -----------------------------------------------------------------------------

void GlDirectDraw::end_shader()
{
#ifndef USE_GL_LEGACY
    if (!_use_int_shader)
        return;
    if (_prev_shader >= 0)
        glAssert(glUseProgram(_prev_shader));
#endif
}

// -----------------------------------------------------------------------------


// =============================================================================
// Some Unit test
// =============================================================================
#if 0

#include "gldirect_draw.hpp"

void test()
{
    GLEnabledSave depth   (GL_DEPTH_TEST, true, true );
    GLEnabledSave lighting(GL_LIGHTING  , true, false);
    GLEnabledSave blend   (GL_BLEND     , true, false);
    GLEnabledSave texture (GL_TEXTURE_2D, true, false);

    GLLineWidthSave line_width(5.f);
    GLPointSizeSave point_size(5.f);

    bool d = true;
    for (int i = 0; i < 2; ++i)
    {
        float off = i*20;
        GlDirect_draw prim;

        GLfloat mv[16], proj[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, mv);
        glGetFloatv(GL_PROJECTION_MATRIX, proj);
        prim.set_matrix( mv, proj);

        prim.color3f(1.f, 0.f, 0.f);

        prim.begin(GL_TRIANGLES);
        prim.vertex3f(off+0.f, 0.f, 0.f);
        prim.vertex3f(off+0.f, 10.f, 0.f);
        prim.vertex3f(off+0.f, 0.f, 10.f);
        prim.end(d);

        prim.color3f(1.f, 1.f, 0.f);
        prim.begin(GL_POINTS);
        prim.vertex3f(off+0.f, 0.f, 0.f);
        prim.vertex3f(off+0.f, 10.f, 0.f);
        prim.vertex3f(off+0.f, 0.f, 10.f);
        prim.end(d);

        prim.color3f(1.f, 0.f, 1.f);
        prim.begin(GL_LINES);
        prim.vertex3f(off+0.f, 0.f, 0.f);
        prim.vertex3f(off+0.f, 10.f, 10.f);
        prim.vertex3f(off+0.f, 10.f, 0.f);
        prim.vertex3f(off+0.f, 0.f, 10.f);
        prim.end(d);

        prim.begin(GL_LINES);
        prim.color3f(0.5f, 1.f, 0.f);
        prim.vertex3f(off+0.f, 0.f, 0.f);
        prim.color3f(0.5f, 1.f, 0.5f);
        prim.vertex3f(off+10.f, 0.f, 0.f);

        prim.color3f(0.f, 1.f, 0.f);
        prim.vertex3f(off+0.f, 0.f, 0.f);
        prim.color3f(0.5f, 0.f, 1.f);
        prim.vertex3f(off+0.f, 0.f, 10.f);
        prim.end(d);

        prim.color3f(0.f, 1.f, 0.f);
        prim.begin(GL_LINE_STRIP);
        prim.vertex3f(off+-5.f, 0.f, 0.f);
        prim.vertex3f(off+-5.f, 10.f, 10.f);
        prim.vertex3f(off+-5.f, 10.f, 0.f);
        prim.vertex3f(off+-5.f, 0.f, 10.f);
        prim.end(d);

        prim.color3f(0.f, 0.f, 1.f);
        prim.begin(GL_LINE_STRIP);
        prim.vertex3f(off+-10.f, 0.f, 0.f  );
        prim.vertex3f(off+-10.f, 10.f, 10.f);
        prim.vertex3f(off+-10.f, 10.f, 0.f );
        prim.vertex3f(off+-10.f, 0.f, 10.f );
        prim.end(d);

        prim.begin(GL_LINE_LOOP);
        prim.vertex3f(off+0.f,  0.f, 20.f  );
        prim.vertex3f(off+0.f, 10.f, 20.f);
        prim.vertex3f(off+10.f, 10.f, 20.f );
        prim.vertex3f(off+10.f,  0.f, 20.f );
        prim.end(d);

        prim.color3f(1.f, 0.f, 1.f);
        prim.begin(GL_TRIANGLE_FAN);
        GlDirect_draw::Attr_id id = prim.vertex3f(5.f,  5.f, 10.f  );
        prim.vertex3f(off+0.f,  0.f, 20.f  );
        prim.vertex3f(off+0.f, 10.f, 20.f);
        prim.vertex3f(off+10.f, 10.f, 20.f );
        prim.vertex3f(off+10.f,  0.f, 20.f );
        prim.end(d);

        static float t = 10.f;
        prim.begin_update(GL_TRIANGLE_FAN);
        prim.set(id, GlDirect_draw::ATTR_CURRENTS, off+5.f, 5.f, t);
        prim.end_update();

        prim.color3f(0.f, 1.f, 0.f);
        prim.begin_update();
        prim.set(id, GlDirect_draw::ATTR_CURRENTS, off+5.f, 5.f, t);
        prim.end_update();

        prim.begin_update();
        prim.set(id, GlDirect_draw::ATTR_POSITION, off+5.f, 5.f, t);
        prim.set(id, GlDirect_draw::ATTR_COLOR   , 0.f, 0.f, t/10.f, 1.f);
        prim.end_update();

        t =  t > 10.f ? 0.f : t + 0.2f;

        if(!d) prim.draw();

        prim.clear();

        d = false;
    }
}

void test2(GLfloat mv[], GLfloat proj[])
{
    GlDirect_draw* draw = new GlDirect_draw();
    draw->enable_lighting(true);
    float l  = 50.f;
    float js =0.2f;
    float ps =0.2f;

    // Some tests with direct draw
    //l = 1.f;
    const float b0 = ps + (l / 10.f); // length of the base
    const float w0 = l / 15.f;        // width of the base


    draw->set_auto_flat_normals( true );
    draw->color3f(0.5f, 1.f, 0.f);
    // First pyramid
    draw->begin( GL_TRIANGLE_FAN );
    {
        draw->vertex3f( ps, 0.f, 0.f );
        draw->vertex3f( b0, 0.f, -w0 );
        draw->vertex3f( b0, -w0, 0.f );
        draw->vertex3f( b0, 0.f,  w0 );
        draw->vertex3f( b0,  w0, 0.f );
        draw->vertex3f( b0, 0.f, -w0 );
    }
    draw->end();

    const float w1 = w0 / 3.f; // Width of the base at the opposite
    l = l-js;
    draw->color3f(0.f, 0.f, 1.f);

    draw->begin( GL_QUAD_STRIP );
    {
        draw->vertex3f( b0, 0.f, -w0 );// a
        draw->vertex3f(  l, 0.f, -w1 );// 0
        draw->vertex3f( b0, -w0, 0.f );// b
        draw->vertex3f(  l, -w1, 0.f );// 1
        draw->vertex3f( b0, 0.f,  w0 );// c
        draw->vertex3f(  l, 0.f,  w1 );// 2
        draw->vertex3f( b0,  w0, 0.f );// d
        draw->vertex3f(  l,  w1, 0.f );// 3
        draw->vertex3f( b0, 5.f/*-0.f--*/, /*-w0*/ 0.f );// a
        draw->vertex3f(  l, 5.f/*-0.f--*/, /*-w1*/ 0.f );// 0
    }
    draw->end();

    // The bone's cap is flat
    draw->color3f(1.f, 1.f, 0.f);
    draw->begin( GL_QUADS );
    {
        draw->color3f(0.5f, 0.3f, 0.f);
        draw->vertex3f( l, 0.f, -w1 );
        draw->color3f(0.1f, 0.1f, 0.f);
        draw->vertex3f( l, -w1, 0.f );
        draw->color3f(0.0f, 0.3f, 0.2f);
        draw->vertex3f( l, 0.f,  w1 );
        draw->color3f(0.0f, 0.0f, 0.4f);
        draw->vertex3f( l,  w1, 0.f );
    }
    draw->end();

    draw->begin( GL_TRIANGLES );
    {
        draw->vertex3f( 2.f+l, 0.f, -w1 );
        draw->vertex3f( 2.f+l, -w1, 0.f );
        draw->vertex3f( 2.f+l, 0.f,  w1 );

        draw->vertex3f( 2.f,  4.f, 0.f );
        draw->vertex3f( 8.f,  4.f, 0.f );
        draw->vertex3f( 4.f,  7.f, 0.f );
    }
    draw->end();

    draw->begin( GL_TRIANGLE_STRIP );
    {
        draw->vertex3f( 0.f, 0.f, 0.f );
        draw->vertex3f( 2.f, 0.f, 0.f );
        draw->vertex3f( 1.f, 3.f, 1.f );

        draw->vertex3f( 2.f, 0.f, 1.f );
        draw->vertex3f( 1.f, 3.f, 1.f );
        draw->vertex3f( 3.f, 3.f, 2.f );
    }
    draw->end();

    draw->set_matrix( mv, proj);
    draw->draw();
    delete draw;
}

#endif
