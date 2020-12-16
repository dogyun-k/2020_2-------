/* ID: 2016116256
* NAME: Kim dogyun
* OS: linux, Ubuntu 16.04 
* Compiler version: gcc 5.4.0 20160609
*/

#include <stdio.h>
#include <stdlib.h>	// atoi, malloc, free
#include <string.h>	// string library
#include <stdint.h>
#include <math.h>
#define MEASURE_TIME	// to measure execution time
#define MEASURE_MEMORY	// to measure required memory

#define min(a, b) (a < b)? a : b
#define max(a, b) (a > b)? a : b
#define MAX_LEN 256
#define INF 99

double distance(int x1, int y1, int x2, int y2);
void show_table(float** array, int map_size);
void show_position(int** array, int map_size);
void quick_sort(int** array, int start, int end);


/////////////////////////////////////////////////////////////////////
// to compute execution time in seconds
/////////////////////////////////////////////////////////////////////
#ifdef MEASURE_TIME
	#include<time.h>
#endif

/////////////////////////////////////////////////////////////////////
// to compute used memory
/////////////////////////////////////////////////////////////////////
#ifdef MEASURE_MEMORY
	static size_t used_memory = 0;
	void *malloc_c(size_t size)
	{
		if ( size > 0 ) {
			// increase the required memory count
			used_memory += size;
			return malloc(size);
		}
		else return NULL;
	}

	// create a duplicate word with counting bytes
	char *strdup_c(const char *s) {
		int size;
		size = strlen(s)+1;   // including last null character
		used_memory += size;
		return strdup(s);
	}

#else
	void *malloc_c(size_t size) { return malloc(size); }
	char *strdup_c(const char *s) { return strdup(s); }
#endif


void show_table(float** arr, int map_size){
	// show table
	int i, j;

	printf("\n\n도착->");
	for(i = 0; i < map_size; i++){
		printf("%6d", i);
	}
	printf("\n");
	for(i = 0; i < map_size; i++){
		printf("%3d | ", i);
		for(j = 0; j < map_size; j++){
			printf("%6.1f", arr[i][j]);
		}
		printf("\n");
	}

}

void show_position(int** arr, int map_size){
	int i;

	printf("%3d | (%3d, %3d)\t0.0\n", 0, arr[0][0], arr[0][1]);
	for(i = 1; i < map_size; i++){
		printf("%3d | (%3d, %3d)\t%.1lf\n", i, arr[i][0], arr[i][1], 
		distance(arr[i][0], arr[i][1], 0, 0));
	}
}

double distance(int x1, int y1, int x2, int y2){
	// return distance (x1, y1) and (x2, y2)
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));

}

void quick_sort(int** arr, int start, int end){
    
	if(start >= end){
        return;
    }

    int pivot = start;
    int i = start + 1;
    int j = end;
    int temp1, temp2;

    while(i <= j){

        while(i <= end && arr[pivot][0] >= arr[i][0]){
            i++;
        }
   
        while(j > start && arr[pivot][0] <= arr[j][0]){
            j--;
        }
		
        if(i > j){
            temp1 = arr[j][0];
			temp2 = arr[j][1];

            arr[j][0] = arr[pivot][0];
			arr[j][1] = arr[pivot][1];

            arr[pivot][0] = temp1;
			arr[pivot][1] = temp2;
        }
        else{
            temp1 = arr[i][0];
			temp2 = arr[i][1];

            arr[i][0] = arr[j][0];
			arr[i][1] = arr[j][1];

            arr[j][0] = temp1;
			arr[j][1] = temp2;
        }
    }

    quick_sort(arr, start, j - 1);
    quick_sort(arr, j + 1, end);
}

int** alloc_position(int x, int y){
	int** array;

	array = malloc_c(sizeof(int*) * x);
	for(int i = 0; i < x; i++){
		array[i] = malloc_c(sizeof(int) * y);
	}

	return array;
}

float** alloc_table(int size){
	float** array;

	array = malloc_c(sizeof(int*) * size);
	for(int i = 0; i < size; i++){
		array[i] = malloc_c(sizeof(int) * size);
	}

	return array;
}

void init_table(float** table, int** position, int size){
	int i, j;

	for(i = 0; i < size; i++){
		for(j = 0; j < size; j++){
			if(i < j){
				table[i][j] = distance(position[i][0], position[i][1], position[j][0], position[j][1]);
			}
			else{
				table[i][j] = INF;
			}
		}
	}

	// printf("\n\n초기 테이블");
	// show_table(table, size);
}

