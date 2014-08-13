#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct employee{
	unsigned char ID[10];
	unsigned char name[20];
	unsigned char sex[10];
	unsigned int age;
};

typedef struct employee_node{
	struct employee worker;
	struct employee_node *next;
} emp_node, *emp_link;

static int emp_input(struct employee *emp_info)
{
	printf("Please input employee info as: ID name sex age \n");
	scanf("%s %s %s %d", emp_info->ID, emp_info->name, emp_info->sex, &emp_info->age);
	printf("input ok: %s %s %s %d\n", emp_info->ID, emp_info->name, emp_info->sex, emp_info->age);
	return 0;
}

static emp_link emp_create(void)
{
	char str[5];
	printf("Create employee list.\n");

	emp_link head=(emp_link)malloc(sizeof(emp_node));
	if(!head){
		printf("memory locate failed!!\n");
		return NULL;
	}
	head->next=NULL;

	emp_link pre=head, new;

	do{
		memset(str, 0, sizeof(str));
		new=(emp_link)malloc(sizeof(emp_node));
		if(!new){
			printf("malloc fail!!\n");
			return NULL;
		}

		new->next=NULL;
		pre->next=new;
		emp_input(&new->worker);
		pre=new;

		printf("Do you want to continue?(y/n)\n");
		scanf("%s", str);
//		scanf("%*[^\n]");
//		scanf("%*c");

	}while(!strcmp(str, "y"));

	return head;
}

static int output(emp_link emp_info)
{
	int i=1;
	emp_link temp=emp_info->next;
	printf("The employee info as follow:\n");

	while(temp){
		printf("%d: %s\t%s\t%s\t%d\t\n", i, temp->worker.ID, temp->worker.name, temp->worker.sex, temp->worker.age);
		i++;
		temp=temp->next ;
	}

	return 0;
}

int main(void)
{
	emp_link h=emp_create();
	output(h);
	return 0;
}
