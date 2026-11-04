#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

typedef struct node{
  int data;
  struct node *next;
} node;

node *create_node(int data) {
  node *new_node = (node *)malloc(sizeof(node));
  new_node->data = data;
  new_node->next = NULL;
  return new_node;
}

node *insert_start(node *head, int data) {
  node *new_node = create_node(data);
  new_node->next = head;
  return new_node;
}

node *insert_node(node *head, int data) {
  node *new_node = create_node(data);
  if (head == NULL) {
    return new_node;
  }
  node *current = head;
  while (current->next != NULL) {
    current = current->next;
  }
  current->next = new_node;
  return head;
}

node *delete_node(node *head, int data) {
  if (head == NULL) {
    return NULL;
  }
  if (head->data == data) {
    node *temp = head;
    head = head->next;
    free(temp);
    return head;
  }
  node *current = head;
  while (current->next != NULL && current->next->data != data) {
    current = current->next;
  }
  if (current->next != NULL) {
    node *temp = current->next;
    current->next = current->next->next;
    free(temp);
  }
  return head;
}

node *search_node(node *head, int data) {
  node *current = head;
  while (current != NULL) {
    if (current->data == data) {
      return current;
    }
    current = current->next;
  }
  return NULL;
}

node* print_list(node* head) {
  node* current = head;
  while (current != NULL) {
    printf("%d -> ", current->data);
    current = current->next;
  }
  printf("NULL\n");
  return head;
}

int main() {
  node* head = NULL;
  int data[] = {4, 1, 3, 2, 5, 6, 7, 8, 9, 10};
  for (int i = 0; i < 10; i++) {
    head = insert_node(head, data[i]);
  }

  print_list(head);

  int del_i[] = {5,1,7,2}; 

  for (int i = 0; i < 4; i++) {
    head = delete_node(head, del_i[i]);
    print_list(head);
  }

  print_list(head);

}
