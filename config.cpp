#include "config.h"

Config::Config(unsigned char **table, unsigned char _size, int _last)
{
    int i, j;
    size = _size;
    status = (unsigned char**)malloc(size*sizeof(unsigned char*));
    for ( i = size ; i-- ; )
        status[i] = (unsigned char*)malloc(size*sizeof(unsigned char));
    for ( i = size ; i -- ; )
        for ( j = size ; j -- ; )
            status[i][j] = table[i][j];
    last = _last;
}

Config::~Config()
{
    //free(status);
}

int Config::getLast()
{
    return last;
}

void Config::setLast(int _last)
{
    last = _last;
}

QList<Config> Config::generate(unsigned char player)
{
    QList<Config> lst;
    /*
    lst = generate_reduced(player);
    if ( lst.size() )
        return lst;
    //*/
    int i = size, j = size;
    for ( i = 0 ; i < size ; i++ )
        for (j = 0 ; j < size ; j++ )
            if ( status[i][j] != 1 && status[i][j] != 2 )
            {
                status[i][j] = player;
                Config tmp(status, size, i*size+j);
                lst.push_back(tmp);
                status[i][j] = 0;
            }
    return lst;
}

QList<Config> Config::generate_reduced(unsigned char player)
{
    QList<Config> lst;
    int x = last / size, y = last % size;
    int i , j;
    for ( i = x-3 ; i <= x+3 ; i++ )
        for (j = y-3 ; j <= y+3 ; j++ )
            if ( 0 <= i && i < size && 0 <= j && j < size && status[i][j] != 1 && status[i][j] != 2 )
            {
                status[i][j] = player;
                Config tmp(status, size, i*size+j);
                lst.push_back(tmp);
                status[i][j] = 0;
            }
    return lst;
}

unsigned char Config::getSize()
{
    return size;
}
