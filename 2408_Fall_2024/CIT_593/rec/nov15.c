
void reverse(int* arr, int length){
    int* s = arr;
    int* e = arr;
    int temp;
    for (int i=0; i<length-1; i++, e++)
    while (s<e){
        temp = *s;
        *s = *e;
        *e = temp;
    }
}

int main(){
    int arr[] = {1,2,3,4,5};
}