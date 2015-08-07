#include<stdio.h>
#include<stdlib.h>
#define MIN(a,b) a<b?a:b
int totalCities,startCity;
typedef struct city								//This structure contains information about each city like its 
{										//number,f,g and h values. It also contains a pointer to the 
  int number,f,g,h;								//next city.
  struct city* next;
}city;

typedef struct list								//This structure is used to maintain the openList and the 
{										//closedList. The openList keeps track of all cities where we can
  city* head;									//travel to from the current city (cities not visited yet).
}list;										//The closedList contains all the cities visited, in the correct
										//order.

void init(list* l)								//Initialize the passed list.
{
  l->head = NULL;
}


void append_city(int val, list* l)						//Used to add new cities to the passed list.
{
  city* tmp = (city*)malloc(sizeof(city));
  city* p = (city*)malloc(sizeof(city));
  p->number = val;
  p->f = p->g = p->h = 0;							//f,g,h values are initialized to zero for each appended city.
  p->next = NULL;
  if(l->head == NULL)								//Empty list.
    l->head = p;
  else
  {
    tmp = l->head;
    while(tmp->next)
      tmp = tmp->next;
    tmp->next = p;								//Append the city to the end of the list.
  }
}


void remove_city(int val, list* l)						//Used to remove cities from the openList.
{
  if(l->head == NULL)								//Empty list so cannot remove any city.
    return;
  city* p = (city*)malloc(sizeof(city));
  city* q = (city*)malloc(sizeof(city));
  p = l->head;
  if(l->head->number == val)							//This is if the list contains just one city.
  {
    l->head = l->head->next;
    free(p);
    return;
  }
  while(p->next && val != (p->next)->number)
   p = p->next;
  if(p->next)
  {
    q = p->next;
    p->next = p->next->next;
    free(q);									//Free the memory allocated to the city that is to be removed.
  }
}


void print(list* closedList)							//Used to print the list information to show the final order of
{										//visiting the cities.
  city* p = (city*)malloc(sizeof(city));  
  p = closedList->head;
  printf("A good solution to the given Travelling Salesman Problem by using the A* Algorithm and additional heauristics is:\n\n");
  printf("Final closedList:\n\n");
  while(p)
  {
    printf("%d--->",p->number);  
    p = p->next; 
  }
  printf("%d\n",startCity);    
}


int find_next_city(list* openList)						//Find the city from the openList that is the best choice 	
{										//considering the city that was just appended to the
  city* p = (city*)malloc(sizeof(city));  					//closedList. We do this by the A* Algorithm. 
  p = openList->head;								//The city with the minimum value of 'f' is chosen
  int tmp,cityNumber = p->number,min = p->f;					//as the next best state.
  while(p)
  {
    tmp = min;
    min = MIN(min,p->f);
    if(min != tmp)
      cityNumber = p->number;							//cityNumber of the city with the lowest cost ('f' value).
    p = p->next;
  } 
  printf("\nNext City Number::  %d\n\n",cityNumber);
  return cityNumber;							
} 
  

void calculate(int index,int cost[totalCities][totalCities],list* openList)	//Used to calculate the 'g','h' and 'f' values for each city 
{										//in the openList. 'index' is the number of the current city.
  city* p = (city*)malloc(sizeof(city));  					//The 'g' value is the cost between each city and the 'index'
  p = openList->head;								//The 'h' value is the Heuristic Function that is used in the
  printf("openList w.r.t. current city %d :\n",index);			  	//A* Algorithm. It gives the cost between each city and the 
  while(p)									//goal state which is the starting city (as we have to return).
  {										
      p->g = cost[p->number][index];
      p->h = cost[p->number][startCity];					//NOTE: The startCity is also the Goal State.
      p->f = p->g + p->h;
      printf("City %d : f = %d \t g = %d \t h = %d \n",p->number,p->f,p->g,p->h);
      p = p->next;
  }
}


