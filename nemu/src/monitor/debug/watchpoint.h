#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char *EXPR;
  bool checked; 
  word_t value;
  /* TODO: Add more members if necessary */

} WP;

void wp_display();

WP* new_wp();

void free_wp(int id);

bool wp_check();
#endif
