#ifndef LOADER_H
#define LOADER_H

#include <stdio.h>
#include "common.h"

typedef struct {
    u4 magic;
    u2 major_version;
    u2 minor_version;
} class_file;

class_file* load_class_file(const char* name);
void free_class_file(class_file* jvm_class);

#endif //LOADER_H
