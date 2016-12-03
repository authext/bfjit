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
#if defined(TARGET_ARCH) && TARGET_ARCH == ARCH_ARM


// Matching includes
#include "jit.hpp"
#include "jit-arm.hpp"
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

        assembled.emplace_back(0xE92D41F0); // stmfd sp!, {r4-r8, lr}
        assembled.emplace_back(0xE28D700C); // add r7, sp, #12

        assembled.emplace_back(0xE1A04000); // mov r4, r0 (r0 = bytes)
        assembled.emplace_back(0xE1A05001); // mov r5, r1 (r1 = getchar)
        assembled.emplace_back(0xE1A06002); // mov r6, r2 (r2 = putchar)
    }


    void epilogue(jit_function::impl* impl)
    {
        auto& assembled{ impl->m_assembled };

        assembled.emplace_back(0xE8BD81F0); // ldmfd sp!, {r4-r8, pc}
    }


    void movement(jit_function::impl* impl, const movement_ast& m_ast)
    {
        auto& assembled{ impl->m_assembled };

        if (m_ast.ty() == movement_ast::type::LEFT)
            assembled.emplace_back(0xE2444001); // sub r4, r4, 1
        else if (m_ast.ty() == movement_ast::type::RIGHT)
            assembled.emplace_back(0xE2844001); // add r4, r4, 1
    }


    void mutation(jit_function::impl* impl, const mutation_ast& m_ast)
    {
        auto& assembled{ impl->m_assembled };

        assembled.emplace_back(0xE5D48000); // ldrb r8, [r4]

        if (m_ast.ty() == mutation_ast::type::INC)
            assembled.emplace_back(0xE2888001); // add r8, r8, 1
        else if (m_ast.ty() == mutation_ast::type::DEC)
            assembled.emplace_back(0xE2488001); // sub r8, r8, 1

        assembled.emplace_back(0xE5C48000); // strb r8, [r4]
    }


    void io(jit_function::impl* impl, const io_ast& i_ast)
    {
        auto& assembled{ impl->m_assembled };

        if (i_ast.ty() == io_ast::type::IN)
        {
            assembled.emplace_back(0xE12FFF35); // blx r5
            assembled.emplace_back(0xE5C40000); // strb r0, [r4]
        }
        else if (i_ast.ty() == io_ast::type::OUT)
        {
            assembled.emplace_back(0xE5D40000); // ldrb r0, [r4]
            assembled.emplace_back(0xE12FFF36); // blx r6
        }
    }


    void loop(jit_function::impl* impl, const loop_ast& l_ast)
    {
        auto& assembled{ impl->m_assembled };

        const auto start_idx{ assembled.size() };
        assembled.emplace_back(0xE5D48000); // ldrb r8, [r4]
        assembled.emplace_back(0xE0188008); // ands r8, r8, r8
        const auto fill_idx{ assembled.size() };
        assembled.emplace_back(0x0); // will be beq end

        assemble
        ( begin(l_ast.inner())
        , end(l_ast.inner())
        , impl
        );

        const auto jmp_offset1
        { (start_idx - assembled.size() - 2) & 0x00FFFFFF
        };
        assembled.emplace_back(0xEA000000 | jmp_offset1 ); // b start

        const auto jmp_offset2
        { (assembled.size() - fill_idx - 2) & 0x00FFFFFF
        };
        assembled[fill_idx] = 0x0A000000 | jmp_offset2;
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