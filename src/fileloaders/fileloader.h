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
#ifndef FILELOADER_H
#define FILELOADER_H

#include <iostream>
#include <fstream>

// =============================================================================
namespace Loaders {
// =============================================================================

/**
  * @ingroup Loaders
  * Text file loader.
  * This namespace contains functions for loading text files.
  */
// =============================================================================
namespace Text {
// =============================================================================

/** @defgroup FileLoaders Simple text file utilities.
 *  @ingroup Loaders
 */

/// @ingroup FileLoaders
unsigned long getFileLength(std::ifstream& file);

/// @ingroup FileLoaders
char* loadFile(const char *fileName);

} // end namespace file ========================================================

} // end namespace loaders =====================================================

#endif // FILELOADER_H
