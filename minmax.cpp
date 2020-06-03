#include "minmax.h"

MinMax::MinMax(unsigned char _h, unsigned char _s, int _t, QTreeWidget* tv)
{
    h = _h + 2;
    //h = 1;
    mh = h;
    size = _s;
    type = _t;
    t_view = tv;
}

int MinMax::think(Config* cfg, QTreeWidget* tv, bool debug)
{
    int move;
    QTreeWidgetItem* root = new QTreeWidgetItem;
    int score = minmax(cfg, h, true, move, root, debug);
    if ( debug )
    {
        root->setBackgroundColor(0, QColor(200, 200, 200, 50));
        root->setBackgroundColor(1, QColor(200, 200, 200, 20));
        root->setBackgroundColor(2, QColor(100, 50, 10, 100));
        root->setTextAlignment(2, Qt::AlignCenter);
        root->setText(0, QString::number(move));
        root->setText(1, "");
        root->setText(2, QString::number(score));
        tv->addTopLevelItem(root);
    }
    return move;
}

int calculate(int tmp)
{
    switch(tmp)
    {
        case 1:return 1;
        case 2:return 10;
        case 3:return 100;
        case 4:return 1000;
        default: return 0;
    }
}

int MinMax::minmax(Config* cfg, unsigned char h, bool max, int& best, QTreeWidgetItem* parent, bool debug)
{
    int score, tmpScore, tmpMove;
    if ( h <= 0 )
    {
        best = cfg->getLast();
        return  heuristic(cfg);
    }
    if ( max )
    {
        score = -100000;
        //*New Version
        int i, j;
        for ( i = 0 ; i < size ; i++ )
            for (j = 0 ; j < size ; j++ )
                if ( cfg->status[i][j] != 1 && cfg->status[i][j] != 2 )
                {
                    int l = cfg->getLast();
                    cfg->status[i][j] = 2;
                    cfg->setLast(i*size+j);
                    tmpMove = -1;
                    QTreeWidgetItem* item;
                    if ( debug )
                    {
                        item = new QTreeWidgetItem;
                        item->setText(0, QString::number(i*size+j));
                        item->setText(1, "O");
                        item->setBackgroundColor(0, QColor(200, 200, 200, 50));
                        item->setBackgroundColor(1, QColor(200, 200, 200, 20));
                        item->setTextAlignment(2, Qt::AlignCenter);
                    }
                    tmpScore = minmax(cfg, h-1, false, tmpMove, item, debug);
                    if ( tmpScore > score && tmpMove >= 0 )
                    {
                        best = cfg->getLast();
                        score = tmpScore;
                    }
                    else if ( tmpMove < 0 )
                    {
                        best = cfg->getLast();
                    }
                    cfg->setLast(l);
                    cfg->status[i][j] = 0;
                    if ( debug )
                    {
                        item->setBackgroundColor(2, QColor(100, 50, 10, 100));
                        item->setText(2, QString::number(tmpScore));
                        parent->addChild(item);
                    }
                }
        //End of new Version*/
        /*
        QList<Config> moves = cfg->generate(2);
        foreach(Config c, moves)
        {
            //qDebug("Move = %d\n", c.getLast());
            tmpMove = -1;
            tmpScore = minmax(&c, h-1, false, tmpMove);
            if ( tmpScore > score && tmpMove >= 0 )
            {
                best = c.getLast();
                score = tmpScore;
            }
            else if ( tmpMove < 0 )
            {
                best = c.getLast();
            }
            //qDebug("Score = %d for move %d -> %d\n", score, c.getLast(), best);
        }
        //*/
        return score;
    }
    else
    {
        score = 100000;
        //*New
        int i, j;
        for ( i = 0 ; i < size ; i++ )
            for (j = 0 ; j < size ; j++ )
                if ( cfg->status[i][j] != 1 && cfg->status[i][j] != 2 )
                {
                    int l = cfg->getLast();
                    cfg->status[i][j] = 1;
                    cfg->setLast(i*size+j);
                    tmpMove = -1;
                    QTreeWidgetItem* item;
                    if ( debug )
                    {
                        item = new QTreeWidgetItem;
                        item->setText(0, QString::number(i*size+j));
                        item->setBackgroundColor(0, QColor(200, 200, 200, 50));
                        item->setBackgroundColor(1, QColor(200, 200, 200, 20));
                        item->setTextAlignment(2, Qt::AlignCenter);
                        item->setText(1, "X");
                    }
                    tmpScore = minmax(cfg, h-1, true, tmpMove, item, debug);
                    if ( tmpScore < score && tmpMove >= 0 )
                    {
                        best = cfg->getLast();
                        score = tmpScore;
                    }
                    else if ( tmpMove < 0 )
                    {
                        best = cfg->getLast();
                    }
                    cfg->setLast(l);
                    cfg->status[i][j] = 0;
                    if ( debug )
                    {
                        item->setBackgroundColor(2, QColor(100, 50, 10, 100));
                        item->setText(2, QString::number(tmpScore));
                        parent->addChild(item);
                    }
                }
        //End of new Version*/
        /*
        QList<Config> moves = cfg->generate(1);
        foreach(Config c, moves)
        {
            tmpMove = -1;
            tmpScore = minmax(&c, h-1, true, tmpMove);
            if ( tmpScore < score && tmpMove >= 0 )
            {
                best = c.getLast();
                score = tmpScore;
            }
            else if ( tmpMove < 0 )
            {
                best = c.getLast();
            }
            //qDebug("    Score = %d for move %d\n", tmpScore, c.getLast());
        }
        //*/
        //qDebug("Score = %d for move %d -> %d\n", score, cfg->getLast(), best);
        return score;
    }
}

