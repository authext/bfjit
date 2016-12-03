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
#include "lex.hpp"
// C++ includes
#include <istream>
#include <vector>
// Local includes
#include "token.hpp"


namespace bfjit
{
    using namespace std;


    vector<token> lex(istream& stream)
    {
        auto tokens{ vector<token>{} };

        auto
            line  { position::line_type(1) },
            column{ position::column_type(1) };

        auto c{ char('\0') };
        while (stream >> c)
        {
            auto pos{ position(line, column) };

            switch (c)
            {
            case '<':
                tokens.emplace_back(token::type::LESS, pos);
                break;

            case '>':
                tokens.emplace_back(token::type::GREATER, pos);
                break;

            case '+':
                tokens.emplace_back(token::type::PLUS, pos);
                break;

            case '-':
                tokens.emplace_back(token::type::MINUS, pos);
                break;

            case '.':
                tokens.emplace_back(token::type::DOT, pos);
                break;

            case ',':
                tokens.emplace_back(token::type::COMMA, pos);
                break;

            case '[':
                tokens.emplace_back(token::type::LBRACKET, pos);
                break;

            case ']':
                tokens.emplace_back(token::type::RBRACKET, pos);
                break;

            case '\n':
                ++line;
                column = position::column_type(0);
                break;

            default:
                ; // Ignore
                break;
            }

            ++column;
        }

        return tokens;
    }
}
