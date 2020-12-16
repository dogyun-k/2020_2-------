/* ID: 2016116256
* NAME: Kim Dogyun
* OS: linux, Ubuntu 16.04 
* Compiler version: gcc 5.4.0 20160609
*/

// NOTE: IN GENERAL, COMMENTED STATEMENTS FOLLOWED BY 
// ">>>" WRITTEN IN CAPITAL LETTERS ARE DIRECTIONS

// >>> (10/100) pts
// >>> IN THE TOP 4-LINES COMMENTS 
// >>> LINE 1: WRITE YOUR ID (NON-NUMERIC LETTERS ARE FINE)
// >>> Line 2: WRITE YOUR NAME (NO HANGUL)
// >>> DO NOT CHANGE OS OR Compiler
// >>> COMPILE AND RUN YOUR CODE ON THE LINUX MACHINE

// HOMEWORK PROGRAMMING ASSIGNMENT 4-1
// HASH FUNCTION DESIGN FOR WORD COUNTING

#include<stdio.h>
#include<stdlib.h>	// atoi
#include<string.h>	// string library

#define MEASURE_TIME	// to measure execution time
#define MEASURE_MEMORY	// to measure required memory

#ifdef MEASURE_TIME
	/////////////////////////////////////////////////////////////////////
	// to compute execution time in seconds
	/////////////////////////////////////////////////////////////////////
	#include<time.h>
#endif

#ifdef MEASURE_MEMORY
	/////////////////////////////////////////////////////////////////////
	// to compute used memory
	/////////////////////////////////////////////////////////////////////
	static size_t used_memory = 0;

	// allocate memory with counting bytes
	void *malloc_c(size_t size) {
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
		size = strlen(s)+1;	// including last null character
		used_memory += size;
		return strdup(s);
	}
#else
	void *malloc_c(size_t size) { return malloc(size); }
	char *strdup_c(const char *s) { return strdup(s); }
#endif

/////////////////////////////////////////////////////////////
// linked list for word counting
/////////////////////////////////////////////////////////////
typedef struct WORD_NODE {
	char *word;	// content
	int count;	// number of appearances
	struct WORD_NODE *next;
}NODE;

struct WORD_NODE *create_wnode( const char w[] ) {
	struct WORD_NODE *newnode;

	// create a new word node
	newnode = (struct WORD_NODE*)malloc_c(sizeof(struct WORD_NODE));
	newnode->word = strdup_c(w);
	newnode->count = 1;
	newnode->next = NULL;

	return newnode;
}

void free_wnodelist_recursive(struct WORD_NODE *node) { 
	if ( node ) {
		free_wnodelist_recursive(node->next);
		free(node->word);
		free(node);
	}
}

/* ========= FILL ======== */
// functions for handing struct WORD_NODE
// changing the struct or 
// defining any additional functions and structs are allowed

/* ========= END OF FILL ======== */



/////////////////////////////////////////////////////////////
// hash table for word counting
/////////////////////////////////////////////////////////////
struct WORD_HASH_TABLE {
	struct WORD_NODE **wnode;	// hash table, array of word node lists
	int size;					// size of the hash table
	int num_words;				// count unique number of words, same as the length of the list
};

struct WORD_HASH_TABLE *create_word_hashtable( int hashtabsize ) {
	int i;
	struct WORD_HASH_TABLE *htab;

	htab = (struct WORD_HASH_TABLE*)malloc_c(sizeof(struct WORD_HASH_TABLE));
	htab->wnode = (struct WORD_NODE**)malloc_c(sizeof(struct WORD_NODE*) * hashtabsize);

	for (i = 0; i < hashtabsize; i++){
		htab->wnode[i] = (NODE*)malloc_c(sizeof(NODE*));
		htab->wnode[i]->next = NULL;
	}

	htab->size = hashtabsize;
	htab->num_words = 0;

	return htab;
}

