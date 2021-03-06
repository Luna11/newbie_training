/*
* File name
*	new.c
*
* Author/Created date
*	Luna Wang, Aug15'14
*
* Copyright (C) 2014 Wistron 
*	All rights reserved.
*
* Description
*	This file is a linklist function exercise source file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* create employee struct */
struct employee{
	unsigned char ID[10];
	unsigned char name[20];
	unsigned char sex[10];
	unsigned int age;
};

/* create link node */
typedef struct employee_node{
	struct employee worker;
	struct employee_node *next;
} emp_node, *emp_link;

static int n=0;     /* declarate a globle variable to record the number of nodes in the link, note: without head node */

/* employee info input function */
static int emp_input(struct employee *emp_info)
{
	printf("Please input employee info as: ID name sex age \n");
	scanf("%s %s %s %d", emp_info->ID, emp_info->name, emp_info->sex, &emp_info->age);
	printf("input ok: %s %s %s %d\n", emp_info->ID, emp_info->name, emp_info->sex, emp_info->age);
	return 0;
}

/* create link */
static emp_link emp_create(void)
{
	char str[5];
	printf("Create employee list.\n");

	/* create head node  */
	emp_link head=(emp_link)malloc(sizeof(emp_node)); 
	if(!head){
		printf("memory locate failed!!\n");
		return NULL;
	}
	head->next=NULL;

	emp_link pre=head, new;   /* declarate pre and new pointer, pre point to head */

	do{
		memset(str, 0, sizeof(str));   /*  clean the string str  */
		
		/* create new node */
		new=(emp_link)malloc(sizeof(emp_node));
		if(!new){
			printf("malloc fail!!\n");
			return NULL;
		}

		new->next=NULL;             /* new node locate to link rear */
		pre->next=new;              /* pre point to new node */
		emp_input(&new->worker);    /* input employee info to this new node */
		pre=new;                    /* pre pointer move backword */

		n++;
		printf("Now there are %d members in the list.\n", n);
		printf("Do you want to continue?(y/n)\n");
		scanf("%s", str);
//		scanf("%*[^\n]");
//		scanf("%*c");

	}while(!strcmp(str, "y"));

	return head;
}

static int output(emp_link emp_head)
{
	int i=1;
	emp_link temp=emp_head->next;

	if(!temp){
		printf("The link is empty.\n");
		return 0;
	}

	printf("The employee info as follow:\n");

	/* traversal link */
	while(temp){
		printf("%d: %s\t%s\t%s\t%d\t\n", i, temp->worker.ID, temp->worker.name, temp->worker.sex, temp->worker.age);
		i++;
		temp=temp->next ;           /* temp pointer move backword */
	}

	return 0;
}

/* insert a new node at the m location */
static emp_link emp_insert(emp_link emp_head, int m)
{
	int i=0;
	emp_link temp;

	if((m>(n+1))||(m<1)){
		printf("Your input %d is invalid.\n", m);
		return NULL;
	} 
	
	if(m==(n+1)){
		temp=emp_head;
		while(temp->next){
			temp=temp->next;
		}
		
		emp_link new=(emp_link)malloc(sizeof(emp_node));
		if(!new){
			printf("malloc fail!!\n");
			return NULL;
		}

		temp->next=new;
		new->next=NULL;
		emp_input(&new->worker);

		n++;
		return new;
	} else {
		temp=emp_head;
		while(i!=(m-1)){
			temp=temp->next;
			i++;
		}

		printf("n=%d, i=%d, m=%d\n",n, i, m);

		/* i==(n-1) */
		emp_link pre, cur, new;
		pre=temp;
		cur=pre->next;

		new=(emp_link)malloc(sizeof(emp_node));
		if(!new){
			printf("malloc fail!!\n");
			return NULL;
		}

		new->next=cur;
		pre->next=new;
		emp_input(&new->worker);
		
		n++;
		return new;
	}
}

/* delete a node which include id */
static emp_link emp_del(emp_link emp_head, char id[10])
{
	int i=0;
//	emp_link temp=emp_head;
	emp_link pre, cur;

	cur=emp_head;
	
	while(strcmp(cur->worker.ID, id) && cur->next){
		pre=cur;
		cur=cur->next;
		i++;
	}

	printf("i=%d\n",i);

	if(!strcmp(cur->worker.ID, id)){
		pre->next=cur->next;
		n--;
		return cur;
	} else {
		printf("There is no ID %s.\n", id);
		return NULL;
	}
}

int main(void)
{
	int m;
	char id[10];

	emp_link h=emp_create();  /* create link */
	
	output(h);                /* traversal and output link */

	/* insert an new node */
	printf("Please input a int number which is the location where you want to insert new employee info:\n");
	scanf("%d", &m);
	emp_link ins=emp_insert(h, m);

	if(ins){
		printf("Insert new info is: %d. %s\t%s\t%s\t%d\t\n", m, ins->worker.ID, ins->worker.name, ins->worker.sex, ins->worker.age);
		output(h);
	}

	/* delete a node */
	printf("Now, delete an employee info:\nPlease input the ID which you want to delete:\n");
	scanf("%s", id);
	emp_link del=emp_del(h, id);
	if(del){
		printf("Delete employee info is: %s\t%s\t%s\t%d\t\n", del->worker.ID, del->worker.name, del->worker.sex, del->worker.age);
		printf("New employee list as follow:");
		output(h);

		free(del);
	}
	return 0;
}
