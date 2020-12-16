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

// HOMEWORK PROGRAMMING ASSIGNMENT 4-2
// HASH FUNCTION DESIGN FOR WORD COUNTING SORTED BY NUMBER OF APPEARANCES

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

/////////////////////////////////////////////////////////////
// linked list for word counting
/////////////////////////////////////////////////////////////
typedef struct WORDNODE {
	char *word;	// content
	int count;	// number of appearances
	struct WORDNODE *next;
	int code;	// code, in the descending order of counts
}NODE;

struct WORDNODE *create_wnode( const char w[] ) {
	struct WORDNODE *newnode;

	// create a new word node
	newnode = (struct WORDNODE*)malloc_c(sizeof(struct WORDNODE));
	newnode->word = strdup_c(w);
	newnode->count = 1;
	newnode->next = NULL;
	newnode->code = -1;	// initially not set
	return newnode;
}

void free_wnodelist_recursive(struct WORDNODE *node) { 
	if ( node ) {
		free_wnodelist_recursive(node->next);
		free(node->word);
		free(node);
	}
}





/////////////////////////////////////////////////////////////
// hash table for word counting
/////////////////////////////////////////////////////////////
struct WORD_HASH_TABLE {
	struct WORDNODE **wnode;	// hash table, array of word node lists
	int size;	// size of the hash table
	int num_words;	// count unique number of words, same as the length of the list
};

struct WORD_HASH_TABLE *create_word_hashtable( int hashtabsize ) {
	int i;
	struct WORD_HASH_TABLE *htab;

	htab = (struct WORD_HASH_TABLE*)malloc_c(sizeof(struct WORD_HASH_TABLE));
	htab->wnode = (struct WORDNODE**)malloc_c(sizeof(struct WORDNODE*)*hashtabsize);
	
	for (i = 0; i < hashtabsize; i++) {
		htab->wnode[i] = (NODE*)malloc_c(sizeof(NODE*));
		htab->wnode[i]->next = NULL;
	}
	htab->size = hashtabsize;
	htab->num_words = 0;

	return htab;
}

void free_hashtable(struct WORD_HASH_TABLE *htab) {
	int i;
	for (i=0; i<htab->size; i++) {
		free_wnodelist_recursive(htab->wnode[i]);
	}
	free(htab);
}

/* ========= FILL ======== */

void print_hash_table(struct WORD_HASH_TABLE *htab){
	printf("print hash table \n");
	printf("hashtable %d buckets %d words", htab->size, htab->num_words);

	for(int i = 0; i < htab->size; i++){
		NODE* cur_node = htab->wnode[i]->next;
		printf("\nBucket %d ", i);
		while(cur_node != NULL){
			printf("(%s %d %d) ", cur_node->word, cur_node->count, cur_node->code);
			cur_node = cur_node->next;
		}
	}

	printf("\n");
}

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

void init_hashtable(struct WORD_HASH_TABLE *htab){
	for(int i = 0; i < htab->size; i++){
		htab->wnode[i]->next = NULL;
	}
}

