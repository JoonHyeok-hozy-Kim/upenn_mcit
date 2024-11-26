#include "customer.h"

typedef struct{
    int num_of_buckets;
    customer** buckets;
} customer_hash;

customer_hash* create_hash_table(int num_of_buckets);

int hash_by_ID(customer_hash* table, int id);
int hash_by_name(customer_hash* table, char* name);

int add_customer_to_tbl(customer_hash* table, int id, char* name)