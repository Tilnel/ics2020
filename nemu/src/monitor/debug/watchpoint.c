#include "watchpoint.h"
#include "expr.h"
#include <stdlib.h>

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

WP* new_wp(char *arg){
  if (free_ == NULL) assert(0);
  WP *tmp, *tmp2;
  tmp = free_;
  free_ = free_ -> next;
  tmp2 = head;
  head = tmp;
  head -> next = tmp2;
  head -> EXPR = (char*) malloc(sizeof(char) * strlen(arg));
  strcpy (head -> EXPR, arg);
  bool flag = true;
  head -> value = expr(arg, &flag);
  head -> checked = true;
  return head;
}

void free_wp(int id) {
  WP *pt = head;
  while(pt != NULL){
    if(pt -> NO == id) break;
    pt = pt -> next;
  }
  if (pt == NULL) {
    printf("Can't find this watchpoint!\n");
    return;
  }
  free(wp_pool[id].EXPR);
  pt = head;
  if (head == &wp_pool[id]) {
    head = head -> next;
  } else {
	  
    while (pt -> next != &wp_pool[id]) {
      pt = pt -> next;
    }
    assert(pt -> next != NULL);
    pt -> next = wp_pool[id].next;
  }

  pt = free_;
  if (pt -> NO > id) {
    wp_pool[id].next = pt;
    free_ = &wp_pool[id];
  } else {
    while (pt -> next -> NO < id){
      pt = pt -> next;
    }

    wp_pool[id].next = pt -> next;
    pt -> next = &wp_pool[id];
  }
  
  return;
}

void init_wp_pool() {
  int i;
  
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

void wp_display(){
  if (head == NULL) {
    printf("No Watchpoints!\n");
    return;
  }

  WP *pt = head;
  printf("Num	Type			What\n");
  while (pt != NULL) {
    printf("%d	hw watchpoint		%s\n", pt -> NO, pt -> EXPR);
    pt = pt -> next;
  }
  return;
}

void wp_check() {
  bool check[NR_WP];
  word_t val;
  for (int i = 0;i < NR_WP;i ++) {
    check[i] = false;
  }
  bool flag = true;
  WP *pt = head;
  while (pt != NULL) {
    val = expr(pt -> EXPR, &flag);
    if (pt -> checked && pt -> value != val) {
      printf("Watchpoint %d: %s\n", pt -> NO, pt -> EXPR);
      printf("Old value: %u\n", pt -> value);
      printf("New value: %u\n", val);
    }
    pt -> value = val;
    check[pt -> NO] = true;
    pt = pt -> next;
  }

  for (int i = 0;i < NR_WP;i ++) {
    wp_pool[i].checked = check[i];
  }
  
  return;
}
