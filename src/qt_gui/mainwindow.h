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
#ifndef VORTEXRENDERER_H
#define VORTEXRENDERER_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QWidgetAction>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>

#include "openglwidget.h"


/** @defgroup InterfaceSystem Qt-OpenGL graphical interface
 *  Graphical User Interface of a minimal OpenGL Renderer
 * @author Mathias Paulin <Mathias.Paulin@irit.fr>
 *  @{
 */

/**
  * @ingroup InterfaceSystem
  * Graphical User Interface classes and functions
  */
// =============================================================================
namespace Gui {
// =============================================================================

/**
  * @ingroup InterfaceSystem
  * The main window of the application.
  * The main window contains a menu bar, a status bar and a central region where openGL rendering will be done.
  * This class manage the general interaction between user and application.
  */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    void updateGL() const
    {
        openGLWindow->updateGL();
    }

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void open();
    void statusChanged(const QString& message);

    void resetCamera();
    void reloadShaders();


private:
    void clear();
    void createActions();
    void createMenus();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    void loadFile(const QString& fileName);
    void setCurrentFile(const QString& fileName);
    QString strippedName(const QString& fullFileName);
    void reset();


    OpenGLWidget* openGLWindow;
    QString curFile;

    QMenu* fileMenu;
    QMenu* renderMenu;
    QAction* openAct;
    QAction* exitAct;
    QAction* checkResetCamera;
    QAction* checkReloadShaders;
    QSize getSize();
    QString mNameFile;
};


} // END namespace gui =========================================================

/** @} */ // end of InterfaceSystem group


#endif
