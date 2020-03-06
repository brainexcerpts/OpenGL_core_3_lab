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
#ifndef GL_ASSERT_H
#define GL_ASSERT_H

/**
 *
 * OpenGL error management class.
 *
 */

#include <GL/glu.h>
#include <iostream>

#ifndef NDEBUG // debug mode

    #include <iostream>
    #include <cassert>

    #ifndef __TO_STR
        #define __TO_STR(x) __EVAL_STR(x)
        #define __EVAL_STR(x) #x
    #endif

    #ifdef __APPLE__ // MacOsX stuff

        #define glAssert(code) \
        code; \
        {\
            GLuint err = glGetError(); \
            if (err != GL_NO_ERROR) { \
                std::cerr<<"erreur OpenGL ("<<__FILE__<<":"<<__LINE__<<", "<<__TO_STR(code)<<\
                    ( (err == GL_INVALID_ENUM) ? " Invalid enum" : \
                    ( (err == GL_INVALID_VALUE) ? " Invalid value" : \
                    ( (err == GL_INVALID_OPERATION) ? " Invalid operation" : "unknown error") \
                    ) \
                    )\
                    << "("<<err<<")"<<std::endl; \
                assert(false); \
            } \
        }

        #define glCheckError() \
        {\
            GLuint err = glGetError(); \
            if (err != GL_NO_ERROR) { \
                std::cerr<<"erreur OpenGL ("<<__FILE__<<":"<<__LINE__<<\
                    ( (err == GL_INVALID_ENUM) ? " Invalid enum" : \
                    ( (err == GL_INVALID_VALUE) ? " Invalid value" : \
                    ( (err == GL_INVALID_OPERATION) ? " Invalid operation" : "unknown error") \
                    ) \
                    )\
                    << "("<<err<<")"<<std::endl; \
                assert(false); \
            } \
        }

    #else // Linux / windows stuff
        #define glAssert(code) \
        code; \
        {\
            GLuint err = glGetError(); \
            if (err != GL_NO_ERROR) { \
                std::cerr<<"erreur OpenGL ("<<__FILE__<<":"<<__LINE__<<", "<<__TO_STR(code)<<") : "<<(const char*)gluErrorString(err)<<"("<<err<<")"<<std::endl; \
                assert(false); \
            } \
        }

        #define glCheckError() \
        {\
            GLuint err = glGetError(); \
            if (err != GL_NO_ERROR) { \
                std::cerr<<"erreur OpenGL ("<<__FILE__<<":"<<__LINE__<<") : "<<(const char*)gluErrorString(err)<<"("<<err<<")"<<std::endl; \
                assert(false); \
            } \
        }
    #endif

#else // No debug
    #define glAssert(code) code;
    #define glCheckError()
#endif



#endif // GL_ASSERT_H
