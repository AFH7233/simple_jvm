#ifndef VM_H
#define VM_H

#include "common.h"
#include "loader.h"

struct VM {
  u4 pc;
  int sp;
};

void vm_run_main(struct VM *vm, struct class_file *class_file);

#endif //VM_H
