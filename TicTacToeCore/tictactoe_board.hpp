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
    ///
    uint8_t attack_points{0};
    ///
    uint8_t defense_points{0};
    ///
    CellValue value{CellValue::None};
    ///
    uint8_t x{0};
    ///
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
    /// \brief At
    /// \param x
    /// \param y
    /// \return
    ///
    Cell& At(uint8_t x, uint8_t y);

    ///
    /// \brief At
    /// \param x
    /// \param y
    /// \return
    ///
    const Cell& At(uint8_t x, uint8_t y) const;

    ///
    /// \brief MaxScoreCell
    /// \return
    ///
    Cell& MaxScoreCell();

    ///
    /// \brief ForEachX
    /// \param x
    /// \param pred
    /// \param include_empty
    ///
    void ForEachX(uint8_t x, ForEachCallable&& pred, bool include_empty = false);

    ///
    /// \brief ForEachY
    /// \param y
    /// \param pred
    /// \param include_empty
    ///
    void ForEachY(uint8_t y, ForEachCallable&& pred, bool include_empty = false);

    ///
    /// \brief ForEachD1
    /// \param pred
    /// \param include_empty
    ///
    void ForEachD1(ForEachCallable&& pred, bool include_empty = false);

    ///
    /// \brief ForEachD2
    /// \param pred
    /// \param include_empty
    ///
    void ForEachD2(ForEachCallable&& pred, bool include_empty = false);

    ///
    /// \brief CountX
    /// \param x
    /// \param val
    /// \return
    ///
    uint16_t CountX(uint8_t x, CellValue val);

    ///
    /// \brief CountY
    /// \param y
    /// \param val
    /// \return
    ///
    uint16_t CountY(uint8_t y, CellValue val);

    ///
    /// \brief CountD1
    /// \param val
    /// \return
    ///
    uint16_t CountD1(CellValue val);

    ///
    /// \brief CountD2
    /// \param val
    /// \return
    ///
    uint16_t CountD2(CellValue val);

private:
    void InitCell(uint8_t x, uint8_t y, uint8_t attack_points)
    {
        At(x, y).attack_points = attack_points;
        At(x, y).x = x;
        At(x, y).y = y;
    }

private:
    std::array<Cell, board_size * board_size> m_board;
};

} // namespace tictactoe

#endif // TICTACTOE_BOARD_HPP
