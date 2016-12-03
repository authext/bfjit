/**************************************************************************
*                                                                         *
* bfjit                                                                   *
* Copyright © 2016 Aleksandar-Vuk Pavlović                                *
*                                                                         *
* This program is free software; you can redistribute it and/or modify    *
* it under the terms of the GNU General Public License as published by    *
* the Free Software Foundation; either version 2 of the License, or       *
* (at your option) any later version.                                     *
*                                                                         *
* This program is distributed in the hope that it will be useful,         *
* but WITHOUT ANY WARRANTY; without even the implied warranty of          *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
* GNU General Public License for more details.                            *
*                                                                         *
* You should have received a copy of the GNU General Public License along *
* with this program; if not, see <http://www.gnu.org/licenses/>, or       *
* write to the Free Software Foundation, Inc., 51 Franklin Street,        *
* Fifth Floor, Boston, MA 02110-1301 USA.                                 *
*                                                                         *
***************************************************************************/
#ifndef OPTIONS_HPP
#define OPTIONS_HPP


// C++ includes
#include <string>
// C includes
#include <cstdint>


struct options
{
    bool optimize; ///< Specifies whether to optimize the generated ast.
    std::size_t size; ///< Specifies the size of the heap.
    std::string file_name; ///< Specifies the name of the input file
    bool pbrain; ///< Specifies whether to enable pbrain extension.
};


/// \brief Parses the options given to the program.
/// \param argc The same argc from main.
/// \param argv The same argv from main.
options parse_options(int argc, char **argv);


#endif
