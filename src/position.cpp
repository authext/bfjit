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
// Matching include
#include "position.hpp"
// C++ includes
#include <ostream>
// C includes
#include <cassert>


namespace bfjit
{
    using namespace std;


    position::position
    ( const position::line_type& line
    , const position::column_type& column
    ) noexcept
        : m_line(line)
        , m_column(column)
    {
        assert(line >= 1);
        assert(column >= 1);
    }


    const position::line_type& position::line() const noexcept
    {
        return this->m_line;
    }


    const position::column_type& position::column() const noexcept
    {
        return this->m_column;
    }



    ostream& operator<<(ostream& out, const position& p)
    {
        return out << p.line() << ':' << p.column();
    }
}
