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

#include "renderer.h"

#include "gl_utils/opengl.h"
#include "gl_utils/gldirect_draw.h"
#include "fileloaders/objloader.h"
#include "fileloaders/fileloader.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>

/** @defgroup RendererGlobalFunctions
  * @author Mathias Paulin <Mathias.Paulin@irit.fr>
  * Rodolphe Vaillant <blog@rodolphe-vaillant.fr>
  *
  * @ingroup RenderSystem
  * Global functions for the renderer.
  *
  * This is a "sandbox" to learn OpenGl API.
  * The code below must be filled by students.
  */
// =============================================================================
namespace RenderSystem {
// =============================================================================

/// Initialization of the resources (Mesh / shaders / Lights ... )
/// (Called once when the application is launched)
void Renderer::initRessources()
{
    // #########################################################################
    // LAB 1 / PART I: CODE TO COMPLETE

    // In this lab the name of the functions to use are given but not
    // details about how to use them
    // Use "google" or "http://www.opengl.org/sdk/docs/man3/" or
    // the command "man" to find the meaning of a function's parameters.
    // One of the goal of this lab is to learn to search and navigate
    // documentation

    initShaders(); // LAB 1 / PART I: Shader initialization (Function to fill)
    initView();    // LAB 1 / PART I: Viewing parameters init (Function to fill)

    // 1 - Enable the depth test with ( glEnable() )
    // N.B (1): Do notice the impressive list of enumerants for "glEnable()".
    // Every enum defines an Opengl "State", these are stored into variables of
    // the "OpenGl context". The function glEnable() is an accessor.
    // N.B (2): Every OpenGl "state" can be disabled with glDisable()

    // ...

    // 2 - Define the drawing mode (Wires or Plain "glPolygonMode()")

    // ...

    // LAB 1 / PART I: END CODE TO COMPLETE
    // #########################################################################


    initGeometry(); // LAB 1 / PART II: Loading or building geometric data.
}

//------------------------------------------------------------------------------

/// Initialization of the view parameters (camera position)
/// (Called once when the application is launched)
void Renderer::initView()
{
    // #########################################################################
    // LAB 1 / PART I: CODE TO COMPLETE


    // Initialize the view matrix ('this->mViewMatrix' attribute)

    // Use the following values:
    // Eye position (eye): 1.f, 0.f, 0.f
    // Aimed point (center): 0.0f, 0.0f, 0.0f
    // Up vector : 0.0f, 1.0f, 0.0f
    //
    // To fill "mViewMatrix" you can use
    // "glm::lookAt()" which helps to compute the view matrix


    // LAB 1 / PART I: END CODE TO COMPLETE
    // #########################################################################
}

// -----------------------------------------------------------------------------

// (N.B: This function will be re-used when initializing shaders)
/// @ingroup RendererGlobalFunctions
/// Print to stderr compilation log of a shader
/// @param shader : identifiant shader Opengl
void printShaderInfoLog(GLint shader)
{
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar* infoLog;
    glAssert(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen));
    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        glAssert(glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog));
        std::cerr << "InfoLog:" << std::endl
                  << infoLog << std::endl;
        delete[] infoLog;
    }
}

// -----------------------------------------------------------------------------

// (N.B: This function will be re-used when initializing shaders)
/// @ingroup RendererGlobalFunctions
/// Print to stderr link log of a program shader
/// @param program: identifiant program shader in opengl
void printProgramInfoLog(GLint program)
{
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar* infoLog;
    glAssert(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen));
    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        glAssert(glGetProgramInfoLog(program, infoLogLen, &charsWritten, infoLog));
        std::cerr << "InfoLog:" << std::endl
                  << infoLog << std::endl;
        delete[] infoLog;
    }
}

//------------------------------------------------------------------------------

