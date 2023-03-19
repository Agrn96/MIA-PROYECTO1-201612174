#ifndef OBMRDISK_H
#define OBMRDISK_H
#include <QString>
#include "string"
using namespace std;

class obmrdisk
{
public:
    obmrdisk();

    string path;

    void mostrardatos(string path);
};

#endif // OBMRDISK_H
