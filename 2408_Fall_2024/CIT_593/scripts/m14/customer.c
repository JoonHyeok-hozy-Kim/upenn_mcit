#include "customer.h" // Import customer and helper functions
#include <stdlib.h>

customer* add_customer(customer* list, int id, char* name){
  customer* new_customer = malloc(sizeof(customer));
  new_customer->id = id;
  new_customer->name = malloc(sizeof(name) + 1);
  strcpy(new_customer->name, name);
  new_customer->next = NULL;

  customer* header = list;
  while (list->next != NULL) { list = list->next; }
  list->next = new_customer;
  return header;
}

int main(){
  customer* customer_list = NULL;

  customer_list = add_customer(customer_list, 1, "Tom");
  customer_list = add_customer(customer_list, 2, "Bob");
}