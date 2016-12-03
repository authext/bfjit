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
#ifndef JIT_HPP
#define JIT_HPP


// C++ includes
#include <memory>
#include <vector>
// Local includes
#include "ast.hpp"
#include "config.hpp"


#if !defined(TARGET_ARCH)
#error "TARGET_ARCH must be defined!"
#elif \
    TARGET_ARCH != ARCH_X86 &&\
    TARGET_ARCH != ARCH_X86_64 &&\
    TARGET_ARCH != ARCH_ARM
#error "TARGET_ARCH must be set to a supported architecture!"
#endif

#if !defined(TARGET_OS)
#error "TARGET_OS must be defined!"
#elif \
    TARGET_OS != OS_WINDOWS &&\
    TARGET_OS != OS_UNIX
#error "TARGET_OS must be set to a supported OS"
#endif


namespace bfjit
{
    /// \brief A class that represents code ready for execution.
    class jit_function
    {
    public:
        /// Associated types
        struct impl;

        // Constructors
        /// \brief \ref jit_function cannot be copied.
        jit_function(const jit_function& src) = delete;
        /// \brief Constructs a new object, given an existing temporary object.
        jit_function(jit_function&& src) = default;

        // Destructor
        /// \brief Destroys the object.
        ~jit_function();

        // Operators
        /// \brief Actually runs the generated code.
        void operator()() const;

        // Static fnctions
        /// \brief *The* function for generating a \ref jit_function.
        /// \param begin_it An iterator to the beginning of the code to jit.
        /// \param end_it An iterator to the end of the code to jit.
        /// \param bytes Pointer to the cell heap.
        static jit_function make
        ( std::vector<combined_ast>::const_iterator begin_it
        , std::vector<combined_ast>::const_iterator end_it
        , std::uint8_t *bytes
        );


    private:
        std::unique_ptr<impl> m_impl;

        // Constructors
        jit_function(std::unique_ptr<impl>&& src);
    };


    void prologue(jit_function::impl* impl);
    void epilogue(jit_function::impl* impl);
    void movement(jit_function::impl* impl, const movement_ast& m_ast);
    void mutation(jit_function::impl* impl, const mutation_ast& m_ast);
    void io(jit_function::impl* impl, const io_ast& i_ast);
    void loop(jit_function::impl* impl, const loop_ast& l_ast);
}


#endif
