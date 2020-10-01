/// @file
///
/// @author
///
/// @copyright

#include "tictactoe_game.hpp"
#include <cassert>
#include <stdexcept>

namespace tictactoe {

TicTacToeGame::TicTacToeGame()
    : m_human_side{PlayerSide::os}
    , m_current_player{PlayerType::human}
    , m_game_status{GameStatus::not_started}
    , m_moves{0u}
{
}

void TicTacToeGame::Start(PlayerSide human_side, PlayerType first_player,
                          const GameUpdateCalback& callback, bool easy_mode)
{
    if (easy_mode) {
        m_policy = std::make_unique<NormalGamePolicy>();
    }
    else {
        m_policy = std::make_unique<ImpossibleGamePolicy>();
    }
    m_board = TicTacToeBoard{};
    m_human_side = human_side;
    m_current_player = first_player;
    m_callback = callback;
    m_game_status = GameStatus::in_progress;
    m_moves = 0;

    if (m_current_player == PlayerType::computer) {
        ComputerMove(true);
    }
    else if (m_callback) {
        m_callback(m_game_status);
    }
}

void TicTacToeGame::HumanMove(uint8_t x, uint8_t y)
{
    assert(m_current_player == PlayerType::human);
    assert(m_board.At(x, y).value == CellValue::None);

    auto& cell = m_board.At(x, y);
    // Mark the cell
    UpdateCell(cell, m_current_player);
    // Re-calculate the attack scores
    UpdateAttackPoints(x, y);
    // Re-calculate the defense scores
    UpdateDefensePoints(x, y);
    // Update game status
    UpdateGame(cell);
    // Computer's turn
    if (m_game_status == GameStatus::in_progress) {
        ComputerMove(false);
    }
}

void TicTacToeGame::ComputerMove(bool first)
{
    assert(m_current_player == PlayerType::computer);

    // First computer move, just start at 0,0
    if (first) {
        auto& cell = m_board.At(0, 0);
        UpdateCell(cell, m_current_player);
        UpdateGame(cell);
        return;
    }

    // Pick the one with the highest score
    auto& cell = m_board.MaxScoreCell();
    // Mark the cell
    UpdateCell(cell, m_current_player);
    // Make another pass to see if our last move opened a win opportunity
    UpdateAttackPoints(cell.x, cell.y);
    // Update game status
    UpdateGame(cell);
}

void TicTacToeGame::UpdateAttackPoints(uint8_t x, uint8_t y)
{
    CellValue human_pieces = (m_human_side == PlayerSide::os) ? CellValue::O : CellValue::X;
    CellValue computer_pieces = (m_human_side == PlayerSide::os) ? CellValue::X : CellValue::O;

    // Count number of human pieces on the column
    auto human_count_x = m_board.CountX(x, human_pieces);
    // Count computer's pieces on the same column
    auto computer_count_x = m_board.CountX(x, computer_pieces);

    // Update the attack points for the entire column
    m_board.ForEachX(x, [this, human_count_x, computer_count_x](Cell& cell) {
        m_policy->UpdateAttackLinePoints(human_count_x, computer_count_x, cell);
    });

    // Count number of human pieces on the row
    auto human_count_y = m_board.CountY(y, human_pieces);
    // Count computer's pieces on the same row
    auto computer_count_y = m_board.CountY(y, computer_pieces);

    // Update the attack points for the entire row
    m_board.ForEachY(y, [this, human_count_y, computer_count_y](Cell& cell) {
        m_policy->UpdateAttackLinePoints(human_count_y, computer_count_y, cell);
    });

    // Is the position on the first diagonal?
    if (x == y) {
        auto human_count_d1 = m_board.CountD1(human_pieces);
        auto computer_count_d1 = m_board.CountD1(computer_pieces);
        // Update the attack points for the first diagonal
        m_board.ForEachD1([this, human_count_d1, computer_count_d1](Cell& cell) {
            m_policy->UpdateAttackLinePoints(human_count_d1, computer_count_d1, cell);
        });
    }
    // Or is it on the second diagonal?
    else if (x == board_size - y - 1) {
        auto human_count_d2 = m_board.CountD2(human_pieces);
        auto computer_count_d2 = m_board.CountD2(computer_pieces);
        // Update the attack points for the second diagonal
        m_board.ForEachD2([this, human_count_d2, computer_count_d2](Cell& cell) {
            m_policy->UpdateAttackLinePoints(human_count_d2, computer_count_d2, cell);
        });
    }
}

void TicTacToeGame::UpdateDefensePoints(uint8_t x, uint8_t y)
{
    CellValue human_pieces = (m_human_side == PlayerSide::os) ? CellValue::O : CellValue::X;

    // Count number of human pieces on the column
    auto human_count_x = m_board.CountX(x, human_pieces);

    // Update the defense poins for the entire column
    m_board.ForEachX(x, [this, human_count_x](Cell& cell) {
        m_policy->UpdateDefenseLinePoints(human_count_x, cell);
    });

    // Count number of human pieces on the row
    auto human_count_y = m_board.CountY(y, human_pieces);

    // Update the defense poins for the entire row
    m_board.ForEachY(y, [this, human_count_y](Cell& cell) {
        m_policy->UpdateDefenseLinePoints(human_count_y, cell);
    });

    // Is the position on the first diagonal?
    if (x == y) {
        auto human_count_d1 = m_board.CountD1(human_pieces);
        // Update the defense points for the first diagonal
        m_board.ForEachD1([this, human_count_d1](Cell& cell) {
            m_policy->UpdateDefenseLinePoints(human_count_d1, cell);
        });
    }
    // Or is it on the second diagonal?
    else if (x == board_size - y - 1) {
        auto human_count_d2 = m_board.CountD2(human_pieces);
        // Update the defense points for the second diagonal
        m_board.ForEachD2([this, human_count_d2](Cell& cell) {
            m_policy->UpdateDefenseLinePoints(human_count_d2, cell);
        });
    }
}

void TicTacToeGame::UpdateCell(Cell& cell, PlayerType player_type)
{
    CellValue human_pieces = (m_human_side == PlayerSide::os) ? CellValue::O : CellValue::X;
    CellValue computer_pieces = (m_human_side == PlayerSide::os) ? CellValue::X : CellValue::O;

    cell.value = player_type == PlayerType::human ? human_pieces : computer_pieces;
    cell.attack_points = 0;
    cell.defense_points = 0;
}

void TicTacToeGame::UpdateGame(Cell& cell)
{
    PlayerSide player;

    switch (m_current_player) {
    case PlayerType::human:
        player = m_human_side;
        m_current_player = PlayerType::computer;
        break;
    case PlayerType::computer:
        player = (m_human_side == PlayerSide::os) ? PlayerSide::xs : PlayerSide::os;
        m_current_player = PlayerType::human;
        break;
    default:
        assert(false);
    }

    ++m_moves;

    // All moves consumed, no winner
    if (m_moves == board_size * board_size) {
        m_game_status = GameStatus::draw;
    }
    // Do we have a winner with the last move?
    else if (IsWinningMove(cell)) {
        m_game_status = (player == PlayerSide::os) ? GameStatus::os_winner : GameStatus::xs_winner;
    }

    if (m_callback) {
        m_callback(m_game_status);
    }
}

bool TicTacToeGame::IsWinningMove(Cell& cell)
{
    assert(cell.value != CellValue::None);

    auto mark_winner_cell = [](Cell& cell) { cell.attack_points = npos; };

    // Check the column
    if (board_size == m_board.CountX(cell.x, cell.value)) {
        m_board.ForEachX(cell.x, mark_winner_cell, true);
        return true;
    }
    // Check the row
    if (board_size == m_board.CountY(cell.y, cell.value)) {
        m_board.ForEachY(cell.y, mark_winner_cell, true);
        return true;
    }
    // Check both diagonals
    if (cell.x == cell.y) {
        if (board_size == m_board.CountD1(cell.value)) {
            m_board.ForEachD1(mark_winner_cell, true);
            return true;
        }
    }
    if (cell.x + cell.y == board_size - 1) {
        if (board_size == m_board.CountD2(cell.value)) {
            m_board.ForEachD2(mark_winner_cell, true);
            return true;
        }
    }
    return false;
}

} // namespace tictactoe
