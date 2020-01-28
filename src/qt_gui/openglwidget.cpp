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
#include "openglwidget.h"

#include "timer.hpp"
#include "gl_utils/glassert.h"

#include <QWheelEvent>
#include <QApplication>
#include <iostream>

// =============================================================================
namespace Gui {
// =============================================================================

static void infoGL()
{
    const GLubyte* str;
    glAssert(str = glGetString(GL_RENDERER));
    std::cout << "Renderer : " << str << std::endl;
    glAssert(str = glGetString(GL_VENDOR));
    std::cout << "Vendor : " << str << std::endl;
    glAssert(str = glGetString(GL_VERSION));
    std::cout << "OpenGL Version : " << str << std::endl;
    glAssert(str = glGetString(GL_SHADING_LANGUAGE_VERSION));
    std::cout << "GLSL Version : " << str << std::endl;
}

// -----------------------------------------------------------------------------

OpenGLWidget::OpenGLWidget(QScreen* screen)
    : QWindow(screen)
    , mWidth(-1)
    , mHeight(-1)
    , m_theRenderer(nullptr)
    , mContext(0)
{
    setSurfaceType(OpenGLSurface);

    QSurfaceFormat format;
    //format.setDepthBufferSize(24);
    format.setMajorVersion(3);
    format.setMinorVersion(2);
    //format.setSamples(4);
    format.setProfile(QSurfaceFormat::CoreProfile);

    setFormat(format);
    create();

    mContext = new QOpenGLContext();
    mContext->setFormat(format);
    mContext->create();
    mContext->makeCurrent(this);

    initializeGL();
    m_theRenderer = new RenderSystem::Renderer();
    m_theRenderer->initRessources();

    resize(QSize(800, 450));

    connect(this, SIGNAL(widthChanged(int)), this, SLOT(resizeGL()));
    connect(this, SIGNAL(heightChanged(int)), this, SLOT(resizeGL()));
    //    QTimer *timer = new QTimer(this);
    //    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    //    timer->start(16);
}

// -----------------------------------------------------------------------------

OpenGLWidget::~OpenGLWidget()
{
    delete m_theRenderer;
    // Must be last to release all OpenGL objects before (VBOs shaders ...):
    delete mContext;
}

// -----------------------------------------------------------------------------

void OpenGLWidget::initializeGL()
{
    makeCurrent();
    glCheckError();
    printContextInfos();
    RenderSystem::initGlew();
    glCheckError();
}

// -----------------------------------------------------------------------------

void OpenGLWidget::printContextInfos()
{
    if (!mContext->isValid())
        std::cerr << "The OpenGL context is invalid!" << std::endl;

    mContext->makeCurrent(this);

    std::cout << "Window format version is: "
              << format().majorVersion() << "."
              << format().minorVersion() << std::endl;

    std::cout << "Context format version is: "
              << mContext->format().majorVersion()
              << "." << mContext->format().minorVersion() << std::endl;
    infoGL();
}

// -----------------------------------------------------------------------------

void OpenGLWidget::resizeGL()
{
    makeCurrent();
    mWidth = width();
    mHeight = height();
    m_theRenderer->setViewport(mWidth, mHeight);
}

// -----------------------------------------------------------------------------

void OpenGLWidget::paintGL()
{
    if (!isExposed())
        return;
    makeCurrent();

    static float time = 0.0;
    static int frames = 0;

    tbx::Timer t;
    t.start();

    // Draw here
    m_theRenderer->render();
#if 0
    // Should crash if we properly setup OpenGL context to be a core profile.
    // then glBegin() is forbiden.
    glBegin(GL_POINTS);
    glVertex3f(0.f, 0.f, 0.f);
    glEnd();
    glCheckError();
#endif

    //glFinish();
    time += t.elapsed();
    if (frames % 25 == 0) {
        float fps = ((float)frames) / time;
        QString thetext = QString("FPS : %1").arg(fps);
        emit fpsChanged(thetext);
        time = 0.0;
        frames = 0;
    }
    frames += 1;
    swapBuffers();
}

// -----------------------------------------------------------------------------

void OpenGLWidget::keyPressEvent(QKeyEvent* e)
{

    switch (e->key()) {
    case Qt::Key_Escape:
        QCoreApplication::instance()->quit();
        break;
    default:
        m_theRenderer->handleKeyEvent(e->text().toLatin1()[0]);
    }
    updateGL();
}

// -----------------------------------------------------------------------------

void OpenGLWidget::wheelEvent(QWheelEvent* e)
{
    RenderSystem::MouseEvent event;
    event.button = RenderSystem::MouseEvent::WHEEL;
    event.delta = (e->angleDelta() / 8).y();
    m_theRenderer->handleMouseEvent(event);
    updateGL();
}

// -----------------------------------------------------------------------------

void OpenGLWidget::mouseMoveEvent(QMouseEvent* e)
{
    RenderSystem::MouseEvent event;

    // dont need to know which button
    event.button = RenderSystem::MouseEvent::MOVE;

    event.modifiers = RenderSystem::MouseEvent::NONE;
    if (e->modifiers() & Qt::ShiftModifier)
        event.modifiers |= RenderSystem::MouseEvent::SHIFT;
    if (e->modifiers() & Qt::ControlModifier)
        event.modifiers |= RenderSystem::MouseEvent::CONTROL;
    if (e->modifiers() & Qt::AltModifier)
        event.modifiers |= RenderSystem::MouseEvent::ALT;

    event.click = false;
    event.x = e->x();
    event.y = e->y();
    event.delta = 0;

    m_theRenderer->handleMouseEvent(event);
    updateGL();
}

// -----------------------------------------------------------------------------

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* e)
{

    RenderSystem::MouseEvent event;

    if (e->button() & Qt::LeftButton)
        event.button = RenderSystem::MouseEvent::LEFT;
    if (e->button() & Qt::MidButton)
        event.button = RenderSystem::MouseEvent::MIDDLE;
    if (e->button() & Qt::RightButton)
        event.button = RenderSystem::MouseEvent::RIGHT;

    event.modifiers = RenderSystem::MouseEvent::NONE;
    if (e->modifiers() & Qt::ShiftModifier)
        event.modifiers |= RenderSystem::MouseEvent::SHIFT;
    if (e->modifiers() & Qt::ControlModifier)
        event.modifiers |= RenderSystem::MouseEvent::CONTROL;
    if (e->modifiers() & Qt::AltModifier)
        event.modifiers |= RenderSystem::MouseEvent::ALT;

#ifdef __APPLE__
    if ((e->modifiers() & Qt::ShiftModifier) && (e->button() & Qt::LeftButton))
        event.button = rendersystem::MouseEvent::MIDDLE;
    if ((e->modifiers() & Qt::ControlModifier) && (e->button() & Qt::LeftButton))
        event.button = rendersystem::MouseEvent::RIGHT;
#endif

    event.click = false;
    event.x = e->x();
    event.y = e->y();
    event.delta = 0;

    m_theRenderer->handleMouseEvent(event);
    updateGL();
}

