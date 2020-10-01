#include "main_window.hpp"
#include <QMessageBox>
#include "ui_main_window.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_callback{[this](tictactoe::GameStatus status) { GameUpdated(status); }}
{
    ui->setupUi(this);

    m_map[0] = ui->cell1;
    m_map[1] = ui->cell2;
    m_map[2] = ui->cell3;
    m_map[3] = ui->cell4;
    m_map[4] = ui->cell5;
    m_map[5] = ui->cell6;
    m_map[6] = ui->cell7;
    m_map[7] = ui->cell8;
    m_map[8] = ui->cell9;

    m_game.Start(tictactoe::PlayerSide::xs, tictactoe::PlayerType::human, m_callback,
                 ui->easyCheckBox->isChecked());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::GameUpdated(tictactoe::GameStatus status)
{
    for (uint8_t x = 0; x < tictactoe::board_size; ++x) {
        for (uint8_t y = 0; y < tictactoe::board_size; ++y) {
            auto& cell = m_game.GetCell(x, y);
            uint8_t id = y * tictactoe::board_size + x;
            auto button = m_map[id];

            switch (cell.value) {
            case tictactoe::CellValue::O:
                button->setText("O");
                button->setEnabled(false);
                button->setDown(true);
                break;
            case tictactoe::CellValue::X:
                button->setText("X");
                button->setEnabled(false);
                button->setDown(true);
                break;
            default:
                //button->setText(QString("%1 %2").arg(cell.attack_points).arg(cell.defense_points));
                button->setText("");
                button->setEnabled(status == tictactoe::GameStatus::in_progress);
                button->setDown(status != tictactoe::GameStatus::in_progress);
                break;
            }
            // Mark winning line
            if (cell.attack_points == tictactoe::TicTacToeGame::npos) {
                button->setStyleSheet("QPushButton{color:green;}");
            }
            else {
                button->setStyleSheet("QPushButton{color:black;}");
            }
        }
    }

    QString str;

    switch (status) {
    case tictactoe::GameStatus::in_progress:
        str = "Game Started";
        break;
    case tictactoe::GameStatus::draw:
        str = "Game Draw";
        break;
    case tictactoe::GameStatus::os_winner:
        str = "O is the winner";
        break;
    case tictactoe::GameStatus::xs_winner:
        str = "X is the winner";
        break;
    default:
        break;
    }

    ui->gameStatusLabel->setText(str);
}

void MainWindow::on_cell1_clicked()
{
    m_game.HumanMove(0, 0);
}

void MainWindow::on_cell2_clicked()
{
    m_game.HumanMove(1, 0);
}

void MainWindow::on_cell3_clicked()
{
    m_game.HumanMove(2, 0);
}

void MainWindow::on_cell4_clicked()
{
    m_game.HumanMove(0, 1);
}

void MainWindow::on_cell5_clicked()
{
    m_game.HumanMove(1, 1);
}

void MainWindow::on_cell6_clicked()
{
    m_game.HumanMove(2, 1);
}

void MainWindow::on_cell7_clicked()
{
    m_game.HumanMove(0, 2);
}

void MainWindow::on_cell8_clicked()
{
    m_game.HumanMove(1, 2);
}

void MainWindow::on_cell9_clicked()
{
    m_game.HumanMove(2, 2);
}

void MainWindow::on_restartButton_clicked()
{
    m_game.Start(tictactoe::PlayerSide::xs, tictactoe::PlayerType::human, m_callback,
                 ui->easyCheckBox->isChecked());
}

void MainWindow::on_computerStart_clicked()
{
    m_game.Start(tictactoe::PlayerSide::os, tictactoe::PlayerType::computer, m_callback,
                 ui->easyCheckBox->isChecked());
}
