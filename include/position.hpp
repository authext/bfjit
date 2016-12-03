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
#ifndef POSITION_HPP
#define POSITION_HPP


// C++ includes
#include <iosfwd>
// C includes
#include <cstdint>


namespace bfjit
{
    /// \brief A class that tracks position of a token in a stream.
    ///
    /// This class contains the line and column information of a token.\n
    /// Both the line and column count start from 1, instead of 0.\n
    /// Like other \ref bfjit classes, objects of this class are **immutable**.
    class position
    {
    public:
        // Associated types
        /// A simple typedef for line numbers.
        using line_type   = std::uint_fast32_t;
        /// A simple typedef for column numbers.
        using column_type = std::uint_fast32_t;

        // Constructors
        /// \brief Constructs a new object, given the line and column information.
        ///
        /// \remark Line and column should both start from 1, not from 0.
        explicit position
        ( const line_type& line
        , const column_type& column
        ) noexcept;
        /// \brief Constructs a new object, given an existing object.
        position(const position& src) noexcept = default;
        /// \brief Constructs a new object, given an existing temporary object.
        position(position&& src) noexcept = default;

        // Destructor
        /// \brief Destroys the object.
        ~position() noexcept = default;

        // Accessors
        /// Returns the line number
        const line_type& line() const noexcept;
        /// Returns the column number
        const column_type& column() const noexcept;


    private:
        line_type   m_line;
        column_type m_column;
    };


    /// A functions that pretty-prints a \ref position object to a stream.
    std::ostream& operator<<(std::ostream& out, const position& p);
}


#endif