void re_arrange(struct WORD_HASH_TABLE *htab){
	NODE* head;
	NODE* cur_node;

	for(int i = 0; i < htab->size; i++){
		if(htab->wnode[i]->next != NULL){
			head = htab->wnode[i]->next;
			break;
		}
	}

	init_hashtable(htab);

	int hash_value;

	while(head->next != NULL){
		hash_value = hash_function(head->word, htab->size);
		cur_node = htab->wnode[hash_value];
		int cmp;
		
		if(cur_node->next == NULL){
			head->next = NULL;
			cur_node->next = head;
		}
		else{
			while(cur_node->next != NULL){
				cmp = strcmp(cur_node->next->word, head->word);

				if(cmp > 0){
					head->next = cur_node->next;
					cur_node->next = head;

					return;
				}
				else{
					cur_node = cur_node->next;
				}
			}
			cur_node->next = head;
			head->next = NULL;
		}

		head = head->next;
	}
	return;

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

void sort_bucket(struct WORD_HASH_TABLE *htab){

	for(int i = 0; i < htab->size; i++){
		if(htab->wnode[i]->next == NULL){
			continue;
		}

		NODE* temp;
		NODE* cur_node = htab->wnode[i]->next;
		NODE* head = htab->wnode[i];
		head->next = NULL;

		while(cur_node != NULL){
			temp = cur_node->next;

			if(head->next == NULL){		// 만약 빈 리스트일 때
				cur_node->next = NULL;
				head->next = cur_node;
				head = htab->wnode[i];
			}
			else{						// 리스트에 뭐가 있을 때
				
				while(head->next != NULL){
					if(head->next->count < cur_node->count){	// 카운터가 클 때
						cur_node->next = head->next;
						head->next = cur_node;
						head = htab->wnode[i];
						break;
					}
					else if(head->next->count > cur_node->count){	// 카운터가 작을 때
						head = head->next;
					}
					else{	// 카운터가 같으면

						if(strcmp(head->next->word, cur_node->word) > 0){
							cur_node->next = head->next;
							head->next = cur_node;
							head = htab->wnode[i];
							break;
						}
						else{
							head = head->next;
						}
						
					}
				}
			}
			if(head->next == NULL){		// 만약 마지막일 때
				cur_node->next = NULL;
				head->next = cur_node;
				head = htab->wnode[i];
			}
			cur_node = temp;
		}// end while
	}// end for
	
}

int sort_all(struct WORD_HASH_TABLE *htab){
	NODE* head;

	int temp = 0;
	for(int i = 0; i < htab->size; i++){
		if(htab->wnode[i]->next != NULL){
			head = htab->wnode[i];
			temp = i;
			break;
		}
	}

	for(int i = temp + 1; i < htab->size; i++){
		NODE* new_node = htab->wnode[i]->next;

		while(htab->wnode[i]->next != NULL && head->next != NULL){
			new_node = htab->wnode[i]->next;
			
			if(head->next->count < new_node->count){ // 클 때
				htab->wnode[i]->next = new_node->next;
				new_node->next = head->next;
				head->next = new_node;
				head = htab->wnode[temp];
			}
			else if(head->next->count > new_node->count){	// 작을 때
				head = head->next;
			}
			else{		// 같을 때
				head = head->next;
				if(head->next == NULL){
					htab->wnode[i]->next = new_node->next;
					new_node->next = head->next;
					head->next = new_node;
					head = htab->wnode[temp];
				}
		
				// if(strcmp(head->next->word, new_node->word) > 0){
				// 	new_node->next = head->next;
				// 	head->next = new_node;
				// 	head = htab->wnode[i];
				// 	break;
				// }
				// else{
				// 	head = head->next;
				// }

			}
			if(htab->wnode[i]->next == NULL){
				break;
			}

		}// end while
	}


	// code num update
	int count = 1;
	head = htab->wnode[temp]->next;
	while(head != NULL){
		head->code += count++;
		head = head->next;
	}

	return temp;
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
		print_hash_table(htab);
		sprintf(s1, "hashtable %d buckets %d words\n", htab->size, htab->num_words);
		fwrite(s1, 1, strlen(s1), fp);
		for(int i = 0; i < htab->size; i++){
			NODE* cur_node = htab->wnode[i]->next;
			
			sprintf(s1, "Bucket %d ", i);
			fwrite(s1, 1, strlen(s1), fp);

			int code = 0;
			while(cur_node != NULL){
				sprintf(s1, "(%s %d %d) ", cur_node->word, cur_node->count, cur_node->code);
				
				n = fwrite(s1, 1, strlen(s1), fp);

				if ( n != strlen(cur_node->word) ) {
					//fprintf(stderr, "file %s, %ld/%ld bytes written\n", outfile, n, strlen(cur_node->word));
				}
				cur_node = cur_node->next;
			}
			fwrite("\n", 1, strlen("\n"), fp);
		}	




		// Write sorted list
		sort_all(htab);
		printf("\n");
		print_hash_table(htab);
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
			sprintf(s1, "(%s %d %d) ", cur_node->word, cur_node->count, cur_node->code);
			
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

void write_encode(char infile[], char outfile[], struct WORD_HASH_TABLE *htab){
	FILE *fp;
	FILE *ifp;
    long n;
	char buffer[256];
	int j;

    // check for output filename
    if ( outfile == NULL ) {
        fprintf(stderr, "NULL output file name\n");
        return;
    }

    // check for file existence
    fp = fopen(outfile,"a");
    if ( !fp ) {
        fprintf(stderr, "cannot open file %s for write\n",outfile);
    }
    else {
		char s1[128];

		// Write encoded
		print_hash_table(htab);
		sprintf(s1, "Encoded word\n");
		fwrite(s1, 1, strlen(s1), fp);


		if ( (ifp = fopen(infile, "r")) == NULL ) {
			fprintf(stderr, "cannot open file %s\n",infile);
			return;
		}
		else {
			NODE* new;
			
			for (j = 0; !feof(ifp); j++) {	// feof: end of file
				if ( fscanf(ifp, "%s", buffer) != 1 ) {
					break;
				}	// out of the loop
				else {
					
					new = search(htab, buffer);
					sprintf(s1, "%d ", new->code);
					fwrite(s1, 1, strlen(s1), fp);
					printf("tttttttttttt\n");

				}
			}
			

			fclose(ifp);
		}

        fclose(fp);
    }
}
/* ========= END OF FILL ======== */






/////////////////////////////////////////////////////////////
// main function
/////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	struct WORD_HASH_TABLE *hashtable;
	int htab_size;
	char buffer[256];	// input string
	char *infile, *outfile1, *outfile2;
	FILE *ifp, *ofp;
	int j;
	/* ========= FILL ======== */
	int hash_value;
	/* ========= END OF FILL ======== */

	#ifdef MEASURE_TIME
	clock_t start, end;
	double cpu_time_used;
	start = clock();
	#endif

	// INPUT ARGUMENTS
	if ( argc != 5 ) {
		fprintf(stderr, "usage: %s hashtablesize infile outfile1 outfile2\n",argv[0]);
		exit(0);
	}
	else { 
		htab_size = atoi(argv[1]);
		infile = argv[2];
		outfile1 = argv[3];
		outfile2 = argv[4];
	}

	// create a hash table
	hashtable = create_word_hashtable(htab_size);


	// check for input file existence
	if ( (ifp = fopen(infile, "r")) == NULL ) {
		fprintf(stderr, "cannot open file %s\n",infile);
		return -1;
	}
	else {
		// read word one by one to the end of file
		NODE* new;
		for (j = 0; !feof(ifp); j++) {	// feof: end of file
			if ( fscanf(ifp, "%s", buffer) != 1 ) {
				break;
			}	// out of the loop
			else {
				/* ========= FILL ======== */
				hash_value = hash_function(buffer, htab_size);
				insert(hashtable, buffer, hash_value);

				/* ========= END OF FILL ======== */
			}
		}
		
		sort_bucket(hashtable);
		write_hashtable(outfile1, hashtable);

		rewind(ifp);	// move to the beginning of the file, for the read in ENCODING

		write_encode(infile, outfile2, hashtable);

		
		/* ========= FILL ======== */
		// B. outfile1
		//   5. print the hash table to file "outfile1"
		//      the member variable "code" is all -1, because they can be decided
		//      after the next step
		//   6. print the sorted (count-then-alphabetic) list of all the words
		//      while sorting, update the member variable "code" of each WORDNODE instances
		//      by the order of "count"
		//   7. print the sorted list of all the words with updatd code
		//
		// C. outfile2
		//   8. ENCODING: now we have the codes of all words by the "count" order, 
		//      input file can be encoded by replacing each word with its code
		//      open file "infile" again for read, and open file "outfile2" for write, 
		//      and store the encoding results (numbers only) to "outfile2"
		//      whitespaces differences (space/linebreak/tab) are accepted 
		//  
		/* ========= END OF FILL ======== */

		fclose(ifp);
	}
	


	////////////////////////////////////////////////test//////////////////////////////////////

	// printf("\n\n\n\n\n######### test ############\n\n");
	// int index = 0;
	// for(int i = 0; i < hashtable->size; i++){
	// 	if(hashtable->wnode[i]->next != NULL){
	// 		index = i;
	// 		break;
	// 	}
	// }
	// NODE* temp = hashtable->wnode[index]->next;

	// while(temp != NULL){
	// 	printf("(%s %d %d) ", temp->word, temp->count, temp->code);
	// 	temp = temp->next;
	// }

	// printf("\n\n\n\n\n###########################\n\n\n");





	////////////////////////////////////////////////////////////////////////////////////////////


	#ifdef MEASURE_TIME
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		fprintf(stdout,"%.5f seconds\n",cpu_time_used);
	#endif
	#ifdef MEASURE_MEMORY
		fprintf(stdout,"%lu bytes ( %.3f x %d )\n",
		used_memory,(double)used_memory/(double)hashtable->num_words,hashtable->num_words);
	#endif
	free_hashtable(hashtable);
	return 0;
}
