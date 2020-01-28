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
#ifndef RENDERER_H
#define RENDERER_H

#include "glm/glm.hpp"

#include <vector>
class GlDirectDraw;

/** @defgroup RenderSystem Simple OpenGL Rendering system
 *  Simple OpenGL 3.2 core renderer.
 * @author Mathias Paulin <Mathias.Paulin@irit.fr>
 * edited by Rodolphe Vaillant <vaillant@irit.fr>
 */

/// Render system classes and functions.
// =============================================================================
namespace RenderSystem {
// =============================================================================

class MyGLMesh;


/**
  * @ingroup RenderSystem
  * Mouse event structure.
  * Mouse interaction events as the renderer will understand them.
  * It is the role of the graphical user interface to translate native mouse
  * event (Qt, WIN32, X11, ...) to this representation.
  */
struct MouseEvent {
    enum Modifiers { NONE = 0x00,
                     CONTROL = 0x01,
                     SHIFT = 0x02,
                     ALT = 0x04 };
    enum Buttons { LEFT = 0x01,
                   MIDDLE = 0x02,
                   RIGHT = 0x04,
                   WHEEL = 0x08,
                   MOVE = 0x10 };
    int button;
    bool click; // true -> click, false -> release
    int modifiers;
    int x;
    int y;
    int delta;
};

// -----------------------------------------------------------------------------

/**
  * @ingroup RenderSystem
  * OpenGL renderer.
  * Manages all of the OpenGL operation to render a scene.
  */
class Renderer {
public:
    /// Default constructor
    Renderer()
        : mWidth(-1)
        , mHeight(-1)
        , mProgram(-1)
        , mVertexShaderId(-1)
        , mFragmentShaderId(-1)
        , mViewMatrix(1.0f)
    {
    }

    /// Destructor
    virtual ~Renderer();

    /// VieViewport management.
    virtual void setViewport(int width, int height);

    /// Render the scene.
    virtual void render();

    /// Handle mouse event given by the vortexEngine
    /// @return 1 if event is understood and fully managed. 0 otherwise.
    virtual int handleMouseEvent(const MouseEvent& event);

    /// Handle key event given by the vortexEngine
    /// @return 1 if event is understood and fully managed. 0 otherwise.
    virtual int handleKeyEvent(char key);

    /// Initialise all the resources needed for rendering : shaders, geometries, teture, ...
    virtual void initRessources();

    /// Initialise the viewing configuration
    virtual void initView();

    /// Initialise the geometric content
    virtual void initGeometry();

    /// Initialise the shader configuration
    virtual void initShaders();

    /// Delete renderer's shaders
    virtual void clearShaders();

    int width() const
    {
        return mWidth;
    }
    int height() const
    {
        return mHeight;
    }

protected:
    int mWidth;
    int mHeight;

private:
    void init_dummy_object();

    /// Vector of meshes to be drawn.
    std::vector<MyGLMesh*> mMeshes;

    /// OpenGl Shader Program to be used when drawing.
    int mProgram;
    int mVertexShaderId;
    int mFragmentShaderId;

    /// Viewing matrix for the rendering.
    glm::mat4 mViewMatrix;

    /// An utility to draw objects easily as in the old Opengl 2.1
    GlDirectDraw* mDummyObject;
};

// -----------------------------------------------------------------------------

// Load OpenGL extensions with GLEW
void initGlew();

} // END namespace rendersystem ================================================

/** @} */ // end of RenderSystem group

#endif
