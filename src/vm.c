#include "vm.h"
#include <stdio.h>
#include <stdlib.h>

#include "log.h"

void vm_init(struct VM *vm) {
  vm->pc = 0;
  vm->sp = 0;
  vm->bytecode = nullptr;
  vm->code_length = 0;
}

void vm_load_class(struct VM *vm, struct class_file *jvm_class) {
  vm->jvm_class = jvm_class;

  struct method_info* main_method = find_method_info(jvm_class, "main", "([Ljava/lang/String;)V");
  if (!main_method) {
    fprintf(stderr, "No hay main method padrino\n");
    exit(EXIT_FAILURE);
  }

#ifdef DEBUG
  for (int i=0 ; i<main_method->attribute_count ; i++) {
    LOG_SEPARATOR;
    LOG("Index %d\n", main_method->attributes[i].name_index);
    LOG("Tag %d\n", vm->jvm_class->constant_pool[main_method->attributes[i].name_index].tag);
    char* name = get_utf8_string( vm->jvm_class, main_method->attributes[i].name_index);
    LOG("Name %s\n", name);
    LOG_SEPARATOR;
  }
#endif

  struct attribute_info* code_attribute = find_attribute_info(jvm_class, main_method, "Code");
  if (!code_attribute) {
    fprintf(stderr, "No hay code attribute padrino\n");
    exit(EXIT_FAILURE);
  }
  u1* info = code_attribute->info;
  vm->code_length =(info[4] << 24) | (info[5] << 16) | (info[6] << 8) | info[7];
  vm->bytecode = &info[8];
  vm->pc = 0;
}

void vm_push(struct VM *vm, uint32_t value) {
  if (vm->sp >= STACK_SIZE) {
    fprintf(stderr, "Stack overflow\n");
    exit(EXIT_FAILURE);
  }
  vm->stack[vm->sp] = value;
  vm->sp++;
}

uint32_t vm_pop(struct VM *vm) {
  if (vm->sp <= 0) {
    fprintf(stderr, "Stack underflow\n");
    exit(EXIT_FAILURE);
  }
  vm->sp--;
  return vm->stack[vm->sp];
}



void vm_run(struct VM *vm) {
  while (vm->pc < vm->code_length) {
  }
}
