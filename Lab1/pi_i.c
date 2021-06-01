/*
#include<stdio.h>
#include<math.h>
#define n 1000000000

int main(){
    double pi=0.0;
    int i;
    double temp;

    for(i=0; i<=n; i++){
        temp=4.0/(1.0+pow((i+0.5)/n,2));
        pi+=1.0*temp/n;
    }
    printf("%0.8f\n",pi);

    return 0;
}
*/

/*
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<malloc.h>

int N=1000000000;
double result[100];
double pi=0.0;
int length;

pthread_mutex_t mutex;

void *thread(void *ID){
    int id=*(int*)ID;
    int i;
    for(i=id*length; i<(id+1)*length && i<N; i++){
        result[id]+=4.0/(1 + ((i + 0.5)/N) * ((i + 0.5)/N));
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

    pi/=N;
    printf("PI = %0.8f\n", pi);
    
    free(threads);
    return 0;
}
*/



#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<malloc.h>
int N =500000000;

double result[100];
double PI =0;
long long length;

pthread_mutex_t mutex ;

void *thread(void * ID)
{	
	int id  = *(int*) ID;
	
	
	int i;
	
	
	for (i = id * length;i < (id + 1) * length && i < N ; i++)
	{
	result[id] += 4.0/((1 + ((i + 0.5)/N) * ((i + 0.5)/N)));
	
	}
	
	pthread_mutex_lock (&mutex);
	
    PI += result[id] ;
    pthread_mutex_unlock (&mutex);
    
    
 } 
 
 
 
int main (int argc, char*argv[])
{	 

	int p;
	p = atoi (argv[1]);
	length = N / p ;
	int i;int x[p] ;
	double result=0;

	pthread_t *threads;
    threads = (pthread_t *) malloc (sizeof(pthread_t) * p);
    
	for (i=0; i<p ;i++)
	{	
		x[i] = i;
		pthread_create(threads + i,NULL,thread,&x[i]);
	}
	
	for (i=0; i < p; i++) {
            pthread_join (threads[i], NULL);
        }
        	   	
    PI /= N;
	printf ("PI  = %0.8f \n",PI );
	free (threads);
	return 0;
}
