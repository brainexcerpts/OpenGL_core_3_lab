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
#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H


#include "rendersystem/renderer.h"
#include <QWindow>
#include <QOpenGLContext>
#include <QImage>


// =============================================================================
namespace Gui {
// =============================================================================

/** @ingroup InterfaceSystem
 *  Qt-OpenGL interface component.
 *  This specialized QGLWidget manage the OpenGL rendering and its interaction with the windowing system.
 */
class OpenGLWidget : public QWindow {
    Q_OBJECT
public:
    OpenGLWidget(QScreen* screen = 0);

    virtual ~OpenGLWidget();
    void resetView();
    void reloadShaders();

    void makeCurrent(){ mContext->makeCurrent(this); }
    void swapBuffers(){ mContext->swapBuffers(this); }

    void printContextInfos();

signals:
    void fpsChanged ( const QString & );

public slots:
    virtual void initializeGL();
    virtual void resizeGL();
    /// Rendering loop (where OpenGl drawing happens)
    virtual void paintGL();
    virtual void updateGL(){ paintGL(); }

protected:
    virtual void mousePressEvent  ( QMouseEvent* e );
    virtual void mouseReleaseEvent( QMouseEvent* e );
    virtual void mouseMoveEvent   ( QMouseEvent* e );
    virtual void wheelEvent       ( QWheelEvent* e );
    virtual void keyPressEvent    ( QKeyEvent*   e );
    virtual void exposeEvent      (QExposeEvent* e );
private:
    void initGlew();

    int mWidth;
    int mHeight;

    RenderSystem::Renderer* m_theRenderer;

    QOpenGLContext* mContext;
};

} // END namespace gui =========================================================

#endif // OPENGLWIDGET_H
