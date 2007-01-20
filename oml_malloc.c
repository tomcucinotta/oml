#include "oml_debug.h"

#if defined(__KERNEL__)
#  include <linux/kernel.h>
#  include <linux/module.h>
#  include <linux/sched.h>
#  include <linux/list.h>
#  include <linux/slab.h>
#else
#  include <stdlib.h>
#endif

#include "oml_malloc.h"

#ifdef OML_MEMORY_CHECK

#define MAX_CHUNK_NUMBER 150
static void * chunks[MAX_CHUNK_NUMBER];
static int curr_chunks = 0;
static int mem_failure = 0;
#ifdef __KERNEL__
spinlock_t chunks_lock __cacheline_aligned;
#endif

/** Adds ptr to chunks[].
 * Performance is not important, here */
void add_chunk(void *ptr) {
  int i;
  for (i=0; i<MAX_CHUNK_NUMBER; i++)
    if (chunks[i] == 0) {
      chunks[i] = ptr;
      curr_chunks++;
      return;
    }
}

/** If ptr is found in chunks[], then remove it and return 1.
 * Otherwise, return 0 */
int rem_chunk(void *ptr) {
  int i;
  for (i=0; i<MAX_CHUNK_NUMBER; i++)
    if (chunks[i] == ptr) {
      chunks[i] = 0;
      curr_chunks--;
      return 1;
    }
  return 0;
}

#ifdef __KERNEL__
#  define chunks_vars unsigned long flags
#  define chunks_lock spin_lock_irqsave(&chunks_lock, flags)
#  define chunks_unlock spin_unlock_irqrestore(&chunks_lock, flags)
#else
#  define chunks_vars
#  define chunks_lock
#  define chunks_unlock
#endif

#endif /* MEMORY_CHECK */

static inline void *oml_ll_malloc(long size) {
#if defined(__KERNEL__)
  return kmalloc(size, GFP_ATOMIC);
#else
  return malloc(size);
#endif
}

static inline void oml_ll_free(void *ptr) {
#if defined(__KERNEL__)
  kfree(ptr);
#else
  free(ptr);
#endif
}

void *oml_malloc(long size) {
  void *ptr;
#ifdef OML_MEMORY_CHECK
  chunks_vars;
#endif
  ptr = oml_ll_malloc(size);
  if (ptr == 0)
    return 0;
#ifdef OML_MEMORY_CHECK
  chunks_lock;
  if (curr_chunks == MAX_CHUNK_NUMBER) {
    oml_log_err("Chunks exhausted: cannot debug memory allocation");
    oml_ll_free(ptr);
    mem_failure = 1;
    chunks_unlock;
    return 0;
  }
  add_chunk(ptr);
  chunks_unlock;
#endif
  return ptr;
}

void oml_free(void *ptr) {
#ifdef OML_MEMORY_CHECK
  chunks_vars;
#endif
  if (ptr == 0) {
    oml_log_err("Trying to free a NULL pointer");
    return;
  }
#ifdef OML_MEMORY_CHECK
  chunks_lock;
  if (! rem_chunk(ptr)) {
    oml_log_err("Freeing a non-allocated or already freed memory chunk");
    chunks_unlock;
    return;
  }
  chunks_unlock;
#endif
  oml_ll_free(ptr);
}

int oml_mem_clean() {
#ifdef OML_MEMORY_CHECK
  return ((curr_chunks == 0) && (mem_failure == 0));
#else
  return 0 == 0;
#endif
}

#ifdef __KERNEL__
EXPORT_SYMBOL_GPL(oml_malloc);
EXPORT_SYMBOL_GPL(oml_free);
EXPORT_SYMBOL_GPL(oml_mem_clean);
#endif
