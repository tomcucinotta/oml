#ifndef __OML_MEMORY_H__
#define __OML_MEMORY_H__

/** @file
 * @brief Space-independent memory allocation functions
 *
 * These functions allocate and free a memory segment,
 * independently of the space in which they are being compiled.
 * For user-space, they map to malloc() and free().
 * For kernel-space, they map to kmalloc() and kfree().
 *
 * @todo Check what are the proper flags to use with kmalloc().
 * @todo These functions may be made static inline.
 */

/** Allocates a memory segment either in user-space or in kernel-space.
 *
 * In case of no memory available, sets qmgr_err to OML_E_NO_MEMORY
 * and returns 0.
 * 
 * @note
 * Another possible implementation of allocation function can be:
 * 
 * @code
 *   #define oml_malloc(x) do { x = xalloc(sizeof (*(x))); } while (0)
 *   #endif
 * 
 * Usage:
 *   my_obj * new_obj;
 *   oml_malloc(new_obj);
 * @endcode
 */
void *oml_malloc(long size);

/** Deallocates a memory segment	*/
void oml_free(void *ptr);

/** If memory check enabled, return 1 if all allocated
 * memory chunks have been freed, and no chunk-related
 * errors occurred ever during oml_malloc() / oml_free()
 */
int oml_mem_clean(void);

#endif
