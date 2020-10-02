# TicTacToe
Simple tic-tac-toe game made in QT.
The project is structurally separated into a core shared library and a QT Widget application.

## Library

### Computer player strategy

The computer chooses its next move based on the best position score across the board, where the position score is calculated between so called attack and defense points.
Each position has its own attack and defense scores stored separately, which get updated with each move.

By default, the attack points are initialized as follows:

    3 2 3
    2 4 2
    3 2 3

Each value represents all possible attack options starting from each cell e.g.:
- Corner values all have 3 attack directions (row, column, diagonal)
- Side values have 2 attack options (row, column)
- Center value has most attack opportunities (row, column, both diagonals)

The defense points are initially all 0, but with each opponent (human) move the defense points increase in order to reflect the opponent opportunities to attack or even to close the game.

Therefore, the computer's next move will be the position with the maximum value of (attack score + defense score). 
The priority is attack first (i.e. if there is an opportunity to close the game, do it), then defend against an imminent opponent's win.

### Difficulty

There are two difficulty levels (easy, hard), where the strategy for adapting the attack/defense points differs slightly. 
The easy mode is more conservative with increasing attack scores. Hard mode aims to be impossible to beat.
The difficulty can be changed from the checkbox from the user interface (easy is default)

## User interface application

The user interface is quite simple and it's developed in QT with QML:
- The game is automatically started (the first player is the human player by default)
- The game can be restarted at any time
- The computer can go first by restarting the game from the button in the right

