#ifndef CHANGE_H
#define CHANGE_H

#include <stdio.h>

struct change {
    int type;
    int line_number;
    char line_data[1016];
};

#endif