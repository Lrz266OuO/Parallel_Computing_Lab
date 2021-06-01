/*
#include<stdio.h>
#define n 1000000000

int main(){
    double pi,temp=0.0;
    int i;
    
    for(i=1;i<=n;i++){
        if(i%2==1) temp+=1.0/(i*2-1);
        else temp-=1.0/(i*2-1); 
    }

    pi=4.0*temp;
    printf("%0.8f\n",pi);
    return 0;
}
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<malloc.h>

int N=500000000;
double result[100];
double pi=0.0;
int length;

pthread_mutex_t mutex;

void *thread(void *ID){
    int id=*(int*)ID;
    int i;
    for(i=id*length; i<(id+1)*length && i<N; i++){
        if(i%2==0){
            result[id]+=(1.0/(i*2+1));
        }
        else{
            result[id]-=(1.0/(i*2+1));
        }
    }

    pthread_mutex_lock(&mutex);
    pi+=result[id];
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[]){
    int p;
    p=atoi(argv[1]);
    length=N/p;
    int x[p];
    int i;
    
    pthread_t *threads;
    threads=(pthread_t*)malloc(sizeof(pthread_t)*p);

    for(i=0; i<p; i++){
        x[i]=i;
        pthread_create(threads+i, NULL, thread, &x[i]);
    }

    for(i=0; i<p; i++){
        pthread_join(threads[i], NULL);
    }

    pi=pi*4.0;
    printf("PI = %0.8f\n", pi);
    
    free(threads);
    return 0;
}