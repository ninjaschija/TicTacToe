#ifndef TICTACTOE_BOARD_HPP
#define TICTACTOE_BOARD_HPP

#include <array>
#include <cstdint>
#include <functional>

namespace tictactoe {

///
/// \brief The CellValue enum
///
enum class CellValue : char { None = 0, X = 'x', O = 'o' };

///
/// \brief board_size
///
constexpr uint8_t board_size = 3;

///
/// \brief The Cell struct
///
struct Cell {
    /// The attack score for the position.
    uint8_t attack_points{0};
    /// The defense score for the position
    uint8_t defense_points{0};
    /// Cell state (X, O, none)
    CellValue value{CellValue::None};
    /// Column
    uint8_t x{0};
    /// Row
    uint8_t y{0};
};

///
/// \brief The TicTacToeBoard class
///
class TicTacToeBoard final {
    using ForEachCallable = std::function<void(Cell&)>;

public:
    ///
    /// \brief TicTacToeBoard constructor
    ///
    TicTacToeBoard();

    /// Default destructor
    ~TicTacToeBoard() = default;

    ///
    /// \brief At Non-const getter for board position
    /// \param x
    /// \param y
    /// \return
    ///
    Cell& At(uint8_t x, uint8_t y);

    ///
    /// \brief At Const getter for board position
    /// \param x
    /// \param y
    /// \return
    ///
    const Cell& At(uint8_t x, uint8_t y) const;

    ///
    /// \brief MaxScoreCell Get the position with the highest score (attach + defense points)
    /// \return
    ///
    Cell& MaxScoreCell();

    ///
    /// \brief ForEachX Convenience cell iterator for a given column
    /// \param x
    /// \param pred
    /// \param include_empty
    ///
    void ForEachX(uint8_t x, ForEachCallable&& pred, bool include_empty = false);

    ///
    /// \brief ForEachY Convenience cell iterator for a given row
    /// \param y
    /// \param pred
    /// \param include_empty
    ///
    void ForEachY(uint8_t y, ForEachCallable&& pred, bool include_empty = false);

    ///
    /// \brief ForEachD1 Convenience cell iterator for the first diagonal
    /// \param pred
    /// \param include_empty
    ///
    void ForEachD1(ForEachCallable&& pred, bool include_empty = false);

    ///
    /// \brief ForEachD2 Convenience cell iterator for the second diagonal
    /// \param pred
    /// \param include_empty
    ///
    void ForEachD2(ForEachCallable&& pred, bool include_empty = false);

    ///
    /// \brief CountX Count positions (X or O) for a given row
    /// \param x
    /// \param val
    /// \return
    ///
    uint16_t CountX(uint8_t x, CellValue val);

    ///
    /// \brief CountY Count positions (X or O) for a given column
    /// \param y
    /// \param val
    /// \return
    ///
    uint16_t CountY(uint8_t y, CellValue val);

    ///
    /// \brief CountD1 Count positions (X or O) for the first diagonal
    /// \param val
    /// \return
    ///
    uint16_t CountD1(CellValue val);

    ///
    /// \brief CountD2 Count positions (X or O) for the second diagonal
    /// \param val
    /// \return
    ///
    uint16_t CountD2(CellValue val);

private:
    ///
    /// \brief InitCell
    /// \param x
    /// \param y
    /// \param attack_points
    ///
    void InitCell(uint8_t x, uint8_t y, uint8_t attack_points)
    {
        At(x, y).attack_points = attack_points;
        At(x, y).x = x;
        At(x, y).y = y;
    }

private:
    ///
    /// \brief m_board Storage for the board data
    ///
    std::array<Cell, board_size * board_size> m_board;
};

} // namespace tictactoe

#endif // TICTACTOE_BOARD_HPP
