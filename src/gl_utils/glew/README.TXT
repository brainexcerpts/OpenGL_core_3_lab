This is the version of glew 1.10.0 modified to support Opengl 3.0 Core profile.
(you can find it here https://github.com/HeliumProject/glew)

In core profile, the original GLEW will cause an "invalid enumerant" error after glewInit() when tested with glGetError().
This is a longstanding known bug waiting to be patched years ago (https://sourceforge.net/p/glew/patches/40/) ...

In this version I also modified header paths of glew.h wglew.h and glxew.h. I also added #define STATIC_GLEW in glew.c
