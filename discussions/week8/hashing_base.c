#include <stdio.h>
#include <stdlib.h>
 
#define MAX 10
 
void main() 
{ 
	int a[MAX],num,key,i;
	char ans;
	int create(int);
	void linear_prob(int [],int,int),display(int []);
	printf("\n Collision Handling By Linaer Probling");
 
	for(i=0;i<MAX;i++)
		a[i]=-1;
 
	do
	{
		printf("\n Enter the Number ");
		scanf("%d",&num);
		key=create(num);
		linear_prob(a,key,num);
		printf("\n Do U Wish to Contiue?(Y/N)");
		scanf(" %c",&ans);
	} while(ans=='y');
 
	display(a);
 
	getchar();
}
 
int create(int num)
{ 
	int key;
	key=num%10;
	return key;
}
 
void linear_prob(int a[MAX],int key,int num)
{ 

}
 
void display(int a[MAX])
{ 
	int i;
	printf("\n\n The HAsh Table is....\n");
	for(i=0;i<MAX;i++)
		printf("\n %d %d",i,a[i]);
 }