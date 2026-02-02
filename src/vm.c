#include "vm.h"
#include <stdio.h>
#include <stdlib.h>

void vm_init(VM* vm) {
  vm->pc = 0;
  vm->sp = 0;
}

void vm_run(VM* vm) {
  while (vm->pc < 1) {

  }
}