int getLineScore(int s, unsigned char ** b)
{
    int i, j, k, tmpx, tmpo, to, tx, score = 0;
    //Lines Score
    for ( i = 0 ; i < s ; i++ )
    {
        for ( j = 0 ; j <= s-4 ; j++ )
        {
            tmpx = tmpo = 0; tx = to = 1;
            for ( k = 0 ; k < 4 ; k ++ )
            {
                if ( b[i][j+k] == 1 || b[i][j+k] == 0 )
                {
                    if ( b[i][j+k] == 1 )
                    {
                        tmpx ++;
                        if ( tmpx == 4 )
                            j += 3;
                        to = 0;
                    }
                }
                else if ( b[i][j+k] == 2 || b[i][j+k] == 0 )
                {
                    if ( b[i][j+k] == 2 )
                    {
                        tmpo ++;
                        if ( tmpo == 4 )
                            j += 3;
                        tx = 0;
                    }
                }
            }
            if ( tx != 0 )
            {
                score -= calculate(tmpx);
            }
            if ( to != 0 )
            {
                score += calculate(tmpo);
            }
        }
    }
    return score;
}

int getColScore(int s, unsigned char ** b)
{
    int i, j, k, tmpx, tmpo, to, tx, score = 0;
    //Column Score
    for ( j = 0 ; j < s ; j++ )
    {
        for ( i = 0 ; i <= s-4 ; i++ )
        {
            tmpx = tmpo = 0; tx = to = 1;
            for ( k = 0 ; k < 4 ; k ++ )
            {
                if ( b[i+k][j] == 1 || b[i+k][j] == 0 )
                {
                    if ( b[i+k][j] == 1 )
                    {
                        tmpx ++;
                        if ( tmpx == 4 )
                            i += 3;
                        to = 0;
                    }
                }
                else if ( b[i+k][j] == 2 || b[i+k][j] == 0 )
                {
                    if ( b[i+k][j] == 2 )
                    {
                        tmpo ++;
                        if ( tmpo == 4 )
                            i += 3;
                        tx = 0;
                    }
                }
            }
            if ( tx != 0 )
            {
                score -= calculate(tmpx);
            }
            if ( to != 0 )
            {
                score += calculate(tmpo);
            }
        }
    }
    return score;
}

