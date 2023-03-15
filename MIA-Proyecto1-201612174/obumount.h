#ifndef OBUMOUNT_H
#define OBUMOUNT_H
#include <QString>
#include "string"
#include "structs.h"
using namespace std;

class obumount
{
public:
    obumount();

    string id= "";

    void mostrardatos(obumount *disco);
};

#endif // OBUMOUNT_H
