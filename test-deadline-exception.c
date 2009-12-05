#include <oml_exceptions.h>
#include <stdio.h>
#include <time.h>

oml_define_exception(ENotReady) oml_extends(oml_exception);

void too_long_func() {
  for (;;)
    ;
  oml_throw(ENotReady);
}

int main() {
  struct timespec dl, wcet;
  clock_gettime(CLOCK_MONOTONIC, &dl);
  dl.tv_sec += 2;
  printf("Starting deadline-bounded region for 2s\n");
  oml_try_within_abs(dl) {
    too_long_func();
  }
  oml_handle
    oml_when (oml_ex_deadline_violation) {
      printf("Absolute DeadlineViolation caught !\n");
    }
    oml_when (oml_ex_time_constr_violation) {
      printf("TimeConstrViolation caught !\n");
    }
    oml_when (oml_exception) {
      printf("Exception caught !\n");
    }
  oml_end;

  wcet.tv_sec = 2;
  wcet.tv_nsec = 0;
  printf("Starting deadline-bounded region for 2s\n");
  oml_try_within_rel(wcet) {
    too_long_func();
  }
  oml_handle
    oml_when (oml_ex_deadline_violation) {
      printf("Relative DeadlineViolation caught !\n");
    }
    oml_when (oml_ex_time_constr_violation) {
      printf("TimeConstrViolation caught !\n");
    }
    oml_when (oml_exception) {
      printf("Exception caught !\n");
    }
  oml_end;

  return 0;
}
