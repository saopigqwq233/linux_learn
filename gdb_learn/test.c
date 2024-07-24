#include<stdio.h>

int main(){
    int arr[4] = {1,2,3,4};
    for(int i = 0;i<sizeof(arr)/sizeof(int);i++){
        printf("%d ",arr[i]);
    }
    printf("\n");
    return 0;
}