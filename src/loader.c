#include "loader.h"
#include <stdio.h>
#include <stdlib.h>

#include "../include/log.h"

/*
static u1 read_u1(FILE* file) {
  u1 val;
  fread(&val, 1, 1, file);
  return val;
}*/

static u2 read_u2(FILE* file) {
  u1 bytes[2];
  fread(&bytes, 1, 2, file);
  return (bytes[0] << 8) | bytes[1];
}


static u4 read_u4(FILE* file) {
  u1 bytes[4];
  fread(&bytes, 1, 4, file);
  return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

struct class_file* load_class_file(const char* name) {
  FILE* file = fopen(name, "rb");
  if (!file) {
    fprintf(stderr, "Failed to open file");
    return NULL;
  }

  struct class_file*  jvm_class = malloc(sizeof(struct class_file));

  //Magic
  jvm_class->magic = read_u4(file);
  if (jvm_class->magic != 0xCAFEBABE) {
    fprintf(stderr, "No es una clase de jvm hdtpm!!\a\n");
    free(jvm_class);
    fclose(file);
    return NULL;
  }

  //Version
  jvm_class->minor_version = read_u2(file);
  jvm_class->major_version = read_u2(file);

  LOG("Magic Number: 0x%X\n", jvm_class->magic);
  LOG("Major version: %d\n", jvm_class->major_version);

  fclose(file);
  return jvm_class;
}

void free_class_file(struct class_file* jvm_class) {
  if ( !jvm_class) return;
  free(jvm_class);
}
