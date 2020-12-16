/* ID : 2016116256
 * NAME : Kim Dogyun
 * OS : linux, Ubuntu 16.04
 * Compiler version : gcc 5.4.0 20160609
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

	int num[] = {atoi(argv[1]), atoi(argv[2]), atoi(argv[3])};

	int temp;
	for(int i = 0; i < sizeof(num) / sizeof(int); i++){
		for(int j = i+1; j < sizeof(num) / sizeof(int); j++){

			if(num[i] > num[j]){
				temp = num[i];
				num[i] = num[j];
				num[j] = temp;
			}
		}
	}

	
	for(int i = 0; i < sizeof(num)/sizeof(int); i++){
		printf("%d ", num[i]);
	}
	printf("\n");

	return 0;
}
