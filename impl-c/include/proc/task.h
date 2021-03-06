#pragma once
#include "fs/vfs.h"
#include <stddef.h>
#include <stdint.h>

#define TASK_STATUS_DEAD 0
#define TASK_STATUS_ALIVE 1
#define TASK_MX_NUM_FD 10

struct cpu_context {
  uint64_t x19;
  uint64_t x20;
  uint64_t x21;
  uint64_t x22;
  uint64_t x23;
  uint64_t x24;
  uint64_t x25;
  uint64_t x26;
  uint64_t x27;
  uint64_t x28;
  uint64_t fp; // x29
  uint64_t lr; // x30
  uint64_t sp; // kernel stack pointer
};

struct task_struct {
  struct cpu_context cpu_context;
  unsigned long id;
  int status;

  int fd_size;
  struct file *fd[TASK_MX_NUM_FD];

  uintptr_t kernel_stack;
  uintptr_t user_stack;
  uintptr_t user_sp; // value of sp in el0

  // address of the program code allocaed in memory
  void *code;
  size_t code_size;
};

struct task_struct *task_create(void *func);

void task_free(struct task_struct *task);

void cur_task_exit();

extern uint32_t new_tid;

static inline void _wait() {
  for (uint64_t j = 0; j < (1 << 27); j++) {
    ;
  }
}

static inline struct task_struct *get_current() {
  unsigned long cur;
  asm volatile("mrs %0, tpidr_el1 \n" : "=r"(cur) :);
  return (struct task_struct *)cur;
}

static inline void set_current(struct task_struct *task) {
  asm volatile("msr tpidr_el1, %0 \n" ::"r"(task) :);
}