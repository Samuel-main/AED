#include <stdio.h> 
#include <stdlib.h>

int main (){
       int **matrizquantidade = (int**)malloc( 3* sizeof(int *)); 
       for(int i = 0;i < 3;i++){
        matrizquantidade[i] = (int*)malloc(3*sizeof(int));
       }
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            scanf("%d", &matrizquantidade[i][j]);
        }
    }
    for(int i = 0; i < 3; i++){
        printf("\n");
        for(int j = 0; j < 3; j++){
            printf("%d", matrizquantidade[i][j]);
        }
    }
    printf("\n");
    for(int i = 0; i < 3;i++){
        free(matrizquantidade[i]);
    }
    return 0;
}
