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
#ifndef AST_HPP
#define AST_HPP

// C++ includes
#include <iosfwd>
#include <vector>
// C includes
#include <cstdint>
// External includes
#include <mapbox/variant.hpp>
// Local includes
#include "position.hpp"


namespace bfjit
{
    class movement_ast;
    class mutation_ast;
    class io_ast;
    class loop_ast;

    /// \brief A typedef that unites all ast types into a single variant.
    using combined_ast = mapbox::util::variant
        < movement_ast
        , mutation_ast
        , io_ast
        , mapbox::util::recursive_wrapper<loop_ast>>;


    /// \brief Abstract base class of all other ast classes.
    class ast
    {
    public:
        // Destructor
        /// \brief Destroys the object.
        virtual ~ast() noexcept = default;

        // Accessors
        /// Returns the position of this instruction in the stream.
        const position& pos() const noexcept;


    protected:
        // Constructors
        /// Constructs a new object, given its position.
        explicit ast(const position& pos) noexcept;
        /// \brief Constructs a new object, given an existing object.
        ast(const ast& src) noexcept = default;
        /// \brief Constructs a new object, given an existing temporary object.
        ast(ast&& src) noexcept = default;

        /// \brief The position of ast in the stream.
        position m_pos;
    };


    /// \brief A class that represents a movement instruction.
    ///
    /// Like other \ref bfjit classes, objects of this class are **immutable**.
    class movement_ast final
        : public ast
    {
    public:
        // Associated types
        /// \brief An enum that contains all possible types of movement
        /// instructions.
        enum class type : std::uint_fast8_t
        { LEFT     ///< Represents a *go left* instruction.
        , RIGHT    ///< Represents a *go right* instruction.
        , __LAST__ ///< Number of enum entries. **INVALID VALUE**
        };

        // Constructors
        /// \brief Constructs a new object, given its position, type and
        /// the number of moves to perform.
        movement_ast
        ( const position& pos
        , const type& ty
        , const size_t& by
        ) noexcept;
        /// \brief Constructs a new object, given an existing object.
        movement_ast(const movement_ast& src) noexcept = default;
        /// \brief Constructs a new object, given an existing temporary object.
        movement_ast(movement_ast&& src) noexcept = default;

        // Destructor
        /// \brief Destroys the object.
        ~movement_ast() noexcept override = default;

        // Accessors
        /// Returns the type of this movement instruction.
        const type& ty() const noexcept;
        /// Returns the amount which the pointer needs to move *by*.
        const size_t& by() const noexcept;


    private:
        type   m_type;
        size_t m_by;
    };


    /// A function that pretty-prints a \ref movement_ast::type object.
    std::ostream& operator<<(std::ostream& out, const movement_ast::type& ty);
    /// A function that pretty-prints a \ref token object.
    std::ostream& operator<<(std::ostream& out, const movement_ast& mast);


    /// \brief A class that represents a single (cellmutation) instruction.
    ///
    /// Like other \ref bfjit classes, objects of this class are **immutable**.
    class mutation_ast final
        : public ast
    {
    public:
        // Associated types
        /// \brief An enum that contains all possible types of mutation
        /// instructions.
        enum class type : std::uint_fast8_t
        { INC      ///< Represents a *increase value* instruction.
        , DEC      ///< Represents a *decrease value* instruction.
        , __LAST__ ///< Number of enum entries. **INVALID VALUE**
        };

        // Constructors
        /// \brief Constructs a new object, given its position, type and
        /// the number of mutations to perform.
        mutation_ast
        ( const position& pos
        , const type& ty
        , const size_t& by
        ) noexcept;
        /// \brief Constructs a new object, given an existing object.
        mutation_ast(const mutation_ast& src) noexcept = default;
        /// \brief Constructs a new object, given an existing temporary object.
        mutation_ast(mutation_ast&& src) noexcept = default;

        // Destructor
        /// \brief Destroys the object.
        ~mutation_ast() noexcept override = default;

        // Accessors
        /// Returns the type of this mutation instruction.
        const type& ty() const noexcept;
        /// Returns the amount which the cell needs to change *by*.
        const size_t& by() const noexcept;


    private:
        type   m_type;
        size_t m_by;
    };


    /// A function that pretty-prints a \ref mutation_ast::type object.
    std::ostream& operator<<(std::ostream& out, const mutation_ast::type& ty);
    /// A function that pretty-prints a \ref mutation_ast object.
    std::ostream& operator<<(std::ostream& out, const mutation_ast& mast);


    /// \brief A class that represent a single I/O instruction.
    ///
    /// Like other \ref bfjit classes, objects of this class are **immutable**.
    class io_ast final
        : public ast
    {
    public:
        // Associated types
        /// \brief An enum that contains all possible types of I/O
        /// instructions.
        enum class type : std::uint_fast8_t
        { IN       ///< Represents an *input number* instruction.
        , OUT      ///< Represents an *output number* instruction.
        , __LAST__ ///< Number of enum entries. **INVALID VALUE**
        };

        // Constructors
        /// \brief Constructs a new object, given its position and type.
        io_ast(const position& pos, const type& ty) noexcept;
        /// \brief Constructs a new object, given an existing object.
        io_ast(const io_ast& src) noexcept = default;
        /// \brief Constructs a new object, given an existing temporary object.
        io_ast(io_ast&& src) noexcept = default;

        // Destructors
        /// \brief Destroys the object.
        ~io_ast() noexcept override = default;

        // Accessors
        /// Returns the type of this I/O instruction.
        const type& ty() const noexcept;


    private:
        type m_type;
    };


    /// A function that pretty-prints a \ref io_ast::type object.
    std::ostream& operator<<(std::ostream& out, const io_ast::type& ty);
    /// A function that pretty-prints a \ref io_ast object.
    std::ostream& operator<<(std::ostream& out, const io_ast& ioast);


    /// \brief A class that represents a brainfuck loop.
    ///
    /// Like other \ref bfjit classes, objects of this class are **immutable**.
    class loop_ast final
        : public ast
    {
    public:


        // Constructors
        /// \brief Constructs a new object, given its position and body.
        loop_ast(const position& pos, const std::vector<combined_ast>& inner);
        /// \brief Constructs a new object, given an existing object.
        loop_ast(const loop_ast& src) = default;
        /// \brief Constructs a new object, given an existing temporary object.
        loop_ast(loop_ast&& src) noexcept = default;

        // Destructor
        /// \brief Destroys the object.
        ~loop_ast() noexcept override = default;

        // Accessors
        /// \brief Returns the instructions containted within the loop.
        const std::vector<combined_ast>& inner() const noexcept;


    private:
        std::vector<combined_ast> m_inner;
    };


    /// A function that pretty-prints a \ref loop_ast object.
    std::ostream& operator<<(std::ostream& out, const loop_ast& ioast);
}


#endif