/// Loading and compiling shaders
/// (Called once when the application is launched)
void Renderer::initShaders()
{

    // #########################################################################
    // LAB 1 / PART I: CODE TO COMPLETE


    // N.B: While doing this lab keep in mind the OpenGl rendering pipeline
    // draw command -> ... -> (vertex shader) -> ... -> (fragment shader) ->  ... -> z-test -> final image
    // and try to remind yourself which stage you are programming
    // and how it operates. The lab can be cryptic at first but if you do this
    // effort everything should be clearer.

    // First, we give the correct answer to load a shader.
    // This is the programm executed at the OpenGl stage called "vertex shader":
    // 1 - Vertex shader :
    //   1.1 - Load the source code from the file "this_project_folder/shaders/vertexdefault.glsl"
    char* vertexShaderSource = Loaders::Text::loadFile("../shaders/vertexdefault.glsl"); // <- loadFile() is already defined in this project

    //   1.2 - Allocate an object called VERTEX_SHADER and associate it to the source code
    //         We use the attribute "mVertexShaderId" to store the vertex Shader identifier
    glAssert(mVertexShaderId = glCreateShader(GL_VERTEX_SHADER));
    glAssert(glShaderSource(mVertexShaderId, 1, (const GLchar**)&vertexShaderSource, NULL));

    //   1.3 - Compile the shader
    glAssert(glCompileShader(mVertexShaderId));

    //   1.4 - Check compilation error
    GLint compiled;
    glAssert(glGetShaderiv(mVertexShaderId, GL_COMPILE_STATUS, &compiled));
    if (!compiled) {
        // Print error message
        std::cerr << " Vertex shader not compiled : " << std::endl;
        printShaderInfoLog(mVertexShaderId); // <- tool defined within this project
    }

    // !!!!!!!
    // Note: glAssert() is not an Opengl function.
    // It's a macro (defined for this project) which goal is to check
    // OpenGl errors. Without it OpenGl won't stop if some error occur. instead
    // the application with continue to execute despite the corrupted OpenGl
    // context. This can produce random behaviors or crash 200 lines later on
    // perfectly correct code.
    // Bottom line: always check ALL OpenGl calls with "glAssert()"
    // Take a look at the definition of glAssert().
    // !!!!!!!

    // 2 - Fragment shader : (Almost identical to vertex shaders...)
    //   2.1 - Load the source code from the file "../shaders/fragmentdefault.glsl"
    //         you will use the function "loaders::text::loadFile()" which reads text files.
    //         Also use the attribute "mFragmentShaderId"
    //   2.2 - Allocate the OpenGl object called FRAGMENT_SHADER and associate it to the source code
    //   2.3 - Compile the shader
    //   2.4 - Check compilation errors


    // 3 - Program :
    //   3.1 - Allocate the OpenGl "program", store its identifier in the attribute "mProgram"
    // and associate the vertex shader and fragment shader (glCreateProgramm() glAttachShader() )


    //   3.2 - Define the index corresponding to "vertex attributes"
    // ("vertex attributes" are defined in the vertex shader "vertexdefault.glsl"
    //  look for variables with the preffix "in").
    //
    // Explanation:
    // For a given mesh, a vertex shader works in parallel over the list of vertices.
    // The file "shaders/vertexdefault.glsl" define the processing of a single
    // vertex (The "for" loop is implicitly done in parallel).
    // Each vertex is associated to various attributes (normals, positions, etc.).
    // Therefore you will find variables in the vertex shaders such as "in vec3 maVar",
    // those are vertex "attributes". One must tell the shader where to look for
    // to initialise these "in" variables.
    // This is done by associating a number (index) to the variable/attribute.
    // Use the same convention as in "MyGLMesh" (PART II of lab 1):
    // -  position            --> index 0
    // -  normal              --> index 1
    // -  texture coordinates --> index 2
    // You must take a look at the file "shaders/vertexdefault.glsl" to discover
    // the name of the variables "in" and assign them to a number.
    // This is done  with the function glBindAttribLocation().
    // N.B: When uploading the mesh from CPU to GPU you will also have to specify
    // those numbers (PART II of lab 1).

    // ...

    //   3.3 - Link the program (i.e. Link vertex shader and fragment shader)
    //         ( glLinkProgram() )

    //   3.4 - Check linking errors
    //          (use glGetProgramiv() then printProgramInfoLog())


    // LAB 1 / PART I: END CODE TO COMPLETE
    // #########################################################################
}

//------------------------------------------------------------------------------

