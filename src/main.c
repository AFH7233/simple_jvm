#include <stdio.h>
#include <stdlib.h>
#include "loader.h"
#include "vm.h"

int main(const int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "%s <class_file?\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  const char *file_name = argv[1];
  struct class_file *jvm_class = load_class_file(file_name);
  if (!jvm_class) {
    fprintf(stderr, "Cannot read file\n");
    exit(EXIT_FAILURE);
  }

  struct VM vm;
  vm_init(&vm);
  vm_load_class(&vm, jvm_class);

  free_class_file(jvm_class);
  return EXIT_SUCCESS;
}
