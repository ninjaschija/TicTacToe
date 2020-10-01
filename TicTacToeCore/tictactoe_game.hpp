/// @file
///
/// @author
///
/// @copyright

#ifndef TICTACTOE_GAME_HPP
#define TICTACTOE_GAME_HPP

#include <functional>
#include <memory>
#include "tictactoecore_global.hpp"
#include <tictactoe_board.hpp>

namespace tictactoe {

///
/// \brief Defines the attack and defense policies for different difficulty levels
///
class GamePolicy {
protected:
    static constexpr uint8_t danger_zone = board_size - 1;

public:
    virtual ~GamePolicy() = default;

    ///
    /// \brief UpdateAttackPoints Update the attack points for a cell
    /// \param enemy_count Number of enemy positions on the same line
    /// \param friendly_count Number of friendly positions on the same line
    /// \param cell
    ///
    virtual void UpdateAttackLinePoints(uint8_t enemy_count, uint8_t friendly_count,
                                        Cell& cell) = 0;

    ///
    /// \brief UpdateDefensePoints Update the defense points for a cell
    /// \param enemy_count Number of enemy positions on the same line
    /// \param cell
    ///
    virtual void UpdateDefenseLinePoints(uint8_t enemy_count, Cell& cell) = 0;
};

///
/// \brief The GameStatus enum
///
enum class GameStatus {
    not_started,
    in_progress,
    draw,
    xs_winner,
    os_winner,
};

///
/// \brief The PlayerSide enum
///
enum class PlayerSide { xs, os };

///
/// \brief The PlayerType enum
///
enum class PlayerType { computer, human };

///
/// \brief Callback used to notify the game status update to the UI
///
using GameUpdateCalback = std::function<void(GameStatus)>;

///
/// \brief The TicTacToeGame class
///
class TICTACTOECORESHARED_EXPORT TicTacToeGame final {
public:
    ///
    /// \brief npos
    ///
    static constexpr uint8_t npos = std::numeric_limits<uint8_t>::max();

    ///
    /// \brief TicTacToeGame constructor
    /// \param human_side
    /// \param first_player
    /// \param callback
    ///
    TicTacToeGame();

    /// Default destructor
    ~TicTacToeGame() = default;

    ///
    /// \brief Start Starts a new game
    /// \param human_side X or O for the human?
    /// \param first_player Who goes first
    /// \param callback Game status update notification
    /// \param easy_mode Easy or difficult level
    ///
    void Start(PlayerSide human_side, PlayerType first_player, const GameUpdateCalback& callback,
               bool easy_mode);

    ///
    /// \brief HumanMove Add position for human player
    /// \param x
    /// \param y
    ///
    void HumanMove(uint8_t x, uint8_t y);

    ///
    /// \brief GetCell Getter for the board cells
    /// \param x
    /// \param y
    /// \return
    ///
    const Cell& GetCell(uint8_t x, uint8_t y) const { return m_board.At(x, y); }

private:
    ///
    /// \brief ComputerMove Perform computer move
    /// \param first First move of the game for the computer
    ///
    void ComputerMove(bool first);

    ///
    /// \brief UpdateAttackPoints Update attack scores starting from x, y
    /// \param x
    /// \param y
    ///
    void UpdateAttackPoints(uint8_t x, uint8_t y);

    ///
    /// \brief UpdateDefensePoints Update defense scores starting from x, y
    /// \param x
    /// \param y
    ///
    void UpdateDefensePoints(uint8_t x, uint8_t y);

    ///
    /// \brief UpdateCell Update cell data after a player move
    /// \param cell
    ///
    void UpdateCell(Cell& cell, PlayerType player);

    ///
    /// \brief UpdateGame Update game data and status and notify
    /// \param cell
    ///
    void UpdateGame(Cell& cell);

    ///
    /// \brief IsWinningMove Check if last move is a game winner
    /// \param cell
    /// \return
    ///
    bool IsWinningMove(Cell& cell);

private:
    std::unique_ptr<GamePolicy> m_policy;
    TicTacToeBoard m_board;
    PlayerSide m_human_side;
    PlayerType m_current_player;
    GameUpdateCalback m_callback;
    GameStatus m_game_status;
    size_t m_moves;
};

} // namespace tictactoe

#endif // TICTACTOE_GAME_HPP
