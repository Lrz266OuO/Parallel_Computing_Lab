#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<pthread.h>
#include<math.h>

int **matrix;
int p;
int N;

void init(){
    int i,j;

    matrix=(int**)malloc(N*sizeof(int*));
    for(i=0;i<N;i++){
        matrix[i]=(int*)malloc(N*sizeof(int));
    }

    int count=0;
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            matrix[i][j]=count;
            count++;
        }
    }
}

void *thread_function(void *ID){
    int id=*(int*)ID;
    int length=N/p;
    int i,j,temp;
    
    for(i=id*length;i<(id+1)*length&&i<N;i++){
        for(j=0;j<i;j++){
            temp=matrix[i][j];
            matrix[i][j]=matrix[j][i];
            matrix[j][i]=temp;
        }
    }
    return NULL;
}

void Free(int n){
    free(matrix);
}

int main(int argc,char* argv[]){
    p=atoi(argv[1]);
    N=atoi(argv[2]);
    init();
    int x[p];
    pthread_t thread[p];
    int i,j;
    FILE *fp = NULL;
    fp = fopen("matrix_RightAngle.txt","w+");

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