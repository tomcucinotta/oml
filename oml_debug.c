#include "oml_debug.h"

static char *oml_errors[] = {
  "Unspecified error",
  "Insufficient memory",
  "Invalid parameter(s)",
  "Unauthorized",
  "Unimplemented",
  "Missing component",
  "Inconsistent state",
  "System overload",
  "Internal error: report to authors, please",
  "Full container",
  "Empty container",
  "Element not found"
};

int oml_log_msg_id = 0;  /**< logging message identifier number */

char *oml_strerror(oml_rv err) {
  int index;
  int errno = (int) err;
  if (err == OML_OK)
    return "Success";

  index = -errno - 16;
  if ( (index < 0) || (index >= (int) (sizeof(oml_errors) / sizeof(*oml_errors))) ) {
    oml_log_err("Invalid index: %d", index);
    return "Bug: unclassified error in oml_types.c";
  }
  return oml_errors[index];
}

#if defined(__KERNEL__)
EXPORT_SYMBOL(oml_strerror);
EXPORT_SYMBOL(oml_log_msg_id);
#endif
