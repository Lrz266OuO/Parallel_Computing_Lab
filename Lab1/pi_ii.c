/*
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#define n 1000000000

int main(){
    int i,count=0;
    double x,y,pi;
    srand((int)time(NULL));
    for(i=0; i<n; i++){
        x=(double)(1.0*rand()/RAND_MAX);
        y=(double)(1.0*rand()/RAND_MAX);
        if(pow(x,2)+pow(y,2)<=1) count+=1;
    }

    pi=4.0*count/n;
    printf("%0.8f\n",pi);

    return 0;
}
*/

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<time.h>
#include<pthread.h>

double pi=0.0;
int N=500000000;
int t;
int length;
double result[100];
pthread_mutex_t mutex;

void *thread_function(void *ID){
    int id=*(int*)ID;
    int i;
    double x,y;
    srand((int)time(NULL));
    
    for(i=0; i<length; i++){
        x=1.0*rand()/RAND_MAX;
        y=1.0*rand()/RAND_MAX;
        if(x*x+y*y<=1.0) result[id]++;
    }

    pthread_mutex_lock(&mutex);
    pi+=result[id];
    pthread_mutex_unlock(&mutex);

}

int main(int argc, char *argv[]){
    int p;
    p=atoi(argv[1]);
    length=N/p;
    int i;
    int x[p];
    
    pthread_t *threads;
    threads=(pthread_t*)malloc(sizeof(pthread_t)*p);

    for(i=0; i<p; i++){
        x[i]=i;
        pthread_create(threads+i, NULL, thread_function, &x[i]);
    }

    for(i=0; i<p; i++){
        pthread_join(threads[i], NULL);
    }

    pi=4.0*pi/N;
    printf("PI = %0.8f\n", pi);
    free(threads);
    return 0;
}