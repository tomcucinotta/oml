/*
 * This file is part of Open Macro Library (OML) - http://oml.sourceforge.net/
 *
 * OML is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * OML is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with Fluid. If not, see
 * <http://www.gnu.org/licenses/>.
 */ 

//#define OML_WITHIN_BENCH
#include <oml_exceptions.h>
#include <stdio.h>
#include <time.h>

oml_define_exception(ENotReady) oml_extends(oml_exception);

void f() {
  for (;;)
    ;
  oml_throw(ENotReady);
}

int main() {
  struct timespec dl;
  clock_gettime(CLOCK_MONOTONIC, &dl);
  dl.tv_sec += 2;
  printf("Starting outer deadline-bounded region for 2s\n");  fflush(stdout);
  oml_try_within_abs(dl) {
    printf("Starting inner deadline-bounded region for 1s\n");  fflush(stdout);
    struct timespec dl2 = { .tv_sec = 1, .tv_nsec = 0 };
    oml_try_within_rel(dl2) {
      f();	/**< First, induce a Deadline violation */
    }
    oml_handle
      oml_when (oml_ex_time_constr_violation) {
        printf("Inner Time Constr Violation caught !\n");
      }
    oml_end;
    f();	/**< Then, induce another Deadline violation */
  }
  oml_handle
    oml_when (oml_ex_deadline_violation) {
      printf("Outer Absolute DeadlineViolation caught !\n");
    }
    oml_when (oml_ex_time_constr_violation) {
      printf("Outer TimeConstrViolation caught !\n");
    }
    oml_when (oml_exception) {
      printf("Outer Exception caught !\n");
    }
  oml_end;

  dl.tv_sec = 2;
  dl.tv_nsec = 0;
  printf("Starting outer deadline-bounded region for 2s\n");
  oml_try_within_rel(dl) {
    printf("Starting inner deadline-bounded region for 4s\n");  fflush(stdout);
    struct timespec dl2 = { .tv_sec = 4, .tv_nsec = 0 };
    oml_try_within_rel(dl2) {
      f();	/**< First, induce the outer Deadline violation */
    }
    oml_handle
      oml_when (oml_ex_time_constr_violation) {
        printf("Inner Time Constr Violation caught !\n");
      }
    oml_end;
    f();
  }
  oml_handle
    oml_when (oml_ex_deadline_violation) {
      printf("Outer Relative DeadlineViolation caught !\n");
    }
    oml_when (oml_ex_time_constr_violation) {
      printf("Outer TimeConstrViolation caught !\n");
    }
    oml_when (oml_exception) {
      printf("Outer Exception caught !\n");
    }
  oml_end;

  return 0;
}
