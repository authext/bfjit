/***************************************************************************
*                                                                         *
* libbfjit                                                                *
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
#ifndef OPTIMIZE_HPP
#define OPTIMIZE_HPP


// C++ includes
#include <vector>
// Local includes
#include "ast.hpp"


namespace bfjit
{
    /// \brief Optimizes the input \ref combined_ast%.
    /// \param begin_it An iterator to the beginning of ast to be optimized.
    /// \param end_it An iterator to the end of ast to be optimized.
    std::vector<combined_ast> optimize
    ( std::vector<combined_ast>::const_iterator begin_it
    , std::vector<combined_ast>::const_iterator end_it
    );
}


#endif
