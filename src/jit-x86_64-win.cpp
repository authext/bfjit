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
#if (defined(TARGET_ARCH) && TARGET_ARCH == ARCH_X86_64) &&\
    (defined(TARGET_OS) && TARGET_OS == OS_WINDOWS)
// Matching includes
#include "jit.hpp"
#include "jit-x86_64.hpp"
// C++ includes
#include <iostream>
// Windows includes
#include "Windows.h-shim.hpp"
// Local includes
#include "ast.hpp"


namespace bfjit
{
    using namespace std;


    void io(jit_function::impl* impl, const io_ast& i_ast)
    {
        auto& assembled{ impl->m_assembled };

        assembled.emplace_back(0x48); // sub rsp, 32
        assembled.emplace_back(0x83);
        assembled.emplace_back(0xEC);
        assembled.emplace_back(0x20);

        if (i_ast.ty() == io_ast::type::IN)
        {
            assembled.emplace_back(0x41); // call r13
            assembled.emplace_back(0xFF);
            assembled.emplace_back(0xD5);

            assembled.emplace_back(0x41); // mov byte [r12], al
            assembled.emplace_back(0x88);
            assembled.emplace_back(0x04);
            assembled.emplace_back(0x24);
        }
        else if (i_ast.ty() == io_ast::type::OUT)
        {
            assembled.emplace_back(0x41); // mov al, byte [r12]
            assembled.emplace_back(0x8A);
            assembled.emplace_back(0x04);
            assembled.emplace_back(0x24);

            assembled.emplace_back(0x48); // movzx rcx, al
            assembled.emplace_back(0x0F);
            assembled.emplace_back(0xB6);
            assembled.emplace_back(0xC8);

            assembled.emplace_back(0x41); // call r14
            assembled.emplace_back(0xFF);
            assembled.emplace_back(0xD6);
        }

        assembled.emplace_back(0x48); // add rsp, 32
        assembled.emplace_back(0x83);
        assembled.emplace_back(0xC4);
        assembled.emplace_back(0x20);
    }


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

        reinterpret_cast<void(*)()>(start_address)();

        if (VirtualProtect
            (start_address
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
