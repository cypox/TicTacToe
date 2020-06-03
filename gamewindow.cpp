#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    debug = false;
    size = 5;
    difficulty = MEDIUM;
    type = 0;
    filled = 0;
    ui->treeWidget->setColumnWidth(0, 250);
    ui->treeWidget->setColumnWidth(1, 75);
    game_ai = new MinMax(difficulty, size, type, ui->treeWidget);
    int i = size;
    board = (unsigned char**)malloc(i * sizeof(unsigned char*));
    for (; i-- ; )
        board[i] = (unsigned char*)malloc(i * sizeof(unsigned char));
    for ( int i = size ; i--; )
        for ( int j = size ; j--; )
            board[i][j] = 0;
    ui->label_4->setText(TYPE0);
    gameStat = PLAY;
    type = 0;
    ui->statusBar->showMessage("Waiting for player to play ...");
    ui->label_5->setText("Game is running");
    turn = TURN_X;
}

GameWindow::~GameWindow()
{
    free(board);
    delete ui;
}

void GameWindow::showDebug()
{
    if ( !debug )
    {
        this->setGeometry(this->geometry().x(), this->geometry().y(), 1300, 670);
        debug = true;
    }
    else
    {
        this->setGeometry(this->geometry().x(), this->geometry().y(), 850, 670);
        debug = false;
    }
}

void GameWindow::showHelp(int t)
{
    ui->label_4->setText((t==0?TYPE0:TYPE1));
}

void GameWindow::resize()
{
    ui->statusBar->showMessage("Creating new board ...");
    int i = ui->spinBox->value(), c = 600/i, j = i;
    size = i;
    ui->tableWidget->setColumnCount(i);
    ui->tableWidget->setRowCount(i);
    for (; i-- ; )
    {
        ui->tableWidget->setColumnWidth(i, c);
        ui->tableWidget->setRowHeight(i, c);
    }
    ui->tableWidget->clear();
    i = j;
    free(board);
    board = (unsigned char**)malloc(i * sizeof(unsigned char*));
    for (; i-- ; )
        board[i] = (unsigned char*)malloc(i * sizeof(unsigned char));
    for ( i = size ; i--; )
        for ( j = size ; j--; )
            board[i][j] = 0;
    delete game_ai;
    game_ai = new MinMax(ui->comboBox->currentIndex()+1, size, ui->comboBox_2->currentIndex(), ui->treeWidget);
    filled = 0;
    gameStat = PLAY;
    type = ui->comboBox_2->currentIndex();
    ui->statusBar->showMessage("Waiting for player to play ...");
    ui->label_5->setText("Game is running");
    ui->tableWidget->setEnabled(true);
    turn = TURN_X;
}

void GameWindow::cpuTurn(int last)
{
    if ( gameStat == PLAY )
    {
        ui->treeWidget->clear();
        int move = game_ai->think(new Config(board, size, last), ui->treeWidget, debug);
        int x = move/size;
        int y = move%size;
        play(x, y);
    }
}

void GameWindow::play(int x, int y)
{
    if ( gameStat == OVER )
    {
        ui->statusBar->showMessage("Game is over. Please restart a new game.");
        return;
    }
    if ( turn == TURN_X )
    {
        if ( board[x][y] != 1 && board[x][y] != 2 )
        {
            QTableWidgetItem *x_item = new QTableWidgetItem("X");
            x_item->setIcon(QIcon(":/x.png"));
            x_item->setTextAlignment(Qt::AlignCenter);
            //x_item->setBackgroundColor(QColor(255, 0, 0, 100));
            x_item->setTextColor(QColor("red"));
            x_item->setFont(QFont("Verdana", 30, 30));
            ui->tableWidget->setItem(x, y, x_item);
            board[x][y] = 1;
            filled ++;
            ui->tableWidget->setEnabled(false);
            ui->statusBar->showMessage("Waiting for computer to play ...");
            turn = TURN_O;
            checkWin();
            if ( gameStat == PLAY )
                cpuTurn(x*size+y);
        }
        else
            ui->statusBar->showMessage("Impossible move. Waiting for player to play again ...");
    }
    else
    {
        QTableWidgetItem *o_item = new QTableWidgetItem("O");
        o_item->setIcon(QIcon(":/o.png"));
        o_item->setTextAlignment(Qt::AlignCenter);
        //o_item->setBackgroundColor(QColor(0, 120, 0, 100));
        o_item->setTextColor(QColor("green"));
        o_item->setFont(QFont("Verdana", 30, 30));
        ui->tableWidget->setItem(x, y, o_item);
        board[x][y] = 2;
        filled ++;
        ui->statusBar->showMessage("Waiting for player to play ...");
        turn = TURN_X;
        ui->tableWidget->setEnabled(true);
        checkWin();
    }
}

