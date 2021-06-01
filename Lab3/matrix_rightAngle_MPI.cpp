#include<stdio.h>
#include<malloc.h>
#include<cmath>
#include<cstdlib>
#include<fstream>
#include<iostream>
#include<mpi.h>

using namespace std;

int N;
double t_start, t_end;

void get_ij(int id, int & i, int n){
    i = id*n;
}

int main(int argc, char * argv[]){

    //N = atoi(argv[1]);
    N = 16;
    int matrix[N][N];

    int count=0;
    for(int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            matrix[i][j] = count;
            count++;
        }
    }

    int myid,num,n;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &num);

    n = N/num;

    int my_i;

    get_ij(myid, my_i, n);

    for (int i = my_i; i < (myid+1)*n; i++){
        for (int j = 0; j < i; j++){
            matrix[i][j] = matrix[i][j]^matrix[j][i];
            matrix[j][i] = matrix[i][j]^matrix[j][i];
            matrix[i][j] = matrix[i][j]^matrix[j][i];
        }
    }

    if(myid == 0){

        int temp[num-1][N][N];
        MPI_Status status;
        t_start = MPI_Wtime();

        for (int i = 1; i < num; i++){
            MPI_Recv(&temp[i-1][0][0], N*N, MPI_INT, i, 30, MPI_COMM_WORLD, &status);
        }

        for (int i = 1; i < num; i++){
            int current_i = i*n;
            for (int m = current_i; m < (i+1)*n; m++){
                for (int j = 0; j < m; j++){
                    matrix[m][j] = temp[i-1][m][j];
                    matrix[j][m] = temp[i-1][j][m];
                }
            }
        }

        ofstream outfile;
        outfile.open("matrix_rightAngle_MPI.txt");
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                outfile<<matrix[i][j]<<" ";
            }
            outfile<<endl;
        }
        outfile.close();

        t_end = MPI_Wtime();
        printf("Matrix order:%d, Time cost:%lf\n",N,t_end-t_start);
                
    }
    else{
        MPI_Send(&matrix[0][0], N*N, MPI_INT, 0, 30, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();

    return 0;
}

