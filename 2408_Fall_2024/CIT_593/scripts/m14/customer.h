typedef struct cust_struct{
    int id;
    char* name;
    struct cust_struct* next;
} customer;

/* Helper Function Definition! */
customer* add_customer(customer* list, int id, char* name);