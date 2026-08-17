#include "vm.h"
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
//static void vm_push(struct VM *vm, uint32_t value);
//static uint32_t vm_pop(struct VM *vm);
static void vm_execute_code(struct VM *vm, struct code_attribute *code);

void vm_run_main(struct VM *vm, struct class_file *jvm_class) {
  vm->pc = 0;
  vm->sp = 0;

  struct method_info *main_method = find_method_info(jvm_class, "main", "([Ljava/lang/String;)V");
  if (!main_method) {
    fprintf(stderr, "No hay main method padrino\n");
    exit(EXIT_FAILURE);
  }

#ifdef DEBUG
  for (int i = 0; i < main_method->attribute_count; i++) {
    LOG_SEPARATOR;
    LOG("Index %d\n", main_method->attributes[i].name_index);
    LOG("Tag %d\n", jvm_class->constant_pool[main_method->attributes[i].name_index].tag);
    char *name = get_utf8_string(jvm_class, main_method->attributes[i].name_index);
    LOG("Name %s\n", name);
    LOG_SEPARATOR;
  }
#endif

  struct attribute_info *code_attribute = find_attribute_info(jvm_class, main_method, "Code");
  if (!code_attribute) {
    fprintf(stderr, "No hay code attribute padrino\n");
    exit(EXIT_FAILURE);
  }

  LOG("Code attribute length: %d\n", code_attribute->length);
  u1 *info = code_attribute->info;
  struct code_attribute code;
  code.name_index = code_attribute->name_index;
  code.length = code_attribute->length;
  code.max_stack = (info[0] << 8) | info[1];
  code.max_locals = (info[2] << 8) | info[3];
  code.bytecode_length = (info[4] << 24) | (info[5] << 16) | (info[6] << 8) | info[7];
  LOG("Main method bytecode length: %d\n", code.bytecode_length);
  code.bytecode = &info[8];
  vm_execute_code(vm, &code);
}

/*
 *Aqui debo reformular algo como que puedo correr cada pedazo de metodo por partes
 *como llamada de microporcesadores y vm global debe ser diferente
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
}*/

static void vm_execute_code(struct VM *vm, struct code_attribute *code) {
  // Que hago con el stack y locals que obtuve del code?
  while (vm->pc < code->bytecode_length) {
    const u1 opcode = code->bytecode[vm->pc];
    vm->pc++;
    switch (opcode) {
      default:
        LOG("OPCODE: 0x%X at PC: %d\n", opcode, vm->pc - 1);
    }
  }
}
