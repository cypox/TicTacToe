#ifndef CONFIG_H
#define CONFIG_H

#include <QList>

class Config
{
public:
    Config(unsigned char**, unsigned char, int);
    ~Config();
    int getLast();
    void setLast(int);
    QList<Config> generate(unsigned char);
    QList<Config> generate_reduced(unsigned char);
    unsigned char getSize();
    unsigned char** status;
private:
    int last;
    unsigned char size;
};

#endif // CONFIG_H
