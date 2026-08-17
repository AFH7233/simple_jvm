#include "loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/log.h"

static u1 read_u1(FILE *file) {
  u1 val;
  fread(&val, 1, 1, file);
  return val;
}

static u2 read_u2(FILE *file) {
  u1 bytes[2];
  fread(&bytes, 1, 2, file);
  return (bytes[0] << 8) | bytes[1];
}

static u4 read_u4(FILE *file) {
  u1 bytes[4];
  fread(&bytes, 1, 4, file);
  return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

struct class_file *load_class_file(const char *name) {
  FILE *file = fopen(name, "rb");
  if (!file) {
    fprintf(stderr, "Failed to open file");
    return nullptr;
  }

  struct class_file *jvm_class = malloc(sizeof(struct class_file));

  //Magic
  jvm_class->magic = read_u4(file);
  if (jvm_class->magic != 0xCAFEBABE) {
    fprintf(stderr, "No es una clase de jvm hdtpm!!\a\n");
    free(jvm_class);
    fclose(file);
    return nullptr;
  }

  //Version
  jvm_class->minor_version = read_u2(file);
  jvm_class->major_version = read_u2(file);

  //Constant Pool
  jvm_class->constant_pool_count = read_u2(file);
  jvm_class->constant_pool = calloc(jvm_class->constant_pool_count, sizeof(struct class_info));
  for (int i = 1; i < jvm_class->constant_pool_count; i++) {
    jvm_class->constant_pool[i].tag = read_u1(file);
    switch (jvm_class->constant_pool[i].tag) {
      case CONSTANT_Utf8: {
        const u2 length = read_u2(file);
        jvm_class->constant_pool[i].info = calloc(3 + length, sizeof(u1));
        jvm_class->constant_pool[i].info[0] = length >> 8;
        jvm_class->constant_pool[i].info[1] = length & 0xff;
        fread(jvm_class->constant_pool[i].info + 2, 1, length, file);
        jvm_class->constant_pool[i].info[length + 2] = (u1) '\0' ; // This way less problems for now
        break;
      }
      case CONSTANT_Integer:
      case CONSTANT_Float:
      case CONSTANT_Fieldref:
      case CONSTANT_Methodref:
      case CONSTANT_InterfaceMethodref:
      case CONSTANT_NameAndType:
      case CONSTANT_Dynamic:
      case CONSTANT_InvokeDynamic: {
        jvm_class->constant_pool[i].info = malloc(4);
        fread(jvm_class->constant_pool[i].info, 1, 4, file);
        break;
      }
      case CONSTANT_Long:
      case CONSTANT_Double: {
        jvm_class->constant_pool[i].info = malloc(8);
        fread(jvm_class->constant_pool[i].info, 1, 8, file);
        i++;
        break;
      }
      case CONSTANT_String:
      case CONSTANT_Class:
      case CONSTANT_Module:
      case CONSTANT_Package:
      case CONSTANT_MethodType: {
        jvm_class->constant_pool[i].info = malloc(2);
        fread(jvm_class->constant_pool[i].info, 1, 2, file);
        break;
      }
      case CONSTANT_MethodHandle: {
        jvm_class->constant_pool[i].info = malloc(3);
        fread(jvm_class->constant_pool[i].info, 1, 3, file);
        break;
      }
      default:
        fprintf(stderr, "Unknown constant tag: %d at index %d\n", jvm_class->constant_pool[i].tag, i);
        free_class_file(jvm_class);
        fclose(file);
        return nullptr;
    }
  }

  jvm_class->access_flags = read_u2(file);
  jvm_class->this_class = read_u2(file);
  jvm_class->super_class = read_u2(file);

  jvm_class->interfaces_count = read_u2(file);
  if (jvm_class->interfaces_count > 0) {
    jvm_class->interfaces = calloc(jvm_class->interfaces_count, sizeof(u2));
    for (int i = 0; i < jvm_class->interfaces_count; i++) {
      jvm_class->interfaces[i] = read_u2(file);
    }
  }

  jvm_class->fields_count = read_u2(file);
  for (int i = 0; i < jvm_class->fields_count; i++) {
    jvm_class->fields[i].access_flags = read_u2(file);
    jvm_class->fields[i].name_index = read_u2(file);
    jvm_class->fields[i].descriptor_index = read_u2(file);
    jvm_class->fields[i].attribute_count = read_u2(file);
    jvm_class->fields[i].attributes = calloc(jvm_class->fields[i].attribute_count, sizeof(u2));
    for (int j = 0; j < jvm_class->fields[i].attribute_count; j++) {
      jvm_class->fields[i].attributes[j].name_index = read_u2(file);
      jvm_class->fields[i].attributes[j].length = read_u4(file);
      if (jvm_class->fields[i].attributes[j].length > 0) {
        jvm_class->fields[i].attributes[j].info = calloc(jvm_class->fields[i].attributes[j].length, sizeof(u1));
        fread(jvm_class->fields[i].attributes[j].info, 1, jvm_class->fields[i].attributes[j].length, file);
      }
    }
  }

  jvm_class->methods_count = read_u2(file);
  jvm_class->methods = calloc(jvm_class->methods_count, sizeof(struct method_info));
  for (int i = 0; i < jvm_class->methods_count; i++) {
    jvm_class->methods[i].access_flags = read_u2(file);
    jvm_class->methods[i].name_index = read_u2(file);
    jvm_class->methods[i].descriptor_index = read_u2(file);
    jvm_class->methods[i].attribute_count = read_u2(file);
    jvm_class->methods[i].attributes = calloc(jvm_class->methods[i].attribute_count, sizeof(u2));
    for (int j = 0; j < jvm_class->methods[i].attribute_count; j++) {
      jvm_class->methods[i].attributes[j].name_index = read_u2(file);
      jvm_class->methods[i].attributes[j].length = read_u4(file);
      if (jvm_class->methods[i].attributes[j].length > 0) {
        jvm_class->methods[i].attributes[j].info = calloc(jvm_class->methods[i].attributes[j].length, sizeof(u1));
        fread(jvm_class->methods[i].attributes[j].info, 1, jvm_class->methods[i].attributes[j].length, file);
      }
    }
  }

  jvm_class->attribute_count = read_u2(file);
  jvm_class->attributes = calloc(jvm_class->attribute_count, sizeof(struct attribute_info));
  for (int i = 0; i < jvm_class->attribute_count; i++) {
    jvm_class->attributes[i].name_index = read_u2(file);
    jvm_class->attributes[i].length = read_u4(file);
    if (jvm_class->attributes[i].length > 0) {
      jvm_class->attributes[i].info = calloc(jvm_class->attributes[i].length, sizeof(u1));
      fread(jvm_class->attributes[i].info, 1, jvm_class->attributes[i].length, file);
    }
  }

  LOG("Magic Number: 0x%X\n", jvm_class->magic);
  LOG("Major version: %d\n", jvm_class->major_version);
  LOG("Minor version: %d\n", jvm_class->minor_version);
  LOG("Constant pool count: %d\n", jvm_class->constant_pool_count);

  fclose(file);
  return jvm_class;
}

struct method_info *find_method_info(struct class_file *jvm_class, const char *name, const char *descriptor);
struct attribute_info *find_attribute_info(struct class_file *jvm_class,
                                           struct method_info *method,
                                           const char *descriptor);

void free_class_file(struct class_file *jvm_class) {
  if (!jvm_class)
    return;

  if (jvm_class->constant_pool) {
    for (u2 i = 1; i < jvm_class->constant_pool_count; i++) {
      free(jvm_class->constant_pool[i].info);
    }

    free(jvm_class->constant_pool);
  }

  free(jvm_class->interfaces);

  if (jvm_class->fields) {
    for (u2 i = 0; i < jvm_class->fields_count; i++) {
      if (jvm_class->fields[i].attributes) {
        for (u2 j = 0; j < jvm_class->fields[i].attribute_count; j++) {
          free(jvm_class->fields[i].attributes[j].info);
        }

        free(jvm_class->fields[i].attributes);
      }
    }

    free(jvm_class->fields);
  }

  if (jvm_class->methods) {
    for (u2 i = 0; i < jvm_class->methods_count; i++) {
      if (jvm_class->methods[i].attributes) {
        for (u2 j = 0; j < jvm_class->methods[i].attribute_count; j++) {
          free(jvm_class->methods[i].attributes[j].info);
        }

        free(jvm_class->methods[i].attributes);
      }
    }

    free(jvm_class->methods);
  }

  if (jvm_class->attributes) {
    for (u2 i = 0; i < jvm_class->attribute_count; i++) {
      free(jvm_class->attributes[i].info);
    }

    free(jvm_class->attributes);
  }

  free(jvm_class);
}

struct method_info *find_method_info(struct class_file *jvm_class, const char *name, const char *descriptor) {
  for (int i = 0; i < jvm_class->methods_count; i++) {
    const char *method_name = get_utf8_string(jvm_class, jvm_class->methods[i].name_index);
    const char *method_descriptor = get_utf8_string(jvm_class, jvm_class->methods[i].descriptor_index);
    if (method_name && method_descriptor && strcmp(method_name, name) == 0 && strcmp(method_descriptor, descriptor) ==
      0) {
      return &jvm_class->methods[i];
    }
  }
  return nullptr;
}
struct attribute_info *find_attribute_info(struct class_file *jvm_class,
                                           struct method_info *method,
                                           const char *name) {
  for (int i = 0; i < method->attribute_count; i++) {
    const char *attribute_name = get_utf8_string(jvm_class, method->attributes[i].name_index);
    if (attribute_name && strcmp(attribute_name, name) == 0) {
      return &jvm_class->attributes[i];
    }
  }
  return nullptr;
}

char *get_utf8_string(struct class_file *jvm_class, u2 index) {
  if (index >= jvm_class->constant_pool_count || jvm_class->constant_pool[index].tag != CONSTANT_Utf8) {
    return nullptr;
  }
  return (char *) (jvm_class->constant_pool[index].info + 2);
}
