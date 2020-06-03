#ifndef MINMAX_H
#define MINMAX_H

#include <QTableWidget>
#include <QThread>
#include "config.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>

class MinMax
{
private:
    unsigned char h;
    unsigned char size;
    unsigned char mh;
    bool debug;
    QTreeWidget* t_view;
public:
    MinMax(unsigned char, unsigned char, int, QTreeWidget*);
    int heuristic(Config*);
    int think(Config*, QTreeWidget*, bool);
    int minmax(Config*, unsigned char, bool, int&, QTreeWidgetItem*, bool);
    int type;
};

#endif // MINMAX_H
