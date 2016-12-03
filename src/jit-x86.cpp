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
#include "config.hpp"
#if defined(TARGET_ARCH) && TARGET_ARCH == ARCH_X86
// Matching includes
#include "jit.hpp"
#include "jit-x86.hpp"
// C++ includes
#include <iterator>
#include <memory>
// C includes
#include <cstdint>
// External includes
#include <mapbox/variant.hpp>


namespace bfjit
{
    using namespace std;


    void assemble
    ( vector<combined_ast>::const_iterator begin_it
    , vector<combined_ast>::const_iterator end_it
    , jit_function::impl *impl
    );


    jit_function::impl::impl(uint8_t *bytes)
        : m_assembled()
        , m_bytes(bytes)
    {
    }


    jit_function::impl::impl(impl&& src) noexcept
        : m_assembled(move(src.m_assembled))
        , m_bytes(src.m_bytes)
    {
    }

    
    jit_function::jit_function(unique_ptr<impl>&& src)
        : m_impl(move(src))
    {
    }


    jit_function::~jit_function()
    {
    }



    void prologue(jit_function::impl* impl)
    {
        auto& assembled{ impl->m_assembled };

        assembled.emplace_back(0x55); // push ebp
        assembled.emplace_back(0x89); // mov ebp, esp
        assembled.emplace_back(0xE5);

        assembled.emplace_back(0x53); // push ebx
        assembled.emplace_back(0x56); // push esi
        assembled.emplace_back(0x57); // push edi

        const auto put_le_dword = [&assembled](const uintptr_t& ptr_int)
        {
            assembled.emplace_back(static_cast<uint8_t>(
                (ptr_int >> 0) & 0xFF));
            assembled.emplace_back(static_cast<uint8_t>(
                (ptr_int >> 8) & 0xFF));
            assembled.emplace_back(static_cast<uint8_t>(
                (ptr_int >> 16) & 0xFF));
            assembled.emplace_back(static_cast<uint8_t>(
                (ptr_int >> 24) & 0xFF));
        };

        const auto bytes_int{ reinterpret_cast<uintptr_t>(impl->m_bytes) };
        assembled.emplace_back(0xBB); // mov ebx, bytes
        put_le_dword(bytes_int);

        const auto getchar_int { reinterpret_cast<uintptr_t>(getchar) };
        assembled.emplace_back(0xBE); // mov esi, getchar
        put_le_dword(getchar_int);

        const auto putchar_int { reinterpret_cast<uintptr_t>(putchar) };
        assembled.emplace_back(0xBF); // mov edi, putchar
        put_le_dword(putchar_int);
    }


    void epilogue(jit_function::impl* impl)
    {
        auto& assembled{ impl->m_assembled };

        assembled.emplace_back(0x5F); // pop edi
        assembled.emplace_back(0x5E); // pop esi
        assembled.emplace_back(0x5B); // pop ebx

        assembled.emplace_back(0xC9); // leave
        assembled.emplace_back(0xC3); // ret
    }


    void movement(jit_function::impl* impl, const movement_ast& m_ast)
    {
        auto& assembled{ impl->m_assembled };

        if (m_ast.ty() == movement_ast::type::LEFT)
            assembled.emplace_back(0x4B); // dec ebx
        else if (m_ast.ty() == movement_ast::type::RIGHT)
            assembled.emplace_back(0x43); // inc ebx
    }


    void mutation(jit_function::impl* impl, const mutation_ast& m_ast)
    {
        auto& assembled{ impl->m_assembled };

        if (m_ast.ty() == mutation_ast::type::INC)
        {
            assembled.emplace_back(0xFE); // inc byte [ebx]
            assembled.emplace_back(0x03);
        }
        else if (m_ast.ty() == mutation_ast::type::DEC)
        {
            assembled.emplace_back(0xFE); // dec byte [ebx]
            assembled.emplace_back(0x0B);
        }
    }


