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
#ifndef PARSE_HPP
#define PARSE_HPP


// C++ includes
#include <iosfwd>
#include <vector>
// External includes
#include <mapbox/variant.hpp>
// Local includes
#include "ast.hpp"
#include "position.hpp"
#include "token.hpp"


namespace bfjit
{
    class parse_error
    {
    public:
        // Associated types
        /// \brief An enum that contains all possible reasons that
        /// parsing can fail.
        enum class type : std::uint_fast8_t
        { MISMATCHED_BRACKETS ///< Indicates that brackets where mismatched.
        , __LAST__ ///< Number of enum entries. **INVALID VALUE**
        };


        // Constructors
        /// \brief Constructs a new object, given its type and position.
        parse_error(const type& ty, const position& pos);

        // Accessors
        /// \brief Returns the type of this error.
        const type& ty() const;
        /// \brief Returns the position of this error.
        const position& pos() const;


    private:
        type m_type;
        position m_pos;
    };


    /// A function that pretty-prints a \ref parse_error::type object.
    std::ostream& operator<<(std::ostream& out, const parse_error::type& ty);
    /// A function that pretty-prints a \ref parse_error object.
    std::ostream& operator<<(std::ostream& out, const parse_error& pe);


    /// \brief A typedef that represents the parsing result.
    ///
    /// If parsing fails, the variant is set to an appropriate
    /// error message. If the parsing fails, the variant is set
    /// to a vector containing the ast.
    using parse_result = mapbox::util::variant
        < parse_error
        , std::vector<combined_ast>
        >;


    /// \brief A function that parses tokens into ast.
    ///
    /// \param begin_it An iterator to the beginning of a token sequence.
    /// \param end_it An iterator to the end of a token sequence.
    parse_result parse
    ( std::vector<token>::const_iterator begin_it
    , std::vector<token>::const_iterator end_it
    );
}


#endif
