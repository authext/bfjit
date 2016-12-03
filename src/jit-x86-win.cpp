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
#include <config.hpp>
#if (defined(TARGET_ARCH) && TARGET_ARCH == ARCH_X86) &&\
    (defined(TARGET_OS) && TARGET_OS == OS_WINDOWS)
// Matching includes
#include "jit.hpp"
#include "jit-x86.hpp"
// C++ includes
#include <iostream>
// Windows includes
#include "Windows.h-shim.hpp"


namespace bfjit
{
    using namespace std;


    void jit_function::operator()() const
    {
        auto& assembled{ m_impl->m_assembled };
        auto start_address{ &assembled[0] };
        const auto byte_size{ assembled.size() * sizeof(*start_address) };

        DWORD old_protect, useless_protect;

        if (VirtualProtect
            ( start_address
            , byte_size
            , PAGE_EXECUTE_READWRITE
            , &old_protect
            ) == 0)
        {
            const auto error{ GetLastError() };

            cout << "VirtualProtect failed with: " << error << endl;
            exit(error);
        }

        reinterpret_cast<void (*)()>(start_address)();

        if (VirtualProtect
            ( start_address
            , byte_size
            , old_protect
            , &useless_protect
            ) == 0)
        {
            const auto error{ GetLastError() };

            cout << "VirtualProtect failed with: " << error << endl;
            exit(error);
        };
    }
}


#endif
