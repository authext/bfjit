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
#include "parse.hpp"
// C++ includes
#include <ostream>
#include <vector>
// C includes
#include <cassert>
#include <cstdint>
// External includes
#include <mapbox/variant.hpp>
// Local includes
#include "ast.hpp"
#include "token.hpp"


namespace bfjit
{
    using namespace std;


    parse_error::parse_error(const type& ty, const position& pos)
        : m_type(ty)
        , m_pos(pos)
    {
    }

    
    const parse_error::type& parse_error::ty() const
    {
        return this->m_type;
    }


    const position& parse_error::pos() const
    {
        return this->m_pos;
    }


    std::ostream& operator<<(std::ostream& out, const parse_error::type& ty)
    {
        static const auto strs{ vector<string>
            { "MISMATCHED_BRACKETS"
            } };
        static const auto num_entries{
            static_cast<size_t>(parse_error::type::__LAST__) };
        assert(strs.size() == num_entries);

        assert(ty < parse_error::type::__LAST__);
        return out << strs[static_cast<size_t>(ty)];
    }


    ostream& operator<<(ostream& out, const parse_error& pe)
    {
        return out << pe.ty() << " " << pe.pos();
    }


    vector<token>::const_iterator parse_helper
    ( vector<token>::const_iterator begin_it
    , vector<token>::const_iterator end_it
    , vector<combined_ast>& vast
    )
    {
        for (auto it { begin_it }; it < end_it; ++it)
        {
            auto inner{ vector<combined_ast>{} };
            const auto& pos{ it->pos() };

            switch (it->ty())
            {
            case token::type::LESS:
                vast.emplace_back(movement_ast
                    ( pos
                    , movement_ast::type::LEFT
                    , 1
                    ));
                break;

            case token::type::GREATER:
                vast.emplace_back(movement_ast
                    ( pos
                    , movement_ast::type::RIGHT
                    , 1
                    ));
                break;

            case token::type::PLUS:
                vast.emplace_back(mutation_ast
                    ( pos
                    , mutation_ast::type::INC
                    , 1
                    ));
                break;

            case token::type::MINUS:
                vast.emplace_back(mutation_ast
                    ( pos
                    , mutation_ast::type::DEC
                    , 1
                    ));
                break;

            case token::type::DOT:
                vast.emplace_back(io_ast
                    ( pos
                    , io_ast::type::OUT
                    ));
                break;

            case token::type::COMMA:
                vast.emplace_back(io_ast
                    ( pos
                    , io_ast::type::IN
                    ));
                break;

            case token::type::LBRACKET:
                it = parse_helper(it + 1, end_it, inner);
                vast.emplace_back(loop_ast
                ( pos
                , inner
                ));
                break;

            case token::type::RBRACKET:
                return it;

            default:
                break;
            }
        }

        return end_it;
    }


    parse_result parse
    ( vector<token>::const_iterator begin_it
    , vector<token>::const_iterator end_it
    )
    {
        auto v{ vector<combined_ast>{} };
        const auto it{ parse_helper(begin_it, end_it, v) };

        if (end_it != it)
        {
            return { parse_error
                { parse_error::type::MISMATCHED_BRACKETS
                , it->pos()
                } };
        }
        else
        {
            return { v };
        }
    }
}
