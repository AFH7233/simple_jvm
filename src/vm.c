#include "vm.h"
#include <stdio.h>
#include <stdlib.h>

void vm_init(struct VM *vm) {
  vm->pc = 0;
  vm->sp = 0;
  vm->bytecode = nullptr;
  vm->code_length = 0;
}

void vm_load_class(struct VM *vm, struct class_file *jvm_class) {
  vm->jvm_class = jvm_class;
}

void vm_run(struct VM *vm) {
  while (vm->pc < 1) {
  }
}
