This C++/Qt project defines the foundations to render 3D objects with OpenGl 3 Core profile 
OpenGl Core profile is NOT OpenGL legacy mode, in legacy mode you can use glBegin() and glEnd() direct draw commands which is not the case here.
We provide a "***.obj" parser for loading 3D objects as well as a rudimentary render loop and Window creation.

The student will have to code camera / shader setup / handle matrices etc.

=============
Dependencies
=============
You only need QT 5.0 or superior to build this project, however,
If its 5.3.1 there is a known bug (https://bugreports.qt-project.org/browse/QTBUG-39859), 
you will need to install libEGL (in ubuntu package libegl1-mesa-dev) otherwise CMake will complain:
The imported target "Qt5::Gui" references the file "Qt5Gui_EGL_LIBRARY-NOTFOUND"

==================
More instructions
==================
The assignments and compilation instructions are in "00_INSTURCTIONS/html/index.html "
which are generated from "./00_INSTURCTIONS/lab_instructions/sujet-iig3d.h" you can run "./00_INSTURCTIONS/run build_doc.sh"
under linux.

The first assignment only requires to fill in the file src/rendersystem/renderer.cpp which contains a lot of explanations.

If your are a teacher the answers should be in the folder 00_ANSWERS