/// Erase shader programs
void Renderer::clearShaders()
{
    // #########################################################################
    // LAB 1 / PART I: CODE TO COMPLETE


    // 1 - Detach shader programs ( glDetachShader() )

    // 2 - Clean shaders (vertex, fragment shaders) ( glDeleteShader() )
    // Note: if you don't detach shaders OpenGL might not actually delete
    // shaders upon the call of glDeleteShader() !

    // 3 - delete the "program shader" itself
    // Note: only the program shader is actually necessary to draw objects.
    // The individual shaders are only necessary before linking. After linking
    // and producing the final program we could have delete those individual
    // shaders in initShaders().


    // LAB 1 / PART I:END CODE TO COMPLETE
    // #########################################################################
}

//------------------------------------------------------------------------------

/// "Render loop": this function is automatically called every time every time the
/// screen/window is refreshed
void Renderer::render()
{
    // We declare the 4x4 matrix to transform object of the scene,
    // the so called "model matrix":
    // For this lab no animation so we use the indentity matrix.
    // The constructor glm::mat4x4(float) set the diagonal values therefore:
    const glm::mat4x4 modelMatrix(1.0f);
    // Is the identiry matrix:
    // 1 0 0 0
    // 0 1 0 0
    // 0 0 1 0
    // 0 0 0 1

    // #########################################################################
    // LAB 1 / PART I: CODE TO COMPLETE


    // This is the main function to render/compute the image
    // We must call the openGL API functions (very low level) to command the GPU
    // to draw our scene correctly using rasterization .


    // 1 - Prepare our image for rendering: Remember this actually a look
    //     and this function gets called over and over again so we must erase
    //     previous drawings and start fresh.
    //    1.1 -  Erase destination buffers ('color' and 'depth' buffers)
    //           (glClearColor(), glClear())

    // 2 - Build 'view' and 'projection' matrices:

    //    2.1 - Begin with the "perspective projection matrix"
    //          You'll need  glm::perspective(float fovy, float aspect, float zNear, float zFar)
    //          This function defines the co called 'view frustum' of your camera.
    //          Look up Google image for "view frustum" for schematics.
    //          Parameters:
    //          - 'fovy' = angle in radian, field of view according the y axis
    //          - 'aspect' = window_width/window_height ( don't forget to cast to floats!)

    //    2.2 - Define the new view matrix merging 'this->mViewMatrix' and 'modelMatrix' together

    //    2.3 - Compute the matrix to transform normals.

    //    2.4 - Compute the final matrix 'MVP' (acronyme for "Model View Projection")
    //          MVP transform a vertex in local coordinates to a vertex in image coordinates

    // 3 - Setting up shader parameters:
    //    3.1 - In a 3D application there might be dozens of shaders. You must
    //          tell OpenGl which one to use with (glUseProgram()).
    //          Set the current shader to be used to the one you created in "this->initShaders()"

    //    3.2 - Set values that are constant per object ('uniform variable' of the shader "../shaders/**.glsl"):
    //          (take a look at the shader's source code)
    //          You must upload to GPU you the matrices (MVP etc.) that you just
    //          computed here on the CPU.
    //          In other words set the shader's uniform variables with
    //          (glGetUniformLocation() et glUniform()).
    //          Note: You will need to convert glm matrices to a pointer with:
    //          'float* ptr = glm::value_ptr(ma_matrice)'


    // LAB 1 / PART I:END CODE TO COMPLETE
    // #########################################################################


    // #########################################################################
    // LAB 1 / PART II:
    // In part I leave this code untouched
    // In part II comment or delete those lines
#if 1
    if (mProgram != -1)
        mDummyObject->draw();
#endif
    // 4 - Instead use 'this->mMeshes' to draw the object of the scene:
    draw_list_mesh();

    // LAB 1 / PART II:END CODE TO COMPLETE
    // #########################################################################
}


// -----------------------------------------------------------------------------

/**
  * @ingroup RenderSystem
  * A mesh with OpenGL rendering capabilities.
  */
class MyGLMesh : public Loaders::Mesh {
private:
    /// OpenGL identifier for the "Vertex Array Object" (VAO) of the mesh
    GLuint mVertexArrayObject;


    /// Index pour accéder mVertexBufferObjects[]
    enum { VBO_VERTICES = 0,
           VBO_INDICES = 1,
           NB_VBOS };

