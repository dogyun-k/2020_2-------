/* ID: 2016116256
* NAME: Kim dogyun
* OS: linux, Ubuntu 16.04 
* Compiler version: gcc 5.4.0 20160609
*/

// Reference https://twinw.tistory.com/126

// do not use memcpy, memccpy, memmove, wmemmove, and other direct memory 
// copy/move functions
// these functions performs 'BLOCKED' memory accesses so that large chunk 
// of memory management are efficiently implemented, 
// so they break UNIT TIME assumption in algorithm design
// string functions: strcpy, strncpy, strlen, strcmp are allowed, but not strdup
//
// NOTE: IN GENERAL, COMMENTED STATEMENTS FOLLOWED BY 
// ">>>" WRITTEN IN CAPITAL LETTERS ARE DIRECTIONS
//
// >>> (10/100) pts
// >>> IN THE TOP 4-LINES COMMENTS 
// >>> LINE 1: REPLACE WITH YOUR ID (IF YOU HAVE NON-NUMERIC, IGNORE IT)
// >>> Line 2: REPLACE WITH YOUR NAME (NO HANGUL)
// >>> DO NOT CHANGE OS AND Compiler
// >>> COMPILE AND RUN YOUR CODE ON THE LINUX MACHINE
//
// HOMEWORK PROGRAMMING ASSIGNMENT 
// 5-1 LONGEST COMMON SUBSEQUENCE
// 5-2 LONGEST COMMON SUBSEQUENCE WITH CIRCULAR SHIFT

#include<stdio.h>
#include<stdlib.h>	// atoi, malloc, free
#include<string.h>	// string library
#include<stdint.h>

#define MEASURE_TIME	// to measure execution time
#define MEASURE_MEMORY	// to measure required memory

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

/* FILL */
/* add any necessary functions, if needed */
// void print_table(int** table, int x, int y){
// 	int i, j;
//
// 	printf("\n     ");
// 	for(i = 0; i < y; i++){
// 		printf("y ");
// 	}
// 	printf("\n     ");
// 	for(i = 0; i < y; i++){
// 		printf("%d ", i);
// 	}
// 	printf("\n    ");
// 	for(i = 0; i < y; i++){
// 		printf("--");
// 	}
// 	printf("\n");
// 	for(i = 0; i < x; i++){
// 		printf("x %d| ", i);
// 		for(j = 0; j < y; j++){
// 			printf("%d ", table[i][j]);
// 		}
// 		printf("\n");
// 	}
// }


void circular_shift(char s[]){

	int i, len = strlen(s);

	char temp = s[len-1];
	for(i = len-1; i > 0; i--){
		s[i] = s[i-1];
	}
	s[0] = temp;
}

/////////////////////////////////////////////////////////////
// hw5-1 find LCS
// input: s1 and s2
// output: lcs 
/////////////////////////////////////////////////////////////
void findLCS(int** table, char lcs[], const char s1[], const char s2[]){
	int i, j;
	int x, y;
	x = strlen(s1);
	y = strlen(s2);

	// init table
	for(i = 0; i < x + 1; i++){
		for(j = 0; j < y + 1; j++){
			table[i][j] = 0;
		}
	}

	// make table
	for(i = 0; i < x; i++){
		for(j = 0; j < y; j++){
			if(s1[i] == s2[j]){
				table[i+1][j+1] = table[i][j] + 1;
			}
			else{
				if(table[i][j+1] > table[i+1][j]){
					table[i+1][j+1] = table[i][j+1];
				}
				else{
					table[i+1][j+1] = table[i+1][j];
				}
				
			}
		}
	}

	// print_table(table, x+1, y+1);
	
	while(table[x][y] != 0){

		if(table[x][y] > table[x][y-1] && table[x][y] > table[x-1][y]){
			*(lcs + table[x][y] - 1) = s1[x-1];
			x--;
			y--;
		}
		else if(table[x][y] == table[x-1][y] && table[x][y] > table[x][y-1]){
			x--;
		}
		else if(table[x][y] == table[x][y-1] && table[x][y] > table[x-1][y]){
			y--;
		}
		else{	
			// 3 way to find LCS.
			// 1) x-- and y--
			// 2) only x--
			// 3) only y--

			// x--;
			y--;
		}
	}
}