    void io(jit_function::impl* impl, const io_ast& i_ast)
    {
        auto& assembled{ impl->m_assembled };

        if (i_ast.ty() == io_ast::type::IN)
        {
            assembled.emplace_back(0xFF); // call esi
            assembled.emplace_back(0xD6);

            assembled.emplace_back(0x88); // mov byte [ebx], al
            assembled.emplace_back(0x03);
        }
        else if (i_ast.ty() == io_ast::type::OUT)
        {
            assembled.emplace_back(0x8A); // mov al, byte [ebx]
            assembled.emplace_back(0x03);

            assembled.emplace_back(0x0F); // movzx eax, al
            assembled.emplace_back(0xB6);
            assembled.emplace_back(0xC0);

            assembled.emplace_back(0x50); // push eax

            assembled.emplace_back(0xFF); // call edi
            assembled.emplace_back(0xD7);

            assembled.emplace_back(0x81); // add esp, 4
            assembled.emplace_back(0xC4);
            assembled.emplace_back(0x04);
            assembled.emplace_back(0x00);
            assembled.emplace_back(0x00);
            assembled.emplace_back(0x00);
        }
    }


    void loop(jit_function::impl* impl, const loop_ast& l_ast)
    {
        auto& assembled{ impl->m_assembled };

        const auto start_idx{ assembled.size() };
        assembled.emplace_back(0xF6); // test byte [ebx], 0xff
        assembled.emplace_back(0x03);
        assembled.emplace_back(0xFF);

        assembled.emplace_back(0x0F); // will be jz end
        assembled.emplace_back(0x84);
        const auto fill_idx{ assembled.size() };
        assembled.emplace_back(0x00);
        assembled.emplace_back(0x00);
        assembled.emplace_back(0x00);
        assembled.emplace_back(0x00);

        assemble
        ( cbegin(l_ast.inner())
        , cend(l_ast.inner())
        , impl
        );

        assembled.emplace_back(0xE9); // jmp start
        const auto diff{ start_idx - assembled.size() - 4 };
        assembled.emplace_back(static_cast<uint8_t>((diff >>  0) & 0xFF));
        assembled.emplace_back(static_cast<uint8_t>((diff >>  8) & 0xFF));
        assembled.emplace_back(static_cast<uint8_t>((diff >> 16) & 0xFF));
        assembled.emplace_back(static_cast<uint8_t>((diff >> 24) & 0xFF));

        const auto end_idx { assembled.size() };

        // Fill the jump from start to end
        const auto diff1{ end_idx - fill_idx - 4 };
        assembled[fill_idx + 0] = static_cast<uint8_t>((diff1 >>  0) & 0xFF);
        assembled[fill_idx + 1] = static_cast<uint8_t>((diff1 >>  8) & 0xFF);
        assembled[fill_idx + 2] = static_cast<uint8_t>((diff1 >> 16) & 0xFF);
        assembled[fill_idx + 3] = static_cast<uint8_t>((diff1 >> 24) & 0xFF);
    }


    void assemble
    ( vector<combined_ast>::const_iterator begin_it
    , vector<combined_ast>::const_iterator end_it
    , jit_function::impl *impl
    )
    {
        for (auto it{ begin_it }; it < end_it; ++it)
        {
            it->match
            ( [&impl](const movement_ast& m_ast) { movement(impl, m_ast); }
            , [&impl](const mutation_ast& m_ast) { mutation(impl, m_ast); }
            , [&impl](const io_ast& i_ast)       { io(impl, i_ast); }
            , [&impl](const loop_ast& l_ast)     { loop(impl, l_ast); }
            );
        }
    }


    jit_function jit_function::make
    ( vector<combined_ast>::const_iterator begin_it
    , vector<combined_ast>::const_iterator end_it
    , uint8_t *bytes
    )
    {
        auto pimpl{ make_unique<impl>(bytes) };
        prologue(pimpl.get());
        assemble(begin_it, end_it, pimpl.get());
        epilogue(pimpl.get());

        return { move(pimpl) };
    }
}


#endif
