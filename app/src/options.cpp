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
// Matching include
#include "options.hpp"
// C++ includes
#include <iostream>
#include <string>
// C includes
#include <cstdlib>
// External includes
#define TCLAP_SETBASE_ZERO 1
#include <tclap/CmdLine.h>
// Local includes
#include "config.hpp"


using namespace std;


options parse_options(int argc, char **argv)
{
    try
    {
        TCLAP::CmdLine cmd("JIT Compiler for Brainfuck", ' ', BFJIT_VERSION);

        TCLAP::SwitchArg sw_optimize(
            "O",
            "optimize",
            "Enable optimizations",
            cmd,
            false);

        class constr : public TCLAP::Constraint<size_t>
        {
        public:
            constr() noexcept
            {
            }

            string description() const override
            {
                return "Must be less than 65536";
            }

            string shortID() const override
            {
                return "number of bytes";
            }

            bool check(const size_t& value) const override
            {
                return value < 65536;
            }
        } static constr;

        TCLAP::ValueArg<size_t> val_size(
            "s",
            "size",
            "Specify heap size",
            false,
            30000,
            &constr,
            cmd);

        TCLAP::SwitchArg sw_pbrain(
            "p",
            "pbrain",
            "Enables pbrain extension",
            cmd,
            false);

        TCLAP::UnlabeledValueArg<string> val_file(
            "file",
            "Input file",
            true,
            "",
            "path",
            cmd);

        cmd.parse(argc, argv);

        return { 
            sw_optimize.getValue(),
            val_size.getValue(),
            val_file.getValue(),
            sw_pbrain.getValue() };
    }
    catch (TCLAP::ArgException& e)
    {
        cerr << "Error: " << e.error() << " for arg " << e.argId() << endl;
        exit(EXIT_FAILURE);
    }
}