void print_hash_table(struct WORD_HASH_TABLE *htab){
	printf("\n\nprint hash table \n");
	printf("hashtable %d buckets %d words", htab->size, htab->num_words);
	for(int i = 0; i < htab->size; i++){
		NODE* cur_node = htab->wnode[i]->next;
		printf("\nBucket %d ", i);
		while(cur_node != NULL){
			printf("(%s %d) ", cur_node->word, cur_node->count);
			cur_node = cur_node->next;
		}
	}
}

void free_hashtable(struct WORD_HASH_TABLE *htab) {
	int i;
	if ( htab ) {
		for (i=0; i<htab->size; i++) 
		free_wnodelist_recursive(htab->wnode[i]);
	}
	free(htab);
}

/* ========= FILL ======== */
// functions for handing struct WORD_HASH_TABLE
// changing the struct or 
// defining any additional functions and structs are allowed
int hash_function(char word[], int htab_size){
	int word_value = 0;

	for(int i = 0; i < strlen(word); i++){
		word_value += (int)word[i];
	}

	return word_value % htab_size;
}

NODE* search(struct WORD_HASH_TABLE *htab, char key[]){
	NODE* cur_node;

	for(int i = 0; i < htab->size; i++){
		cur_node = htab->wnode[i]->next;

		while(cur_node != NULL){
			if(strcmp(cur_node->word, key) == 0){
				return cur_node;
			}
			cur_node = cur_node->next;
		}
	}

	return 0;
}

void insert(struct WORD_HASH_TABLE *htab, char word[], int hash_value){

	if(search(htab, word) != 0){
		search(htab, word)->count++;
		return;
	}

	NODE* cur_node = htab->wnode[hash_value];
	NODE* new_node = create_wnode(word);
	int cmp;
	
	
	if(cur_node->next == NULL){
		htab->num_words++;
		new_node->next = NULL;
		new_node->count = 1;
		cur_node->next = new_node;

	}
	else{
		while(cur_node->next != NULL){
			cmp = strcmp(cur_node->next->word, new_node->word);

			if(cmp > 0){
				htab->num_words++;
				new_node->next = cur_node->next;
				cur_node->next = new_node;

				return;
			}
			else{
				cur_node = cur_node->next;
			}
		}
		htab->num_words++;
		cur_node->next = new_node;
		new_node->next = NULL;
	}
	
	return;
}

void sort_all(struct WORD_HASH_TABLE *htab){
	NODE* head;

	int temp = 0;
	for(int i = 0; i < htab->size; i++){
		if(htab->wnode[i]->next != NULL){
			head = htab->wnode[i];
			temp = i;
			break;
		}
	}

	printf("\n\n%d\n\n", temp);

	for(int i = temp + 1; i < htab->size; i++){
		NODE* new_node = htab->wnode[i]->next;

		while(htab->wnode[i]->next != NULL && head->next != NULL){
			new_node = htab->wnode[i]->next;
			
			if(strcmp(head->next->word, new_node->word) > 0){
				htab->wnode[i]->next = new_node->next;
				new_node->next = head->next;
				head->next = new_node;
				head = htab->wnode[temp];

				if(htab->wnode[i]->next == NULL){
					break;
				}
			}
			else{
				head = head->next;
				if(head->next == NULL){
					htab->wnode[i]->next = new_node->next;
					new_node->next = head->next;
					head->next = new_node;
					head = htab->wnode[temp];

					if(htab->wnode[i]->next == NULL){
						break;
					}
				}
			}
		}
	}
}

