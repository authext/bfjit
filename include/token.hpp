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
#ifndef TOKEN_HPP
#define TOKEN_HPP


// C++ includes
#include <iosfwd>
// C includes
#include <cstdint>
// Local includes
#include "position.hpp"


namespace bfjit
{
    /// \brief A class that represents a single token extracted from a stream
    ///
    /// This class contains the type and position information of a token.\n
    /// Like other \ref bfjit classes, objects of this class are **immutable**.
    class token
    {
    public:
        // Associated types
        /// \brief An enum that contains all possible types of tokens.
        enum class type : std::uint_fast8_t
        { LESS     ///< Represents the `<` character.
        , GREATER  ///< Represents the `>` character.
        , PLUS     ///< Represents the `+` character.
        , MINUS    ///< Represents the `-` character.
        , DOT      ///< Represents the `.`character.
        , COMMA    ///< Represents the `,` character.
        , LBRACKET ///< Represents the `[` character.
        , RBRACKET ///< Represents the `]` character.
        , __LAST__ ///< Number of enum entries. **INVALID VALUE**
        };

        // Constructors
        /// Constructs a new object, given the type and position.
        explicit token(const type& ty, const position& pos) noexcept;
        /// \brief Constructs a new object, given an existing object.
        token(const token& src) noexcept = default;
        /// \brief Constructs a new object, given an existing temporary object.
        token(token&& src) noexcept = default;

        // Destructor
        /// \brief Destroys the object.
        ~token() noexcept = default;

        // Accessors
        /// Returns the type of this token.
        const type& ty() const noexcept;
        /// Returns the position of this token in the stream.
        const position& pos() const noexcept;


    private:
        type     m_type;
        position m_pos;
    };


    /// A function that pretty-prints a \ref token::type object.
    std::ostream& operator<<(std::ostream& out, const token::type& ty);
    /// A function that pretty-prints a \ref token object.
    std::ostream& operator<<(std::ostream& out, const token& tok);
}


#endif