/////////////////////////////////////////////////////////////
// hw5-2 find LCS with circular shift
// input: s1 and s2
// output: s1 and lcs --- s1 will be overwritten
//   s1 shifted string of s1
//   lcs LCS of shifted-s1 and s2
/////////////////////////////////////////////////////////////
int findLCS_circ(int** table, char lcs[], char s1[], const char s2[])
{
	int i;
	int max = strlen(lcs), count = 0;

	for(i = 0; i < strlen(s1); i++){
		circular_shift(s1);
		findLCS(table, lcs, s1, s2);

		if(max < strlen(lcs)){
			max = strlen(lcs);
			count = i + 1;
		}
	}

	for(i = 0; i < count; i++){
		circular_shift(s1);
	}
	findLCS(table, lcs, s1, s2);

	return count;
}

/////////////////////////////////////////////////////////////
// main function
/////////////////////////////////////////////////////////////
/* FILL: may change the content of main function, as long as 
* the printed output is the same */
int main(int argc, char *argv[])
{
	char *s1, *s2;
	int l1, l2, min_l;
	char *lcs;	// to store results of LCS algorithms
	int cut_max;	// cut point for circular LCS
	int i, j;
	int x, y;


	#ifdef MEASURE_TIME
		clock_t start, end;
		double cpu_time_used;
	#endif

	if ( argc != 3 ) {
		fprintf(stderr, "usage: %s string1 string2\n",argv[0]);
		exit(0);
	}
	else {
		fprintf(stdout,"$ ");
		for (i=0; i<argc; i++) fprintf(stdout,"%s ",argv[i]);
		fprintf(stdout,"\n");
	}

	s1 = strdup_c(argv[1]);
	s2 = strdup_c(argv[2]);

	#ifdef MEASURE_TIME
		start = clock();
	#endif

	l1 = strlen(s1);
	l2 = strlen(s2);
	min_l = (l1<l2) ? l1:l2;	// minimum of l1 and l2

	// allocate memory for lcs 
	lcs = (char*)malloc_c(sizeof(char)*(min_l+1));

	// allocate memory for table to dp
	x = strlen(s1);
	y = strlen(s2);
	int** table = malloc_c(sizeof(int*) * (x + 1));
	for(i = 0; i < x + 1; i++){
		table[i] = malloc_c(sizeof(int) * (y + 1));
	}
	

	// 1) find LCS and its length, and print the LCS
	findLCS(table, lcs, s1, s2);
	fprintf(stdout, "1) S1 S2 LCS = %s %s %s\n", s1, s2, lcs);

	// 2) find LCS circularly
	// lcs: found longest common subsequence with circular shift
	// s1: circularly shifted string of the original s1
	// s2: unchanged (const char*)
	cut_max = findLCS_circ(table, lcs, s1, s2);
	fprintf(stdout, "2) S1(shift=%d) S2 LCS = %s %s %s\n", cut_max, s1, s2, lcs);

	#ifdef MEASURE_TIME
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		fprintf(stdout,"%.5f seconds\n",cpu_time_used);
	#endif
	#ifdef MEASURE_MEMORY
		fprintf(stdout,"%lu bytes ( %.3f x %d )\n",
		used_memory,(double)used_memory/(double)(l1*l2),(l1*l2));
	#endif

	fprintf(stdout, "\n");


	// free table
	for(i = 0; i < x+1; i++){
		free(table[i]);
	}
	free(table);
	free(lcs); 
	free(s1); 
	free(s2);
}

