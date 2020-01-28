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
#include "mainwindow.h"

#include <QApplication>

#include "qt_gui/openglwidget.h"

#include <QSettings>
#include <QMessageBox>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextStream>
#include <QCloseEvent>
#include <QFileDialog>

#include <iostream>

// =============================================================================
namespace Gui {
// =============================================================================


MainWindow::MainWindow()
    : curFile("")
    , fileMenu(0)
    , renderMenu(0)
{
    std::cout << std::endl
              << " -------- IG3D Renderer -------- "
              << std::endl
              << std::endl;

    // Create main windows actions, menu and status bar
    createActions();
    createMenus();
    createStatusBar();

    // Get persistent settings from system
    readSettings();

    // Create OpenGL widget
    openGLWindow = new OpenGLWidget();
    openGLWindow->setVisible(false);

    // Integrate OpenGL widget in the main window
    QWidget* glWidget = QWidget::createWindowContainer(openGLWindow, this);
    setCentralWidget(glWidget);
    connect(openGLWindow, SIGNAL(fpsChanged(const QString&)),
            this, SLOT(statusChanged(const QString&)));
}

// -----------------------------------------------------------------------------

void MainWindow::clear()
{
    delete openGLWindow;
    delete fileMenu;
    delete renderMenu;
}

// -----------------------------------------------------------------------------

MainWindow::~MainWindow()
{
    clear();
}

// -----------------------------------------------------------------------------

void MainWindow::closeEvent(QCloseEvent* event)
{
    writeSettings();
    clear();
    event->accept();
}

// -----------------------------------------------------------------------------

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        loadFile(fileName);
}

// -----------------------------------------------------------------------------

void MainWindow::createActions()
{
    openAct = new QAction(QIcon(":/fileopen.xpm"), tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    checkResetCamera = new QAction(tr("&Reset Camera"), this);
    checkResetCamera->setShortcut(tr("Ctrl+R"));
    checkResetCamera->setStatusTip(tr("Reset Camera attributes"));
    connect(checkResetCamera, SIGNAL(triggered()), this, SLOT(resetCamera()));

    checkReloadShaders = new QAction(tr("Reload &Shaders"), this);
    checkReloadShaders->setShortcut(tr("Ctrl+S"));
    checkReloadShaders->setStatusTip(tr("Reload Shaders"));
    connect(checkReloadShaders, SIGNAL(triggered()), this, SLOT(reloadShaders()));
}

// -----------------------------------------------------------------------------

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    renderMenu = menuBar()->addMenu(tr("&Rendering"));
    renderMenu->addAction(checkResetCamera);
    renderMenu->addSeparator();
    renderMenu->addAction(checkReloadShaders);
}

// -----------------------------------------------------------------------------

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

// -----------------------------------------------------------------------------

void MainWindow::readSettings()
{
    QSettings settings("MinimalRenderer", "MinimalRenderer");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(800, 690)).toSize();
    resize(size);
    move(pos);
}

// -----------------------------------------------------------------------------

void MainWindow::writeSettings()
{
    QSettings settings("MinimalRenderer", "IG3D Rendering System");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

// -----------------------------------------------------------------------------

void MainWindow::loadFile(const QString& fileName)
{
    QFile file(fileName);
    QString reason;
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);


    //bool succeed = loadScene(fileName, reason);

    bool succeed = true;

    QApplication::restoreOverrideCursor();

    if (succeed) {
        reset();
        setCurrentFile(fileName);
        statusBar()->showMessage(tr("File loaded"), 2000);
        openGLWindow->updateGL();
    }
    else {
        QMessageBox::warning(this, tr("Application"), tr("Cannot read file %1\n%2").arg(fileName));
        statusBar()->showMessage(tr("Error loading file"), 2000);
    }
}

// -----------------------------------------------------------------------------

void MainWindow::setCurrentFile(const QString& fileName)
{
    curFile = fileName;

    QString shownName;
    if (curFile.isEmpty())
        shownName = "untitled.obj";
    else
        shownName = strippedName(curFile);
    /*save the name of the file without extension*/
    mNameFile = shownName;
    mNameFile.truncate(shownName.indexOf(".", 0));
    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Application")));
}

// -----------------------------------------------------------------------------

QString MainWindow::strippedName(const QString& fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

// -----------------------------------------------------------------------------

void MainWindow::reset()
{
    resetCamera();
}

// -----------------------------------------------------------------------------

void MainWindow::statusChanged(const QString& message)
{
    statusBar()->showMessage(message);
}

// -----------------------------------------------------------------------------

void MainWindow::reloadShaders()
{
    openGLWindow->reloadShaders();
    openGLWindow->updateGL();
}

// -----------------------------------------------------------------------------

void MainWindow::resetCamera()
{
    openGLWindow->resetView();
    openGLWindow->updateGL();
}

// -----------------------------------------------------------------------------

} // END namespace gui =========================================================
