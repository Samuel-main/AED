#include <stdio.h>
#include <stdlib.h>
//"w" cria arquivo
//"r" escreve no arquivo
//
// int main(void){
//     FILE * arquivo;
//     int c;
//     arquivo = fopen("entrada.txt", "r");
//     if(arquivo == NULL){
//         printf("Não foi possivel criar esse arquivo");
//         exit(1);
//     }
//     else{
//         printf("Arquivo criado. ");
//     }
//     while (!feof(arquivo)){
//     c = fgetc(arquivo);
//     printf("%c",c);
//     }
//     fclose(arquivo);
    


//     return 0;
// }

int main(void){
    Aluno a = {"Samuel", 20192098};
    Aluno b;
    FILE * arquivo = fopen("dados.dat", "rb");
    if(arquivo == NULL){
        exit(1);
    }
    //fwrite(&a, sizeof(Aluno), 1,arquivo);
    fread(&b, sizeof(Aluno),1,arquivo);
    fclose(arquivo);
    printf("Matricula: %d \n", b.mat);
    printf("Nome: %s", b.nome);
    return 0;

}