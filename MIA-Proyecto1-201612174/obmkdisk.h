#ifndef OBMKDISK_H
#define OBMKDISK_H
#include <QString>
#include "string"
using namespace std;


class obmkdisk
{
public:
    obmkdisk();
    int size;
    string fit;
    string unit;
    string path;

    void mostrardatos(obmkdisk *disco);
};

#endif // OBMKDISK_H