void GameWindow::sumHorLines(int* X, int* O)
{
    int i, j, k, x = 0, o = 0;
    for ( i = 0 ; i < size ; i ++ )
    {
        for ( j = 0 ; j <= size - 4 ; j ++ )
        {
            x = 0; o = 0;
            for ( k = 0 ; k < 4 ; k ++ )
            {
                if ( board[i][j+k] == 1 )
                    x ++;
                else if ( board[i][j+k] == 2 )
                    o ++;
            }
            if ( x == 4 )
                (*X) ++;
            else if ( o == 4 )
                (*O) ++;
        }
    }
}

void GameWindow::sumVerLines(int* X, int* O)
{
    int i, j, k, x = 0, o = 0;
    for ( i = 0 ; i < size ; i ++ )
    {
        for ( j = 0 ; j <= size - 4 ; j ++ )
        {
            x = 0; o = 0;
            for ( k = 0 ; k < 4 ; k ++ )
            {
                if ( board[j+k][i] == 1 )
                    x ++;
                else if ( board[j+k][i] == 2 )
                    o ++;
            }
            if ( x == 4 )
                (*X) ++;
            else if ( o == 4 )
                (*O) ++;
        }
    }
}

void GameWindow::sumLDiags(int* X, int* O)
{
    int i, j, k, x, o, s = size;
    unsigned char**b = board;
    for ( i = s - 4 ; i >= 0 ; i -- )
    {
        for ( j = 0 ; j <= s-4-i ; j ++ )
        {
            x = o = 0;
            for ( k = j ; k < j+4 ; k ++ )
            {
                if ( b[i+k][k] == 1 )
                    x ++;
                else if ( b[i+k][k] == 2 )
                    o ++;
            }
            if ( x == 4 )
                (*X)++;
            else if ( o == 4 )
                (*O)++;
        }
    }

    for ( i = 1 ; i <= s-4 ; i ++ )
    {
        for ( j = 0 ; j <= s-4-i ; j ++ )
        {
            x = o = 0;
            for ( k = i+j ; k < i+j+4 ; k ++ )
            {
                if ( b[k-j-1][k] == 1 )
                    x ++;
                else if ( b[k-j-1][k] == 2 )
                    o ++;
            }
            if ( x == 4 )
                (*X)++;
            else if ( o == 4 )
                (*O)++;
        }
    }
}

void GameWindow::sumRDiags(int* X, int* O)
{
    int i, j, k, x, o, s = size, l;
    unsigned char**b = board;
    for ( i = s - 4 ; i >= 0 ; i -- )
    {
        for ( j = 0 ; j <= s-4-i ; j ++ )
        {
            x = o = 0;
            for ( k = s-1-j, l = i+j ; k > s-5-j ; l ++, k -- )
            {
                if ( b[l][k] == 1 )
                    x ++;
                else if ( b[l][k] == 2 )
                    o ++;
            }
            if ( x == 4 )
                (*X)++;
            else if ( o == 4 )
                (*O)++;
        }
    }
    for ( i = 1 ; i <= s-4 ; i ++ )
    {
        for ( j = 0 ; j <= s-4-i ; j ++ )
        {
            x = o = 0;
            for ( k = s-1-j-i, l = j ; k > s-5-j-i ; l ++, k -- )
            {
                if ( b[l][k] == 1 )
                    x ++;
                else if ( b[l][k] == 2 )
                    o ++;
            }
            if ( x == 4 )
                (*X)++;
            else if ( o == 4 )
                (*O)++;
        }
    }
}

void GameWindow::checkWin()
{
    int x = 0, o = 0;
    sumHorLines(&x, &o);
    sumVerLines(&x, &o);
    sumLDiags(&x, &o);
    sumRDiags(&x, &o);
    switch(type)
    {
    case 0 :
        if ( filled == size*size )
        {
            gameStat = OVER;
            ui->statusBar->showMessage("Game is over. Please restart a new game.");
            if ( x > o )
                ui->label_5->setText("Congratulation you have beaten our intelligence.");
            else if ( o > x )
                ui->label_5->setText("We am sorry, our intelligence beats you.");
            else
                ui->label_5->setText("No winner in this match.");
        }
        break;
    case 1 :
        if ( filled == size*size )
        {
            gameStat = OVER;
            ui->statusBar->showMessage("Game is over. Please restart a new game.");
            ui->label_5->setText("No winner in this match.");
        }
        if ( x > 0 )
        {
            gameStat = OVER;
            ui->statusBar->showMessage("Game is over. Please restart a new game.");
            ui->label_5->setText("Congratulation you have beaten our intelligence.");
        }
        else if ( o > 0 )
        {
            gameStat = OVER;
            ui->statusBar->showMessage("Game is over. Please restart a new game.");
            ui->label_5->setText("Our am sorry, our intelligence beats you.");
        }
        break;
    }
}
