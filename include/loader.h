#ifndef LOADER_H
#define LOADER_H

#include <stdio.h>
#include "common.h"

enum constant_tag{
    CONSTANT_Utf8               = 1,
    CONSTANT_Integer             = 3,
    CONSTANT_Float               = 4,
    CONSTANT_Long                = 5,
    CONSTANT_Double               = 6,
    CONSTANT_Class                = 7,
    CONSTANT_String               = 8,
    CONSTANT_Fieldref             = 9,
    CONSTANT_Methodref            = 10,
    CONSTANT_InterfaceMethodref   = 11,
    CONSTANT_NameAndType          = 12,
    CONSTANT_MethodHandle         = 15,
    CONSTANT_MethodType           = 16,
    CONSTANT_Dynamic              = 17,
    CONSTANT_InvokeDynamic        = 18,
    CONSTANT_Module               = 19,
    CONSTANT_Package              = 20
} ;

struct class_info{
    u1 tag;
    u2 name_index;
};

struct class_file{
    u4 magic;
    u2 major_version;
    u2 minor_version;
    u2 constant_pool_count;

};

struct class_file* load_class_file(const char* name);
void free_class_file(struct class_file* jvm_class);

#endif //LOADER_H
