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
/// \brief The GamePolicy class
///
class GamePolicy {
protected:
    static constexpr uint8_t danger_zone = board_size - 1;

public:
    virtual ~GamePolicy() = default;

    ///
    /// \brief UpdateAttackPoints
    /// \param human_count
    /// \param computer_count
    /// \param cell
    ///
    virtual void UpdateAttackLinePoints(uint8_t enemy_count, uint8_t friendly_count,
                                        Cell& cell) = 0;

    ///
    /// \brief UpdateDefensePoints
    /// \param human_count
    /// \param cell
    ///
    virtual void UpdateDefenseLinePoints(uint8_t human_count, Cell& cell) = 0;
};

///
/// \brief The NormalGamePolicy class
///
class NormalGamePolicy : public GamePolicy {
public:
    void UpdateAttackLinePoints(uint8_t enemy_count, uint8_t friendly_count, Cell& cell) override
    {
        // Weaker attack
        if (friendly_count == danger_zone) {
            ++cell.attack_points;
        }
        // Some attack opportunities were lost because the enemy defended
        else if (enemy_count < danger_zone) {
            if (cell.attack_points > 0) {
                --cell.attack_points;
            }
        }
    }

    void UpdateDefenseLinePoints(uint8_t enemy_count, Cell& cell) override
    {
        // Weaker defense
        if (enemy_count > 0) {
            cell.defense_points = 1;
        }
    }
};

///
/// \brief The ImpossibleGamePolicy class
///
class ImpossibleGamePolicy : public GamePolicy {
public:
    void UpdateAttackLinePoints(uint8_t enemy_count, uint8_t friendly_count, Cell& cell) override
    {
        // We have an opportunity to close the game, make sure it's taken
        if (friendly_count == danger_zone) {
            cell.attack_points = 20;
        }
        // Some attack opportunities were lost because the enemy defended
        else if (enemy_count < danger_zone) {
            if (cell.attack_points > 0) {
                --cell.attack_points;
            }
        }
    }

    void UpdateDefenseLinePoints(uint8_t enemy_count, Cell& cell) override
    {
        // The higher the enemy count, the higher the defense
        if (enemy_count > 0) {
            cell.defense_points = enemy_count;
            if (enemy_count == danger_zone) {
                cell.defense_points = 10;
            }
        }
    }
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
    /// \brief Start
    /// \param human_side
    /// \param first_player
    /// \param callback
    /// \param easy_mode
    ///
    void Start(PlayerSide human_side, PlayerType first_player, const GameUpdateCalback& callback,
               bool easy_mode);

    ///
    /// \brief HumanMove
    /// \param x
    /// \param y
    ///
    void HumanMove(uint8_t x, uint8_t y);

    ///
    /// \brief GetCell
    /// \param x
    /// \param y
    /// \return
    ///
    const Cell& GetCell(uint8_t x, uint8_t y) const { return m_board.At(x, y); }

private:
    ///
    /// \brief ComputerMove
    /// \param first
    ///
    void ComputerMove(bool first);

    ///
    /// \brief UpdateAttackPoints
    /// \param x
    /// \param y
    ///
    void UpdateAttackPoints(uint8_t x, uint8_t y);

    ///
    /// \brief UpdateDefensePoints
    /// \param x
    /// \param y
    ///
    void UpdateDefensePoints(uint8_t x, uint8_t y);

    ///
    /// \brief UpdateCell
    /// \param cell
    ///
    void UpdateCell(Cell& cell, PlayerType player);

    ///
    /// \brief UpdateGame
    /// \param cell
    ///
    void UpdateGame(Cell& cell);

    ///
    /// \brief IsWinningMove
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
