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
#include "token.hpp"
// C++ includes
#include <ostream>
#include <string>
#include <vector>
// C includes
#include <cassert>
// Local includes
#include "position.hpp"


namespace bfjit
{
    using namespace std;


    token::token(const token::type& ty, const position& pos) noexcept
        : m_type(ty)
        , m_pos(pos)
    {
    }


    const token::type& token::ty() const noexcept
    {
        return this->m_type;
    }


    const position& token::pos() const noexcept
    {
        return this->m_pos;
    }


    ostream& operator<<(ostream& out, const token::type& ty)
    {
        static const auto strs{ vector<string>
            { "LESS"
            , "GREATER"
            , "PLUS"
            , "MINUS"
            , "DOT"
            , "COMMA"
            , "LBRACKET"
            , "RBRACKET"
            } };
        static const auto num_entries{
            static_cast<size_t>(token::type::__LAST__) };
        assert(strs.size() == num_entries);

        assert(ty < token::type::__LAST__);
        return out << strs[static_cast<size_t>(ty)];
    }


    ostream& operator<<(ostream& out, const token& tok)
    {
        return out << tok.ty() << ' ' << tok.pos();
    }
}
