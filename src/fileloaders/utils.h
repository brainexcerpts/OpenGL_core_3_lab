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
#ifndef UTILS_H
#define UTILS_H

#include <ostream>

/**
  * @defgroup Utilities Utilities
  * @author Mathias Paulin <Mathias.Paulin@irit.fr>
  */
/**
  * @ingroup Utilities
  * Utilities that are missing in used libraries.
 */
namespace Utils {

inline std::ostream &operator<<(std::ostream &stream, const glm::mat4x4 &m) {
    stream << m[0][0] << " " << m[0][1] << " " << m[0][2] << " " << m[0][3] << std::endl;
    stream << m[1][0] << " " << m[1][1] << " " << m[1][2] << " " << m[1][3] << std::endl;
    stream << m[2][0] << " " << m[2][1] << " " << m[2][2] << " " << m[2][3] << std::endl;
    stream << m[3][0] << " " << m[3][1] << " " << m[3][2] << " " << m[3][3] << std::endl;
    return stream;
}

inline std::ostream &operator<<(std::ostream &stream, const glm::vec3 &v) {
    stream << v[0] << " " << v[1] << " " << v[2];
    return stream;
}

inline void printMatrix(std::ostream &stream, const glm::mat4x4 &m) {
    stream << m[0][0] << " " << m[0][1] << " " << m[0][2] << " " << m[0][3] << std::endl;
    stream << m[1][0] << " " << m[1][1] << " " << m[1][2] << " " << m[1][3] << std::endl;
    stream << m[2][0] << " " << m[2][1] << " " << m[2][2] << " " << m[2][3] << std::endl;
    stream << m[3][0] << " " << m[3][1] << " " << m[3][2] << " " << m[3][3] << std::endl;
}


inline float sqlength ( const glm::vec3 & v ) {
    float sqr = v.x * v.x + v.y * v.y + v.z * v.z;
    return sqr;
}

inline glm::vec3 perpendicular (const glm::vec3 & v) {
    static const float fSquareZero = 1e-06 * 1e-06;
    glm::vec3 perp = glm::cross(v, glm::vec3(1,0,0));
    if (sqlength(perp)< fSquareZero) {
        /* This vector is the Y axis multiplied by a scalar, so we have
               to use another axis.
            */
        perp = glm::cross(v, glm::vec3(0,1,0));
    }
    return perp;
}

} // namespace util

#endif
