#ifndef OBEXEC_H
#define OBEXEC_H
#include <QString>
#include "string"
#include "structs.h"
using namespace std;

class obexec
{
public:
    obexec();

    string path;

    void exec();
};

#endif // OBEXEC_H
