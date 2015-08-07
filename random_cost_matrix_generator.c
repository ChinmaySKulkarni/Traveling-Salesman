#include<stdio.h>
#include<stdlib.h>	
#define MAX 100
int main()
{
  int n,i,j,**cost;
  FILE *fp = fopen("costs.txt","w");						//costs.txt contains the cost matrix.
  printf("\t\t\tThis program generates a random cost matrix for the Travelling Salesman Program.\n\n");
  printf("Enter the number of cities to be travelled to:\n");
  scanf("%d",&n);
  srand(time(0));								//Seeding to ensure that we get a different sequence of random 
  cost = (int**)malloc(sizeof(int*)*n);						//numbers every time we run this program.
  for(i = 0;i < n;i++)
    cost[i] = (int*)malloc(sizeof(int)*n);
  for(i = 0;i < n;i++)
  {
    for(j = 0;j < n;j++)  
    {
      cost[i][j] = (rand() % MAX) + 1;						//Cost between two cities cannot be 0.
      if(i == j)
        cost[i][j] = 0;								//Diagonal entries are made 0.
    }
  }
  for(i = 1;i < n;i++)
  {
    for(j = 0;j < i;j++)  
      cost[i][j] = cost[j][i];							//To make it a symmetric matrix.
  }
  printf("\nThe randomly generated cost matrix for %d cities is:\n\n",n);
  for(i = 0;i < n;i++)
  {
    for(j = 0;j < n;j++)    
      printf("%d\t",cost[i][j]);
    printf("\n");
  }
  for(i = 0;i < n;i++)				
    for(j = 0;j < n;j++)  
      fprintf(fp,"%d\n",cost[i][j]);						//Write the matrix to the file specified.
  fclose(fp);
  printf("\nNow compile and run the program for the Travelling Salesman Problem.\n\n");
}
