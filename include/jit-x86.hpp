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
#ifndef JIT_X86_HPP
#define JIT_X86_HPP


#include "config.hpp"
#if defined(TARGET_ARCH) && TARGET_ARCH == ARCH_X86


// C++ includes
#include <vector>
// C includes
#include <cstdint>
// Local includes
#include "jit.hpp"


namespace bfjit
{
    struct jit_function::impl
    {
        std::vector<uint8_t> m_assembled;
        std::uint8_t *m_bytes;

        explicit impl(std::uint8_t *bytes);
        impl(impl&& src) noexcept;
    };
}


#endif


#endif
