#include "customer.h"
#include "customer_hash.h"
#include <stdlib.h>

customer_hash* create_hash_table(int num_of_buckets){
    customer_hash* my_table = NULL;

    my_table->buckets = malloc(sizeof(customer*) * num_of_buckets);
    my_table->num_of_buckets = num_of_buckets;

    return my_table;
}


int hash_by_ID(customer_hash* table, int id){
    if (table==NULL) return -1;

    int buckets = table->num_of_buckets;

    int bucket = id % buckets;

    return bucket;
}

int hash_by_name(customer_hash* table, char* name){
    if (table==NULL || name==NULL) return -1;

    int buckets = table->num_of_buckets;

    char first_letter = name[0];
    int bucket = first_letter - 'A';
    bucket = bucket % buckets;

    return bucket;
}

int add_customer_to_tbl(customer_hash* table, int id, char* name){
    if (table==NULL || name==NULL) return -1;

    int bucket = hash_by_ID(table, id);
    if (bucket < 0 || bucket > table->num_of_buckets) return -2;

    customer* head_ptr = add_customer(table->buckets[bucket], id, name);
    if (head_ptr==NULL) return -3;

    table->buckets[bucket] = head_ptr;
    return 0;
}