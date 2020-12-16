/* ID : 2016116256
 * NAME : Kim Dogyun
 * OS : linux, Ubuntu 16.04
 * Compiler version : gcc 5.4.0 20160609
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct student{
	int id;
	char name[128], major[128];
};

int main(void){
	struct student *myself;
	myself = (struct student*)malloc(sizeof(struct student));

	myself->id = 2016116256;
	strcpy(myself->name, "Kim Dogyun");
	strcpy(myself->major, "Electronics Engineering");


	printf("ID : %d\n", myself->id);
	printf("NAME : %s\n", myself->name);
	printf("MAJOR : %s\n", myself->major);
}
