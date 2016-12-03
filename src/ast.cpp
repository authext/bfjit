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
#include "ast.hpp"
// C++ includes
#include <ostream>
#include <string>
#include <vector>
// C includes
#include <cassert>
// External includes
#include <mapbox/variant.hpp>


namespace bfjit
{
    using namespace std;


    // ast
    ast::ast(const position& pos) noexcept
        : m_pos(pos)
    {
    }


    const position& ast::pos() const noexcept
    {
        return this->m_pos;
    }


    // movement_ast
    movement_ast::movement_ast
    ( const position& pos
    , const movement_ast::type& ty
    , const size_t& by
    ) noexcept
        : ast(pos)
        , m_type(ty)
        , m_by(by)
    {
    }


    const movement_ast::type& movement_ast::ty() const noexcept
    {
        return this->m_type;
    }


    const size_t& movement_ast::by() const noexcept
    {
        return this->m_by;
    }


    ostream& operator<<(ostream& out, const movement_ast::type& ty)
    {
        static const auto strs{ vector<string>
            { "LEFT"
            , "RIGHT"
            } };
        static const auto num_entries{
            static_cast<size_t>(movement_ast::type::__LAST__) };
        assert(strs.size() == num_entries);

        assert(ty < movement_ast::type::__LAST__);
        return out << strs[static_cast<size_t>(ty)];
    }


    ostream& operator<<(ostream& out, const movement_ast& mast)
    {
        return out << mast.ty() << ' ' << mast.by() << ' ' << mast.pos();
    }


    // mutation_ast
    mutation_ast::mutation_ast
    ( const position& pos
    , const mutation_ast::type& ty
    , const size_t& by
    ) noexcept
        : ast(pos)
        , m_type(ty)
        , m_by(by)
    {
    }


    const mutation_ast::type& mutation_ast::ty() const noexcept
    {
        return this->m_type;
    }


    const size_t& mutation_ast::by() const noexcept
    {
        return this->m_by;
    }


    ostream& operator<<(ostream& out, const mutation_ast::type& ty)
    {
        static const auto strs{ vector<string>
            { "INC"
            , "DEC"
            } };
        static const auto num_entries{
            static_cast<size_t>(mutation_ast::type::__LAST__) };
        assert(strs.size() == num_entries);

        assert(ty < mutation_ast::type::__LAST__);
        return out << strs[static_cast<size_t>(ty)];
    }


    ostream& operator<<(ostream& out, const mutation_ast& mast)
    {
        return out << mast.ty() << ' ' << mast.by() << ' ' << mast.pos();
    }


    // io_ast
    io_ast::io_ast(const position& pos, const io_ast::type& ty) noexcept
        : ast(pos)
        , m_type(ty)
    {	
    }


    const io_ast::type& io_ast::ty() const noexcept
    {
        return this->m_type;
    }


    ostream& operator<<(ostream& out, const io_ast::type& ty)
    {
        static const auto strs{ vector<string>
            { "IN"
            , "OUT"
            } };
        static const auto num_entries{
            static_cast<size_t>(io_ast::type::__LAST__) };
        assert(strs.size() == num_entries);

        assert(ty < io_ast::type::__LAST__);
        return out << strs[static_cast<size_t>(ty)];
    }


    ostream& operator<<(ostream& out, const io_ast& ioast)
    {
        return out << ioast.ty() << ' ' << ioast.pos();
    }


    // loop_ast
    loop_ast::loop_ast
    ( const position& pos
    , const vector<combined_ast>& inner)
        : ast(pos)
        , m_inner(inner)
    {	
    }


    const vector<combined_ast>& loop_ast::inner() const noexcept
    {
        return this->m_inner;
    }


    ostream& operator<<(ostream& out, const loop_ast& ioast)
    {
        for (const auto& e : ioast.inner())
        {
            e.match(
                [&out](const movement_ast& m_ast) { out << m_ast; },
                [&out](const mutation_ast& m_ast) { out << m_ast; },
                [&out](const io_ast& i_ast)       { out << i_ast; },
                [&out](const loop_ast& l_ast)     { out << l_ast; });
        }

        return out;
    }
}