void travel(int cost[totalCities][totalCities],list* openList, list* closedList)//This function is called recursively to get the entire path of 
{										//traversal between cities.
  int nextCityNumber;
  if(openList->head == NULL)							//This function is called until the openList becomes empty
    return;									//i.e. No more cities left to be travelled to.
  city* p = (city*)malloc(sizeof(city));  
  p = closedList->head;								
  while(p->next)
    p = p->next;								//The current city is the one that is at the end of the 
  calculate(p->number,cost,openList);						//closedList. Note that the path till this city has already
  nextCityNumber = find_next_city(openList);					//been calculated. The cost of the remaining cities in the 
  remove_city(nextCityNumber,openList);						//openList are calculated based on this current city and then 
  append_city(nextCityNumber,closedList);					//the city with the least overall cost (least 'f' value), is
  travel(cost,openList,closedList);						//chosen as the next city to be travelled to.
}										//Next we remove this city from our openList and append it
										//to the closedList. Then recursively call 'travel()'.

int find_start_cities(int cost[totalCities][totalCities])			//This is used to find the cities from which travelling should
{										//begin to other cities. 
  int i,j,tmp,minX = 1,minY = 0,min = cost[1][0],minNext1,minNext2;
  for(i = 1;i < totalCities;i++)						//Check only the lower triangular matrix, as the cost matrix is 
  {										//symmetric.
    for(j = 0;j < i;j++)
    {
        tmp = min;
        min = MIN(min,cost[i][j]);						//Find the entry with the lowest cost.
        if(min != tmp)
        {									//Find the two cities corresponding to this minimum cost.
          minX = i;
          minY = j;
        }									//Now from these two cities we have to choose one to start
    } 										//our traversal. The second city is returned.
  }
  minNext1 = 1000000000;							//A large number.
  for(j = 0;j < totalCities;j++)
  {
    if(j != minY && cost[minX][j] != 0)						//Extra Heuristic information.
      minNext1 = MIN(minNext1,cost[minX][j]);
  }
  minNext2 = 1000000000;
  for(i = 0;i < totalCities;i++)
  {
    if(i != minX && cost[i][minY] != 0)
      minNext2 = MIN(minNext2,cost[i][minY]);
  }
  if(minNext1 < minNext2)							//Out of the two cities corresponding to the lowest cost in the 
  {										//cost matrix, we choose the city that has the lowest cost 
    startCity = minX;								//to some third city, relative to the second city. There is a 
    return minY;								//high probability that while returning to the startCity, 
  }										//the path from this third city is taken, hence improving our 
  startCity = minY; 								//chances of a better solution.
  return minX;
}										
										

int main()
{
  int i,j,secondCity;
  FILE* fp = fopen("costs.txt","r");						//cost.txt contains the costs between all cities.
  printf("\t\t\tThis is a program to solve the Travelling Salesman Problem using the A* Algorithm.\n\n");
  printf("Enter the total number of cities to be visited:\n");
  scanf("%d",&totalCities);
  if(totalCities <= 0)
  {
    printf("Non-positive number entered! Terminating..\n");
    exit(1);
  }
  if(totalCities == 1)
  {
    printf("Enter more than 1 city! Terminating..\n");
    exit(1);
  }
  int cost[totalCities][totalCities];
  list openList, closedList;
  init(&openList);								//Initialize both lists.
  init(&closedList);
  printf("\nScanning the values of the cost matrix.. (Ensure that distances between the same pair of cities are the same!)\n\n");
  for(i = 0;i < totalCities;i++)
    for(j = 0;j < totalCities;j++)
      fscanf(fp,"%d",&cost[i][j]);						//Accept the user-input costs between cities.
  for(i = 0;i < totalCities;i++)				
  {	
    for(j = 0;j < totalCities;j++)  
    {
      if(i == j)								//Check validity of the costs.
      {
        if(cost[i][j] != 0)
        {
          printf("Error in cost matrix! Terminating..\n");
          exit(1);
        }
      }
      if(cost[i][j] != cost[j][i])
      {
        printf("Error in cost matrix! Terminating..\n");
        exit(1);
      }
    }     
  }										//We find which city to start traversal from. Also, the second 
  secondCity = find_start_cities(cost);						//city is returned.
  for(i = 0; i < totalCities;i++)						
  {										//Initially openList contains all the cities, except the first
    if(i != startCity && i != secondCity)					//two cities. These cities are numbered 0 to (totalCities - 1).  
      append_city(i,&openList);							
  }
  append_city(startCity,&closedList);						//Append the first two cities to the closedList.
  append_city(secondCity,&closedList);
  printf("The starting city is:\tCity %d\n\n",startCity);
  printf("The second city is  :\tCity %d\n\n",secondCity);
  travel(cost,&openList,&closedList);
  print(&closedList);
  fclose(fp);
}
