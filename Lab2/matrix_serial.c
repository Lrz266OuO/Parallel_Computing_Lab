#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>
#include<malloc.h>

int **matrix;
int N;

void serial(int row, int column, int n){
    int i,j;
    int temp;
    for(i=row; i<n; i++){
        for(j=column; j<i; j++){
            temp=matrix[i][j];
            matrix[i][j]=matrix[j][i];
            matrix[j][i]=temp;
        }
    }
}

void init(int n){
    int i,j;

    matrix=(int**)malloc(n*sizeof(int*));
    for(i=0; i<n; i++){
        matrix[i]=(int*)malloc(n*sizeof(int));
    }
    
    int count=0;
    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            matrix[i][j]=count;
            count++;
        }
    }
}

void Free(int n){
    free(matrix);
}

int main(int argc, char *argv[]){
    N=atoi(argv[1]);
    FILE *fp = NULL;
    fp = fopen("matrix_Serial.txt","w+");

    init(N);
    serial(0,0,N);

    int i,j;
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            fprintf(fp, "%d ", matrix[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    Free(N);
    return 0;
}