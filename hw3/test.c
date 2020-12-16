#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    
    char* C = "asdf.zxcv.asdf.qewr.dsf.zxvc.adsf.qewr.";
    char* B = (char*)malloc(sizeof(char) * strlen(C));
    
    strcpy(B, C);

    for(int i = 0; i < strlen(B); i++){
        printf("%p = %c\n", B + (sizeof(char) * i), *(B + (sizeof(char) * i)));
    }
    
    strcpy(B, "");

    printf("B = %s\n", B);

    strcpy(B , "a");
    
    strcpy(B + sizeof(char) , "b");
    strcpy(B + 2, "C");
    strcpy(B + 3, "Dasdf");
    
    for(int i = 0; i < strlen(B); i++){
        printf("%d : %p = %c\n", i, B + (sizeof(char) * i), *(B + (sizeof(char) * i)));
    }
}