int getDiagScoreLR(int s, unsigned char** b)
{
    int i, j, k, tmpx, tmpo, to, tx, score = 0;
    //Diagonals Score
    //Diagonals of form \\ from bottom to 0
    for ( i = s - 4 ; i >= 0 ; i -- )
    {
        for ( j = 0 ; j <= s-4-i ; j ++ )
        {
            tmpx = tmpo = 0; tx = to = 1;
            for ( k = j ; k < j+4 ; k ++ )
            {
                if ( b[i+k][k] == 1 || b[i+k][k] == 0 )
                {
                    if ( b[i+k][k] == 1 )
                    {
                        tmpx ++;
                        if ( tmpx == 4 )
                        {
                            j += 4;
                            k = j+4;
                        }
                        to = 0;
                    }
                }
                else if ( b[i+k][k] == 2 || b[i+k][k] == 0 )
                {
                    if ( b[i+k][k] == 2 )
                    {
                        tmpo ++;
                        if ( tmpo == 4 )
                        {
                            j += 4;
                            k = j+4;
                        }
                        tx = 0;
                    }
                }
            }
            if ( tx != 0 )
            {
                score -= calculate(tmpx);
            }
            if ( to != 0 )
            {
                score += calculate(tmpo);
            }
        }
    }

    //Diagonals of form \\ from 0 to top
    for ( i = 1 ; i <= s-4 ; i ++ )
    {
        for ( j = 0 ; j <= s-4-i ; j ++ )
        {
            tmpx = tmpo = 0; tx = to = 1;
            for ( k = i+j ; k < i+j+4 ; k ++ )
            {
                if ( b[k-j-1][k] == 1 || b[k-j-1][k] == 0 )
                {
                    if ( b[k-j-1][k] == 1 )
                    {
                        tmpx ++;
                        if ( tmpx == 4 )
                        {
                            j += 3;
                            k = i+j+4;
                        }
                        to = 0;
                    }
                }
                else if ( b[k-j-1][k] == 2 || b[k-j-1][k] == 0 )
                {
                    if ( b[k-j-1][k] == 2 )
                    {
                        tmpo ++;
                        if ( tmpo == 4 )
                        {
                            j += 3;
                            k = j+4;
                        }
                        tx = 0;
                    }
                }
            }
            if ( tx != 0 )
            {
                score -= calculate(tmpx);
            }
            if ( to != 0 )
            {
                score += calculate(tmpo);
            }
        }
    }
    return score;
}

int getDiagScoreRL(int s, unsigned char** b)
{
    int i, j, k, tmpx, tmpo, to, tx, score = 0;
    int l;
    //Diags // from bot to 0
    for ( i = s - 4 ; i >= 0 ; i -- )
    {
        for ( j = 0 ; j <= s-4-i ; j ++ )
        {
            tmpx = tmpo = 0; tx = to = 1;
            for ( k = s-1-j, l = i+j ; k > s-5-j ; l ++, k -- )
            {
                if ( b[l][k] == 1 || b[l][k] == 0 )
                {
                    if ( b[l][k] == 1 )
                    {
                        tmpx ++;
                        if ( tmpx == 4 )
                        {
                            j += 3;
                            k = s-5-j;
                        }
                        to = 0;
                    }
                }
                else if ( b[l][k] == 2 || b[l][k] == 0 )
                {
                    if ( b[l][k] == 2 )
                    {
                        tmpo ++;
                        if ( tmpo == 4 )
                        {
                            j += 3;
                            k = s-5-j;
                        }
                        tx = 0;
                    }
                }
            }
            if ( tx != 0 )
            {
                score -= calculate(tmpx);
            }
            if ( to != 0 )
            {
                score += calculate(tmpo);
            }
        }
    }
    //Diags // from 0 to top
    for ( i = 1 ; i <= s-4 ; i ++ )
    {
        for ( j = 0 ; j <= s-4-i ; j ++ )
        {
            tmpx = tmpo = 0; tx = to = 1;
            for ( k = s-1-j-i, l = j ; k > s-5-j-i ; l ++, k -- )
            {
                if ( b[l][k] == 1 || b[l][k] == 0 )
                {
                    if ( b[l][k] == 1 )
                    {
                        tmpx ++;
                        if ( tmpx == 4 )
                        {
                            j += 3;
                            k = s-5-j-i;
                        }
                        to = 0;
                    }
                }
                else if ( b[l][k] == 2 || b[l][k] == 0 )
                {
                    if ( b[l][k] == 2 )
                    {
                        tmpo ++;
                        if ( tmpo == 4 )
                        {
                            j += 3;
                            k = s-5-j-i;
                        }
                        tx = 0;
                    }
                }
            }
            if ( tx != 0 )
            {
                score -= calculate(tmpx);
            }
            if ( to != 0 )
            {
                score += calculate(tmpo);
            }
        }
    }
    return score;
}

int MinMax::heuristic(Config* cfg)
{
    int s = cfg->getSize();
    unsigned char **b = cfg->status;
    int score = 0;
    score += getLineScore(s, b);
    score += getColScore(s, b);
    score += getDiagScoreLR(s, b);
    score += getDiagScoreRL(s, b);
    /* Dont allow cpu to run behind player (advantage player with first move)
    if ( b[cfg->getLast()/s][cfg->getLast()%s] == 2 )
    {
        if ( score < 0 )
            score *= 3;
        else
            score /= 3;
    }
    else if ( b[cfg->getLast()/s][cfg->getLast()%s] == 1 )
    {
            if ( score > 0 )
                score *= 3;
            else
                score /= 3;
    }
    //*/
    return score;
}

int _heuristic(Config *)
{
    return rand()%20000-10000;
}