    /// OpenGL identifiers for the VBOs of our mesh
    /// N.B: use VBO_VERTICES and VBO_INDICES to access this array elements
    GLuint mVertexBufferObjects[NB_VBOS];

public:
    MyGLMesh(const Loaders::Mesh& mesh)
        : Loaders::Mesh(mesh)
    {
    }

    MyGLMesh(const std::vector<float>& vertexBuffer,
             const std::vector<int>& triangleBuffer,
             bool hasNormals = true,
             bool hasTextureCoords = true)
        : Loaders::Mesh(vertexBuffer,
                        triangleBuffer,
                        std::vector<int>(),
                        hasNormals,
                        hasTextureCoords)
    {
    }

    /// Upload du maillage sur GPU
    /// Build VertexArrayObjects for the mesh.
    void compileGL()
    {
        // This function aims to prepare our mesh for rendering with OpenGl.
        // To this end, you must load into video memory (GPU)
        // the mesh from central memory (CPU). OpenGl provides various types of
        // GPU buffers to do so. These are called VBOs for Vertex Buffer Object.
        // Then you will have to 'explain' to OpenGL how is organized the
        // memory of your VBOs. This is done with a VAO Vertex Array Object,
        // which memories which VBOs to use and what is the layout of those VBOs.
        //
        // Before you start coding anything you must read and understand how
        // is organized the data of this mesh on CPU.
        // "MyGLMesh" inherits from another class... take a look to his parent
        // to find out the answers.. "Mesh" defines sub structures
        // "TriangleIndex" and "Vertex" that might be of interest...
        //
        // doc: http://www.opengl.org/sdk/docs/man3/

        // Now that you have a rough image our mesh storage in CPU let's upload
        // it GPU. In other words we will copy "Mesh" data to OpenGL structures.
        // But here you won't be able to do a simple:
        // opengl_class.mAttribut = ma_class.mAttribut
        // You can only manipulate OpenGl data through C functions such as "gl_XXXX_()".

        // Under the hood OpenGL structures such as VAO and VBOS look like this:
        /*
            struct VertexArrayObject { // <- The VAO
                // Attributes:
                VertexBufferObject _list_of_positions[NB_VERTS]; // Array of vertex positions
                VertexBufferObject _list_of_normales[NB_VERTS]; // Array of vertex normals
                // ...

        // List of triangles:
        // Example First triangle (p1,p2,p3):
        // p1 _list_of_positions[ _list_of_triangles[0] ]
        // p2 _list_of_positions[ _list_of_triangles[2] ]
        // p3 _list_of_positions[ _list_of_triangles[3] ]

        ElementBufferObject _list_of_triangles[NB_TRIS*3];

    };
    Well, OpenGL might not store things exactly like this VAOs but
              this give you an idea. accessing the structure directly is not
              possible you must use glXXX() accessors.
              */

              // #####################################################################
              // LAB 1 / PART II: CODE TO COMPLETE


              // 1 - Create a VAO. Generate a unique identifier for a VertexArrayObject
              // and store it in this->mVertexArrayObject
              // ( glGenVertexArrays() )

              // 2 - Create 2 VBOs. Generate two identifiers for VertexBufferObject
              // (one for vertices VBO_VERTICES, another for faces (triangles) VBO_INDICES)
              // save them in this->mVertexBufferObjects
              // ( glGenBuffers() )

              // 3 - Tell OpenGL which VAO we are currently working.
              // Enable the previously created VertexArrayObject (VAO)
              // ( glBindVertexArray() )

              // 4 - Tell OpenGl which VBO we are currently working on. Enable the
              // previously created VertexBufferObject *for vertices*
              // (glBindBuffer())

              // Note: VBO for vertex attributes
              // (position, normal, speed, color, acceleration...) alway have the same
              // type (i.e. GL_ARRAY_BUFFER)

              // 5 - Fill the VertexBufferObject of vertices with
              // (glBufferData())

              // 6 - Describe the buffer memory layout / organization
              // (glVertexAttribPointer())

              // Note: You need to tell OpenGL how is organized your data.
              // for instance associate
              // - position      -> (index 0)
              // - normal        -> (index 1)
              // - texture coord -> (index 2)

              // 7 - Enable which attributes are to be used (position, normal, etc.)
              // (glEnableVertexAttribArray)

              // 8 - Enable the VertexBufferObject *for faces*.
              // Be careful this VBO is a list of faces therefore his type is GL_ELEMENT_ARRAY_BUFFER
              // and not GL_ARRAY_BUFFER which is used for vertex attributes
              // ...

              // 9 - Fill VertexBufferObject *of faces*
              // ...

              // LAB 1 / PART II: END CODE TO COMPLETE
              // #####################################################################


              // Binding to 0 means 'unBind()' and garantees no buffer is enabled
              glAssert(glBindVertexArray(0));
}

/// Draw the VertexArrayObjects (VAO "mVertexArrayObject") of the mesh.
void drawGL()
{
    // Draw the mesh loaded in video memory thanks to our VAO

    // #####################################################################
    // LAB 1 / PART II: CODE TO COMPLETE


    // 1 - Enable the VAO (bind)

    // 2 - Draw triangles.
    // Les sommets des triangles étant indexés et non consécutifs (sauf cas très particulier)
    // on utilisera la fonction glDrawElements(...)

    // Watch out! The "count" parameter of glDrawElements() does not define
    // the number of triangles but the actual size your index buffer.
    // (i.e. the number of integers stored in your GL_ELEMENT_ARRAY_BUFFER)


    // LAB 1 / PART II: END CODE TO COMPLETE
    // #####################################################################
}

/// Destructor
~MyGLMesh()
{
    // #####################################################################
    // LAB 1 / PART II: CODE TO COMPLETE

    // 1 - delete VBO and VBE
    // glDeleteBuffers()

    // 2 - Delete VAO
    // glDeleteVertexArrays()


    // LAB 1 / PART II: END CODE TO COMPLETE
    // #####################################################################
}
};

