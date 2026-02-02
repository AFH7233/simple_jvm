#ifndef VM_H
#define VM_H

#include "common.h"
#include <stdint.h>

#define STACK_SIZE 1024
#define LOCALS_SIZE 256

typedef struct {
  u4 pc;
  int stack[STACK_SIZE];
  int sp;
  int locals[LOCALS_SIZE];
} VM;

void vm_init(VM* vm);
void vm_run(VM* vm);
void vm_push(VM* vm, int value);
int vm_pop(VM* vm);

#endif //VM_H
