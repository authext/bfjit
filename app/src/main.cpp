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
// C++ includes
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
// C includes
#include <cstdint>
// External includes
#include <mapbox/variant.hpp>

#include <lex.hpp>
#include <jit.hpp>
#include <optimize.hpp>
#include <parse.hpp>
// Local includes
#include "options.hpp"


using namespace std;


int main(int argc, char **argv)
{
    const auto opts{ parse_options(argc, argv) };

    auto bytes{ make_unique<uint8_t[]>(opts.size) };
    ifstream file(opts.file_name);

    const auto  t{ bfjit::lex(file) };
    const auto  a{ bfjit::parse(begin(t), end(t)) };

    a.match
    ( [](const bfjit::parse_error& pe)
        {
            cerr << "Error: " << pe;
        }
    , [&bytes, &opts](const vector<bfjit::combined_ast>& ast)
        {
            const auto& o
            { opts.optimize ? bfjit::optimize(begin(ast), end(ast)) : ast };

            bfjit::jit_function::make(begin(o), end(o), bytes.get())();
        }
    );
}