// -----------------------------------------------------------------------------

/// Load (from file) and upload to GPU (compile) the various meshes of our scene.
/// (Called once when the application is launched)
void Renderer::initGeometry()
{

    // #########################################################################
    // LAB 1 / PART II: CODE TO COMPLETE


    // Once PART I of lab 1 is finished and checked
    // You can delete / disable the line below
#if 1
    // This builds a VAO (upload to GPU) with data for a triangle and a sphere.
    // This is only here to test PART I without having to do PART II
    init_dummy_object();
#endif

    // In this function we do not use OpenGL API functions. Our goal is
    // to parse files containing our 3D objects, and upload in video memory.
    // A parser is already available all is asked here is to explore this very
    // project's code and find what you need.

    // 1 - Build a vector of meshes (std::vector<loaders::Mesh*>)
    // from the file "../data/camel.obj"
    // You need to create a "loaders::obj_mtl::ObjLoader".
    // and use the method ".load()" to parse an "camel.obj".
    // If an error occurs print it.
    // Retreive the parsed meshes with ".getObjects()"


    // 2 - Convert the list of meshes to "MyGLMesh"
    // (use this->mMeshes to store the converted objects)

    // 3 - Upload to GPU with ".compileGL()"

    // LAB 1 / PART II: END CODE TO COMPLETE
    // #########################################################################
}

// -----------------------------------------------------------------------------

void Renderer::draw_list_mesh()
{
    // #########################################################################
    // LAB 1 / PARTIE II: 

    // 4 - Dessiner les objets de la scène dans l'attribut 'mMeshes':

    for (std::vector<MyGLMesh*>::iterator it = mMeshes.begin(); it != mMeshes.end(); ++it ) {
        (*it)->drawGL();
    }
    // LAB 1 / PART II: 
    // #########################################################################
}

// -----------------------------------------------------------------------------

