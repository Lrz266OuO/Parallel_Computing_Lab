#include<stdio.h>
#include<malloc.h>
#include<cmath>
#include<cstdlib>
#include<fstream>
#include<iostream>
#include<mpi.h>
using namespace std;
int **matrix, **out, *tmp;
int n;
double t_start, t_end;
MPI_Status status;

void init(){
	matrix = (int **)malloc(n * sizeof(int *));
	out = (int **)malloc(n * sizeof(int *));
    for(int i=0;i<n;i++){
        matrix[i] = (int *)malloc(n * sizeof(int));
        out[i] = (int *)malloc(n * sizeof(int));
    }
}

void getValue(){
	int cnt = 0;
    for(int i=0;i<n;i++){
    	for(int j=0;j<n;j++){
    		matrix[i][j] = out[i][j] = cnt;
    		cnt++;
		}
	}
}

int main(int argc, char *argv[]){
	//矩阵维数 
	//n = atoi(argv[1]);
    n = 16;

	int my_rank,group_size;
	int sqrt_group_size,u,v,length;
    //进程数开根号即每行或每列的子块数，列号，行号，每个块的维数 
	//init之前不是简单的想象中并行，这里对于每个进程而言都是串行执行了一下 
	//开始并行程序 
	MPI_Init(&argc, &argv);
	//获取进程数 
	MPI_Comm_size(MPI_COMM_WORLD,&group_size);
	//获取PID 
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

	sqrt_group_size = sqrt(group_size);
	u = my_rank%sqrt_group_size;
	v = my_rank/sqrt_group_size;
	length = n/sqrt_group_size;

	tmp = (int *)malloc(sizeof(int)*length*length);

	if(my_rank == 0){
		//0号进程对矩阵初始化，不知道为什么放在并行化外初始化会段错误
		init();
		getValue();

   		t_start = MPI_Wtime();
		//这两层循环是找到对应编号的子块

		for(int i=0;i<sqrt_group_size;i++){
			for(int j=0;j<sqrt_group_size;j++){

				//这里该确定每一个子块中的元素对应在matrix里的位置了 
				//貌似传递的数据需存在一维数组中，二维可能存在内存不连续的状况
				int point = 0;
				for(int k=length*i;k<length*(i+1);k++){
					for(int l=length*j;l<length*(j+1);l++){
						tmp[point] = matrix[k][l];
						point++;
					}
				}

				//0号子块由0号进程自己转置
				if(i==0 && j==0){
					for(int i=0;i<length;i++){
			            for(int j=0;j<length;j++){
			                out[i][j] = matrix[j][i];
			            }
			        }
				}

				//其他子块送到其他进程中去转置 
				else{
					MPI_Send(tmp,length*length,MPI_INT,i*sqrt_group_size+j,i*sqrt_group_size+j,MPI_COMM_WORLD);
				}
			}
		}

    }
    else{
    	
    	MPI_Recv(tmp,length*length,MPI_INT,0,my_rank,MPI_COMM_WORLD,&status);
    	
    	//内部转置 
    	int t;
        for(int i=0;i<length;i++){
            for(int j=i+1;j<length;j++){
                t = tmp[i*length+j];
                tmp[i*length+j] = tmp[j*length+i];
                tmp[j*length+i] = t;
            }
        }
        MPI_Send(tmp,length*length,MPI_INT,0,my_rank,MPI_COMM_WORLD);
    }
    
    if(my_rank==0){

    	//还是先找每一个子块对应的进程 
        for(int i=0;i<sqrt_group_size;i++){
            for(int j=0;j<sqrt_group_size;j++){
            	if(i!=0 || j!=0){
            		
	                MPI_Recv(tmp,length*length,MPI_INT,i*sqrt_group_size+j,i*sqrt_group_size+j,MPI_COMM_WORLD,&status);
	                
					for(int x=0;x<length;x++){
	                    for(int y=0;y<length;y++){
	                    	//要交换的子块进行交换 
	                        out[j*length+x][i*length+y] = tmp[x*length+y];
	                    }
	                }
            	}
            }
        }

        ofstream outfile;
        outfile.open("matrix_chessboard_MPI.txt");
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                outfile<<out[i][j]<<" ";
            }
            outfile<<endl;
        }
        outfile.close();

        t_end = MPI_Wtime();
        printf("Matrix order:%d, Time cost:%lf\n",n,t_end-t_start);
        free(matrix);
    }
    free(tmp);
	MPI_Finalize();
	return 0;
}