int find_min_index(float* arr, int size){
	float min = INF;
	int result;

	for(int i = 0; i < size; i++){
		if(arr[i] <= min){
			result = i;
			min = arr[i];
		}
	}

	return result;
}

float greedy(float** table, int** position, int size, int fuel){
	float total = 0;
	int start = 0;
	int end;

	while(end != size-1){
		end = find_min_index(table[start], size);
		
		if(fuel - table[start][end] < 0){
			
		}

		printf("(%d, %d) | %.1f\n", position[start][0], position[start][1], table[start][end]);
		total += table[start][end];
		fuel -= table[start][end];
		start = end;
	}


	printf("Total = %.1f\n", total);

	return total;
}

void floyd(float** table, int** position, int size){
	int i, j, k;

	for(i = 1; i < size; i++){
		for(j = 1; j < size; j++){
			for(k = 1; k < size; k++){
				table[j][k] = min(table[j][k], table[j][i] + table[i][k]);
			}
		}
	}
	show_table(table, size);
}

void find_shortest_path(float** table, int** position, int size){

	int index = 0;
	int min_index = 0;

	while(index != size - 1){
		min_index = find_min_index(table[index], size);
		printf("%d(%d, %d) ", min_index, position[min_index][0], position[min_index][1]);
		index = min_index;	
	}
	printf("\n");
}

// float through_num_node(float** table, int size, int* num, int node){
//
// 	int start = 0, end = size-1;
// 	float total = 0;
//
// 	if(num == 0){
// 		return total;
// 	}
// 	else if(num == 1){
// 		total += table[start][node] + table[node][end];
// 	}
// 	else{
//
// 	}
//
// }

// int search(float** table, int** position, int size){
//	
// 	float total = 0;
// 	int start = 0;
// 	int end = size - 1;
//
// 	for(int i = 0; i < size; i++){
//
// 		for(int j = 0; j < i; j++){
//			
// 		}
//
// 	}
//
// 	printf("Total = %.1f\n", total);
// 	return 0;
// }




/////////////////////////////////////////////////////////////
// main function
/////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    char* infile;
	int** position;
	float** table;
    FILE* fp;
    char buffer[MAX_LEN];
	int i, map_size = 0;
	float total = 0;
    int fuel[4] = {250, 500, 950, 1350};

	#ifdef MEASURE_TIME
		clock_t start, end;
		double cpu_time_used;
	#endif
	#ifdef MEASURE_TIME
		start = clock();
	#endif

    // Get argument as file name
	if ( argc != 2 ) {
		fprintf(stderr, "usage: %s [file_name]\n", argv[0]);
		exit(0);
	}
	else {
		infile = argv[1];
	}



	// open the file
    if ( (fp = fopen(infile, "r")) == NULL ) {		// if file can not open
		fprintf(stderr, "cannot open file %s\n",infile);
		return -1;
	}
	else {		// file open!!a
	
        fscanf(fp, "%s", buffer);
        map_size = atoi(buffer);
	
		// 2차원 변수 배열 동적 할당
		position = alloc_position(map_size, 2);
		table = alloc_table(map_size);

		// 변수들 배열에 넣기
		for(i = 0; i < map_size; i++){
			fscanf(fp, "%s", buffer);
			position[i][0] = atoi(buffer);

			fscanf(fp, "%s", buffer);
			position[i][1] = atoi(buffer);
		}

		// 배열 정렬 : x 오름차순
		quick_sort(position, 0, map_size-1);

		init_table(table, position, map_size);
		
		// floyd(table, position, map_size);
		// greedy(table, position, map_size, 10);
		// search(table, position, map_size);
		find_shortest_path(table, position, map_size);

		fclose(fp);
	}


	show_position(position, map_size);

	printf("\n\nTable");
	show_table(table, map_size);


	for(i = 1; i < map_size; i++){
		total += distance(position[i][0], position[i][1], position[i-1][0], position[i-1][1]);
	}
	printf("total = %.1lf\n", total);



	#ifdef MEASURE_TIME
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		fprintf(stdout,"%.5f seconds\n",cpu_time_used);
	#endif
	#ifdef MEASURE_MEMORY
		fprintf(stdout,"%lu bytes ( %.3f x %d )\n", 
			used_memory, (double)used_memory/(double)(map_size), 2 * map_size);
	#endif
	fprintf(stdout, "\n");

	// free
	for(i = 0; i < map_size; i++){
		free(position[i]);
	}
	free(position);
}