void write_hashtable(char outfile[], struct WORD_HASH_TABLE *htab){
	FILE *fp;
    long n;

    // check for output filename
    if ( outfile == NULL ) {
        fprintf(stderr, "NULL output file name\n");
        return;
    }

    // check for file existence
    fp = fopen(outfile,"w");
    if ( !fp ) {
        fprintf(stderr, "cannot open file %s for write\n",outfile);
    }
    else {
		char s1[128];

		// Write hashtable
		sprintf(s1, "hashtable %d buckets %d words\n", htab->size, htab->num_words);
		fwrite(s1, 1, strlen(s1), fp);
		for(int i = 0; i < htab->size; i++){
			NODE* cur_node = htab->wnode[i]->next;
			
			sprintf(s1, "Bucket %d ", i);
			fwrite(s1, 1, strlen(s1), fp);

			while(cur_node != NULL){
				sprintf(s1, "(%s %d) ", cur_node->word, cur_node->count);
				
				n = fwrite(s1, 1, strlen(s1), fp);

				if ( n != strlen(cur_node->word) ) {
					//fprintf(stderr, "file %s, %ld/%ld bytes written\n", outfile, n, strlen(cur_node->word));
				}
				cur_node = cur_node->next;
			}
			fwrite("\n", 1, strlen("\n"), fp);
		}	

		sort_all(htab);
		// Write sorted list
		sprintf(s1, "\nalphabet-sorted %d words\n", htab->num_words);
		fwrite(s1, 1, strlen(s1), fp);

		NODE* cur_node;

		for(int i = 0; i < htab->size; i++){
			if(htab->wnode[i]->next != NULL){
				cur_node = htab->wnode[i]->next;
				break;
			}
		}
		

		while(cur_node != NULL){
			sprintf(s1, "(%s %d) ", cur_node->word, cur_node->count);
			
			n = fwrite(s1, 1, strlen(s1), fp);

			if ( n != strlen(cur_node->word) ) {
				//fprintf(stderr, "file %s, %ld/%ld bytes written\n", outfile, n, strlen(cur_node->word));
			}
			cur_node = cur_node->next;
		}
		fwrite("\n", 1, strlen("\n"), fp);


        fclose(fp);
    }
}

/* ========= END OF FILL ======== */


/////////////////////////////////////////////////////////////
// main function
/////////////////////////////////////////////////////////////
int main(int argc, char *argv[]){

	struct WORD_HASH_TABLE *hashtable;
	int htab_size;
	char buffer[256];	// input string
	char *infile, *outfile;
	FILE *ifp, *ofp;
	int j;
	/* ========= FILL ======== */
	// any additional variables can be defined here
	int hash_value;
	/* ========= END OF FILL ======== */

	#ifdef MEASURE_TIME
		clock_t start, end;
		double cpu_time_used;
		start = clock();
	#endif

	// INPUT ARGUMENTS
	if ( argc != 4 ) {
		//fprintf(stderr, "%d\n",argc);
		fprintf(stderr, "usage: %s hashtablesize infile outfile\n",argv[0]);
		exit(0);
	}
	else { 
		htab_size = atoi(argv[1]);
		infile = argv[2];
		outfile = argv[3];
	}

	// create a hash table
	hashtable = create_word_hashtable(htab_size);

	// check for input file existence
	if ( (ifp = fopen(infile,"r")) == NULL ) {
		fprintf(stderr, "cannot open file %s\n",infile);
		return -1;
	}
	else {
		// read word one by one to the end of file
		for (j = 0; !feof(ifp); j++) {	// feof: end of file
			// 1. read word
			if ( fscanf(ifp, "%s", buffer) != 1 ){
				break;
			} 	// out of the loop
			else {
				/* ========= FILL ======== */
				hash_value = hash_function(buffer, htab_size);
				insert(hashtable, buffer, hash_value);
				/* ========= END OF FILL ======== */
			}
		}
		fclose(ifp);	// read done

		// check for output file
		if ( (ofp = fopen(outfile, "w")) == NULL ) {
			fprintf(stderr, "cannot open file for write %s\n",outfile);
			return -1;
		}
		else {
			/* ========= FILL ======== */
			// 5. print the hash table to the output file (ofp)
			print_hash_table(hashtable);
			write_hashtable(outfile, hashtable);

			// 6. print the sorted list of all the words to the output file (ofp)

			/* ========= END OF FILL ======== */
			fclose(ofp);
		}
	} 

	print_hash_table(hashtable);


	#ifdef MEASURE_TIME
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		fprintf(stdout,"\n\n%.5f seconds\n",cpu_time_used);
	#endif
	#ifdef MEASURE_MEMORY
		fprintf(stdout,"%lu bytes ( %.3f x %d )\n",
		used_memory, (double)used_memory/(double)hashtable->num_words, hashtable->num_words);
	#endif
	free_hashtable(hashtable);
	return 0;
}
