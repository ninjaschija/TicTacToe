#include "tictactoe_board.hpp"
#include <algorithm>
#include <cassert>

namespace tictactoe {

TicTacToeBoard::TicTacToeBoard()
{
    InitCell(0, 0, 3);
    InitCell(0, 1, 2);
    InitCell(0, 2, 3);

    InitCell(1, 0, 2);
    InitCell(1, 1, 4);
    InitCell(1, 2, 2);

    InitCell(2, 0, 3);
    InitCell(2, 1, 2);
    InitCell(2, 2, 3);
}

Cell& TicTacToeBoard::At(uint8_t x, uint8_t y)
{
    assert(x < board_size && y < board_size);
    return m_board[y * board_size + x];
}

const Cell& TicTacToeBoard::At(uint8_t x, uint8_t y) const
{
    assert(x < board_size && y < board_size);
    return m_board[y * board_size + x];
}

Cell& TicTacToeBoard::MaxScoreCell()
{
    auto first_empty = std::find_if(m_board.begin(), m_board.end(),
                                    [](const auto& c) { return c.value == CellValue::None; });
    assert(first_empty != m_board.end());

    return *std::max_element(first_empty, m_board.end(), [](const auto& c1, const auto& c2) {
        return c1.attack_points + c1.defense_points < c2.attack_points + c2.defense_points;
    });
}

void TicTacToeBoard::ForEachX(uint8_t x, const ForEachCallable& pred, bool include_empty)
{
    for (uint8_t i = 0; i < board_size; ++i) {
        auto& cell = At(x, i);
        if (include_empty || cell.value == CellValue::None) {
            pred(cell);
        }
    }
}

void TicTacToeBoard::ForEachY(uint8_t y, const ForEachCallable& pred, bool include_empty)
{
    for (uint8_t i = 0; i < board_size; ++i) {
        auto& cell = At(i, y);
        if (include_empty || cell.value == CellValue::None) {
            pred(cell);
        }
    }
}

void TicTacToeBoard::ForEachD1(const ForEachCallable& pred, bool include_empty)
{
    for (uint8_t i = 0; i < board_size; ++i) {
        auto& cell = At(i, i);
        if (include_empty || cell.value == CellValue::None) {
            pred(cell);
        }
    }
}

void TicTacToeBoard::ForEachD2(const ForEachCallable& pred, bool include_empty)
{
    for (uint8_t i = 0; i < board_size; ++i) {
        auto& cell = At(i, board_size - i - 1);
        if (include_empty || cell.value == CellValue::None) {
            pred(cell);
        }
    }
}

uint16_t TicTacToeBoard::CountX(uint8_t x, CellValue val) const
{
    uint16_t sum = 0;
    for (uint8_t i = 0; i < board_size; ++i) {
        const Cell& cell = At(x, i);
        if (cell.value == val) {
            ++sum;
        }
    }
    return sum;
}

uint16_t TicTacToeBoard::CountY(uint8_t y, CellValue val) const
{
    uint16_t sum = 0;
    for (uint8_t i = 0; i < board_size; ++i) {
        const Cell& cell = At(i, y);
        if (cell.value == val) {
            ++sum;
        }
    }
    return sum;
}

uint16_t TicTacToeBoard::CountD1(CellValue val) const
{
    uint16_t sum = 0;
    for (uint8_t i = 0; i < board_size; ++i) {
        const Cell& cell = At(i, i);
        if (cell.value == val) {
            ++sum;
        }
    }
    return sum;
}

uint16_t TicTacToeBoard::CountD2(CellValue val) const
{
    uint16_t sum = 0;
    for (uint8_t i = 0; i < board_size; ++i) {
        const Cell& cell = At(i, board_size - i - 1);
        if (cell.value == val) {
            ++sum;
        }
    }
    return sum;
}

} // namespace tictactoe
