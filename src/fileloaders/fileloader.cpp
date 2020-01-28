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
#include "fileloader.h"

namespace Loaders {
namespace Text {

//! side effect : seekg to begin of file
unsigned long getFileLength(std::ifstream& file)
{
    if (!file.good()) return 0;

    file.seekg(0, std::ios::end);
    unsigned long len = file.tellg();
    file.seekg(std::ios::beg);

    return len;
}

char* loadFile(const char *fileName)
{
    std::ifstream file;
    file.open(fileName, std::ios::in);
    if (!file) {
        std::cerr << "could not open file " << fileName << std::endl;
        char *shaderSource = new char[1];
        shaderSource[0]=0;
        return shaderSource;
    }

    unsigned long len = getFileLength(file);
    if (len == 0) {
        char *shaderSource = new char[1];
        shaderSource[0]=0;
        return shaderSource;
    }

    char *shaderSource = new char[len+1];

    // len isn't always strlen cause some characters are stripped in ascii read...
    // it is important to 0-terminate the real length later, len is just max possible value...
    shaderSource[len] = 0;
    unsigned int i = 0;
    while (file.good()) {
        // get character from file.
        shaderSource[i] = file.get();
        if (!file.eof()) i++;
    }

    shaderSource[i] = 0;  // 0 terminate it.
    file.close();
    return shaderSource;
}

} // end namespace file
} // end namespace loaders

