#include "customer_hash.h"
#include <stdlib.h>

int main(){
    customer_hash* my_table = NULL;
    char name[100];
    int id;
    int buckets;
    int add_result = -1;

    printf("How many buckets should we have?\n");
    scanf("%d", &buckets);
    my_table = create_hash_table(buckets);

    int match = 2; // To check if the valid input was given.
    while (match == 2){
        printf("Enter a customer name and ID\n");
        match = scanf("%s %d", name, &id);

        add_result = add_customer_to_tbl(my_table, id, name)
        if (add_result != 0){
            printf("error adding customer with error code %d", add_result);
            return -1;
        }
    }

    return 0;
}