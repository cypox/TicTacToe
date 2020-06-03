#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTreeWidgetItem>
#include <QTimer>
#include "minmax.h"

#define TURN_X 0
#define TURN_O 1
#define EASY 1      //Lookup to 1 move (best for cpu)
#define MEDIUM 2    //Lookup to 2 moves (worst for player)
#define HARD 3      //Lookup to 3 moves (best (worst for player) for cpu)
#define TYPE0 "In this mode winner is the first player to make one row of 4 same elements (Xs or Os)."
#define TYPE1 "In this mode winner is the player who have more 4-line Xs or Os once the grid is full."
#define PLAY 1
#define OVER 2

namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();
    void cpuTurn(int);
    void checkWin();
    void sumHorLines(int*, int*);
    void sumVerLines(int*, int*);
    void sumLDiags(int*, int*);
    void sumRDiags(int*, int*);
private slots:
    void play(int, int);
    void resize();
    void showHelp(int);
    void showDebug();

private:
    Ui::GameWindow *ui;
    unsigned char turn;
    unsigned char size;
    unsigned char **board;
    int filled;
    int gameStat;
    int type;
    int difficulty;
    MinMax *game_ai;
    bool debug;
};

#endif // GAMEWINDOW_H
