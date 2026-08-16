#ifndef VM_H
#define VM_H

#include "common.h"
#include <stdint.h>

#define STACK_SIZE 1024
#define LOCALS_SIZE 256

struct VM {
  u4 pc;
  int stack[STACK_SIZE];
  int sp;
  int locals[LOCALS_SIZE];
};

void vm_init(struct VM *vm);
void vm_run(struct VM *vm);
void vm_push(struct VM *vm, int value);
int vm_pop(struct VM *vm);

#endif //VM_H