// -----------------------------------------------------------------------------

void OpenGLWidget::mousePressEvent(QMouseEvent* e)
{
    RenderSystem::MouseEvent event;

    if (e->button() & Qt::LeftButton)
        event.button = RenderSystem::MouseEvent::LEFT;
    if (e->button() & Qt::MidButton)
        event.button = RenderSystem::MouseEvent::MIDDLE;
    if (e->button() & Qt::RightButton)
        event.button = RenderSystem::MouseEvent::RIGHT;

    event.modifiers = RenderSystem::MouseEvent::NONE;
    if (e->modifiers() & Qt::ShiftModifier)
        event.modifiers |= RenderSystem::MouseEvent::SHIFT;
    if (e->modifiers() & Qt::ControlModifier)
        event.modifiers |= RenderSystem::MouseEvent::CONTROL;
    if (e->modifiers() & Qt::AltModifier)
        event.modifiers |= RenderSystem::MouseEvent::ALT;

#ifdef __APPLE__
    if ((e->modifiers() & Qt::ShiftModifier) && (e->button() & Qt::LeftButton))
        event.button = rendersystem::MouseEvent::MIDDLE;
    if ((e->modifiers() & Qt::ControlModifier) && (e->button() & Qt::LeftButton))
        event.button = rendersystem::MouseEvent::RIGHT;
#endif

    event.click = true;
    event.x = e->x();
    event.y = e->y();
    event.delta = 0;

    m_theRenderer->handleMouseEvent(event);
    updateGL();
}

// -----------------------------------------------------------------------------

void OpenGLWidget::exposeEvent(QExposeEvent* e)
{
    if (isExposed()) {
        updateGL();
    }
}

// -----------------------------------------------------------------------------

void OpenGLWidget::resetView()
{
    m_theRenderer->initView();
    updateGL();
}

// -----------------------------------------------------------------------------

void OpenGLWidget::reloadShaders()
{
    m_theRenderer->clearShaders();
    m_theRenderer->initShaders();
    updateGL();
}

// -----------------------------------------------------------------------------

} // END namespace gui==========================================================
