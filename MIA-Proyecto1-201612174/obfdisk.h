#ifndef OBFDISK_H
#define OBFDISK_H
#include <QString>
#include "string"
#include "structs.h"
using namespace std;

class obfdisk
{
public:
    obfdisk();

    int size;
    string fit = "WF";
    string unit = "K";
    string path;
    string type = "P";
    string name;
    string delet = "";
    bool add_type;
    int add_size = 0;

    void mostrardatos(obfdisk *disco);
    void create(obfdisk *disco);
    void add(obfdisk *disco);
    void deleter(obfdisk *disco);


};

#endif // OBFDISK_H
