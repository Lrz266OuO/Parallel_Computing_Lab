#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<math.h>
#include<pthread.h>

int **matrix;
int **temp;
int p;
int N;

void init(){
    int i,j;

    matrix=(int**)malloc(N*sizeof(int*));
    temp=(int**)malloc(N*sizeof(int*));
    for(i=0;i<N;i++){
        matrix[i]=(int*)malloc(N*sizeof(int));
        temp[i]=(int*)malloc(N*sizeof(int));
    }

    int count=0;
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            matrix[i][j]=count;
            temp[i][j]=count;
            count++;
        }
    }
}

void *thread_function(void *ID){
    int id=*(int*)ID;
    int u=id/sqrt((double)p);
    int v=id%((int)sqrt((double)p));
    int m=sqrt((double)(N*N/p));
    int i,j;
    for(i=u*m;i<(u+1)*m;i++){
        for(j=v*m;j<(v+1)*m;j++){
            matrix[i][j]=temp[j][i];
        }
    }
    return NULL;
}

void Free(int n){
    free(matrix);
    free(temp);
}

int main(int argc,char* argv[]){
    p=atoi(argv[1]);
    N=atoi(argv[2]);
    init();
    int x[p];
    pthread_t thread[p];
    int i,j;
    FILE *fp = NULL;
    fp = fopen("matrix_Chessboard.txt","w+");

    for(i=0;i<p;i++){
        x[i]=i;
        pthread_create(&thread[i],NULL,thread_function,&x[i]);
    }
    for(i=0;i<p;i++){
        pthread_join(thread[i],NULL);
    }

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