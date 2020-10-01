#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QPushButton>
#include <map>
#include <tictactoe_game.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    void GameUpdated(tictactoe::GameStatus status);

private slots:
    void on_cell1_clicked();
    void on_cell2_clicked();
    void on_cell3_clicked();
    void on_cell4_clicked();
    void on_cell5_clicked();
    void on_cell6_clicked();
    void on_cell7_clicked();
    void on_cell8_clicked();
    void on_cell9_clicked();
    void on_restartButton_clicked();
    void on_computerStart_clicked();

private:
    Ui::MainWindow* ui;
    tictactoe::GameUpdateCalback m_callback;
    tictactoe::TicTacToeGame m_game;
    std::map<uint8_t, QPushButton*> m_map;
};

#endif // MAIN_WINDOW_HPP
