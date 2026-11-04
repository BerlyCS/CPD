#include <omp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
  int data;
  struct node *next;
} node;

node *create_node(int data) {
  node *n = (node *)malloc(sizeof(node));
  n->data = data;
  n->next = NULL;
  return n;
}

node *insert_start(node *head, int data) {
  node *n = create_node(data);
  n->next = head;
  return n;
}

node *insert_node(node *head, int data) {
  node *n = create_node(data);
  if (!head)
    return n;
  node *c = head;
  while (c->next)
    c = c->next;
  c->next = n;
  return head;
}

node *delete_node(node *head, int data) {
  if (!head)
    return NULL;
  if (head->data == data) {
    node *t = head;
    head = head->next;
    free(t);
    return head;
  }
  node *c = head;
  while (c->next && c->next->data != data)
    c = c->next;
  if (c->next) {
    node *t = c->next;
    c->next = c->next->next;
    free(t);
  }
  return head;
}

node *search_node(node *head, int data) {
  node *c = head;
  while (c) {
    if (c->data == data)
      return c;
    c = c->next;
  }
  return NULL;
}

node *print_list(node *head) {
  node *c = head;
  while (c) {
    printf("%d -> ", c->data);
    c = c->next;
  }
  printf("NULL\n");
  return head;
}

int list_length(node *head) {
  int n = 0;
  node *c = head;
  while (c) {
    n++;
    c = c->next;
  }
  return n;
}

void free_list(node *head) {
  while (head) {
    node *t = head;
    head = head->next;
    free(t);
  }
}

// implementacion openmp
typedef struct {
  node dummy;
} plist_t;

void plist_init(plist_t *L) {
  L->dummy.data = -1;
  L->dummy.next = NULL;
}

void plist_free(plist_t *L) {
  free_list(L->dummy.next);
  L->dummy.next = NULL;
}

void plist_insert(plist_t *L, int data) {
  node *n = create_node(data);
  n->next = L->dummy.next;
  L->dummy.next = n;
}

node *plist_search(plist_t *L, int data) {
  node *c = L->dummy.next;
  while (c) {
    if (c->data == data)
      return c;
    c = c->next;
  }
  return NULL;
}

int plist_delete(plist_t *L, int data) {
  node *prev = &L->dummy;
  node *c = L->dummy.next;
  while (c && c->data != data) {
    prev = c;
    c = c->next;
  }
  if (c) {
    prev->next = c->next;
    free(c);
    return 1;
  }
  return 0;
}

int plist_count(plist_t *L) {
  int n = 0;
  node *c = L->dummy.next;
  while (c) {
    n++;
    c = c->next;
  }
  return n;
}


typedef struct {
  int is_delete;
  int value;
} op_t;

int main(void) {
  printf("Secuencial\n");

  node *head = NULL;
  int data[] = {4, 1, 3, 2, 5, 6, 7, 8, 9, 10};
  for (int i = 0; i < 10; i++)
    head = insert_node(head, data[i]);
  printf("Lista inicial:  ");
  print_list(head);

  int del_i[] = {5, 1, 7, 2};
  for (int i = 0; i < 4; i++) {
    head = delete_node(head, del_i[i]);
    printf("Tras del %2d:  ", del_i[i]);
    print_list(head);
  }
  printf("Longitud final: %d\n", list_length(head));
  printf("Buscar 3: %s\n", search_node(head, 3) ? "presente" : "ausente");
  printf("Buscar 5: %s\n", search_node(head, 5) ? "presente" : "ausente");
  free_list(head);

  /* Paralelo */

  printf("Threads disponibles: %d\n", omp_get_max_threads());

  return 0;
}