int Renderer::handleMouseEvent(const MouseEvent& event)
{
    //static int modifiers = 0;
    //static bool moved = false;
    static int x = 0;
    static int y = 0;
    static int button = 0;

    if (event.click) {
        x = event.x;
        y = event.y;
        button = event.button;
        //modifiers = event.modifiers;
    }

    if (event.button == MouseEvent::MOVE) {

        // Note: The frame of the camera is i, j, k encoded into the
        // this->mViewMatrix, Find by yourself you to extract i, j, k from this
        // matrix. Watch out, glm matrices are "column major" (just like OpenGL)

        float dx = (float)(event.x - x) / (float)mWidth;
        float dy = (float)(event.y - y) / (float)mHeight;

        // #####################################################################
        // LAB 2 CODE TO COMPLETE

        switch (button) {
        case MouseEvent::LEFT: {
            float a = dx, b = dy;
            a = b;
            b = a; // HACK: Avoid unsued warning

            // 1 - Retrieve xvec and yvec axis of the camera
            // 2 - Modify mViewMatrix in order to rotate about dy*360.f degres around xvec
            // 3 - Modify mViewMatrix in order to rotate about dx*360.f degres around yvec
        } break;
        case MouseEvent::RIGHT: {
            // 1 - Compute the translation vector xvec and yvec according to the camera X and Y axis
            // 2 - Modify mViewMatrix to translate about xvec+yvec
        } break;
        case MouseEvent::MIDDLE: {
            // 1 - Compute the translation vector zvec according to the camera Z axis
            // 2 - Modify mViewMatrix to translate about zvec
        } break;
        }

        // TP2 END CODE TO COMPLETE
        // #####################################################################

        x = event.x;
        y = event.y;
        //moved = true;
        return 1;
    }
    else {
        //moved = false;
    }

    if (event.button == MouseEvent::WHEEL) {
        return 1;
    }

    if (!event.click) {
        button = 0;
        //modifiers = 0;
    }

    return 1;
}

// -----------------------------------------------------------------------------

void Renderer::setViewport(int width, int height)
{
    mWidth = width;
    mHeight = height;
    glAssert(glViewport(0, 0, mWidth, mHeight));
}

// -----------------------------------------------------------------------------

int Renderer::handleKeyEvent(char key)
{

    switch (key) {
    case 'w':
        glAssert(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        break;
    case 'f':
        glAssert(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        break;
    }
    return 1;
}

// -----------------------------------------------------------------------------

Renderer::~Renderer()
{
    for (unsigned i = 0; i < mMeshes.size(); ++i)
        delete mMeshes[i];

    clearShaders();
    delete mDummyObject;
}

// -----------------------------------------------------------------------------

// Initialize the vertex and triangle list to define a sphere and a single triangle.
// this code is here to allow students to test "lab 1 part I" without having
// to define an object. Instead they can simply use this
void Renderer::init_dummy_object()
{
    mDummyObject = new GlDirectDraw();

    // We use the shaders defined in this file and not those defined in class GlDirect_draw;
    mDummyObject->enable_internal_shader(false);

    // Associate attributes (position, normale, etc.) to an index (0, 1, etc.)
    // This must be the same as in "initShaders()" which should be the case if
    // students our instructions properly
    mDummyObject->set_attr_index(GlDirectDraw::ATTR_POSITION, 0);
    mDummyObject->set_attr_index(GlDirectDraw::ATTR_NORMAL, 1);
    mDummyObject->set_attr_index(GlDirectDraw::ATTR_TEX_COORD, 2);
    mDummyObject->set_auto_flat_normals(true);

    // Define a simple trianlge
    mDummyObject->begin(GL_TRIANGLES);
    mDummyObject->color3f(1.f, 0.f, 1.f);
    mDummyObject->vertex3f(0.f, 0.f, 0.2f);
    mDummyObject->vertex3f(0.f, 0.2f, 0.f);
    mDummyObject->vertex3f(0.f, 0.f, -0.2f);
    mDummyObject->end();

    // Define a sphere with points
    mDummyObject->begin(GL_POINTS);
    for (float u_ = 0; u_ < 1.f; u_ += 0.01f) {
        for (float v_ = 0; v_ < 1.f; v_ += 0.01f) {
            float u = u_ * (2. * M_PI);
            float v = v_ * M_PI;

            float scale = 0.5;
            float x = cos(u) * sin(v) * scale;
            float y = sin(u) * sin(v) * scale;
            float z = cos(v) * scale;

            mDummyObject->normal3f(x, y, z);
            mDummyObject->vertex3f(x, y, z);
        }
    }
    mDummyObject->end();
}

// -----------------------------------------------------------------------------

// We need to run init glew outside the cpp of QT because GLEW headers aren't
// compatible with <QOpenGLContext>
void initGlew()
{
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        // Problem: glewInit failed, something is seriously wrong.
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    glCheckError();
}

} // END namespace RenderSystem ================================================
