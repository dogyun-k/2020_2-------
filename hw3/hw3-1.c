/* ID: 2016116256		>>> REPLACE WITH YOUR ID
* NAME: Kim Dogyun	    >>> REPLACE WITH YOUR NAME
* OS: linux, Ubuntu 16.04 
* Compiler version: gcc 5.4.0 20160609
*/

//////// Reference ////////////////////////////////////////////
// Naver Blog : https://blog.naver.com/ndb796 (Donbin Na blog)
// Lecture 07
///////////////////////////////////////////////////////////////


// no memcpy, memccpy, memmove, wmemmove, or other direct memory copy/move functions
//  these functions performs 'BLOCKED' operations so that large chunk of memory 
//  operations are efficiently implemented, so they break UNIT TIME assumption
//  in algorithm design
// string functions: strcpy, strncpy, strlen, strcmp are allowed, but not strdup

// >>> (10/100) pts
// >>> IN THE TOP 4-LINES COMMENTS 
// >>> LINE 1: REPLACE WITH YOUR ID (IF YOU HAVE NON-NUMERIC, IGNORE IT)
// >>> Line 2: REPLACE WITH YOUR NAME (NO HANGUL)
// >>> DO NOT CHANGE OS AND Compiler
// >>> COMPILE AND RUN YOUR CODE ON THE LINUX MACHINE

// HOMEWORK PROGRAMMING ASSIGNMENT 3
// IMPLEMENTATION OF SORTING ALGORITHMS FOR CHAR ARRAY STRINGS

#include<stdio.h>
#include<stdlib.h>	// atoi, malloc, free
#include<string.h>	// string library
#include<stdint.h>	// fixed-width integers, since C99 --- 1999 ISO standards

#define MEASURE_TIME	// to measure execution time
#define MEASURE_MEMORY	// to measure required memory
// from GJANG: free is not considered
// just counts the amount of memory required by malloc_c

/////////////////////////////////////////////////////////////////////
// to compute execution time in seconds
/////////////////////////////////////////////////////////////////////
#ifdef MEASURE_TIME
    #include<time.h>
#endif

/////////////////////////////////////////////////////////////////////
// to compute used memory
// use malloc_c defined below, instead of malloc, calloc, realloc, etc.
// malloc_c accumulates the size of the dynamically allocated memory to
// global/static variable used_memory, so that we can measure the 
// used amount of memory exactly.
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
        else {
            return NULL;
        }
    }
#else
    void *malloc_c(size_t size) { 
        return malloc(size); 
    }
#endif

/////////////////////////////////////////////////////////////
// read whole file into memory
/////////////////////////////////////////////////////////////
char* read_bytes( const char infile[], long *pNbytes )
// returns an char array with its size stored in
// the memory indicated by integer pointer variable pNbytes
// the retured memory should freed by the caller
{
    char *B;
    long n;
    FILE *fp;

    // check for input file name
    if ( infile == NULL ) {
        fprintf(stderr, "NULL input file name\n");
        return NULL;
    }

    // check for file existence
    fp = fopen(infile,"r");
    if ( !fp ) {
        fprintf(stderr, "cannot open file %s for read\n",infile);
        return NULL;
    }
    else {
        // find the file size
        fseek(fp, 0, SEEK_END);	// move to the end of file
        *pNbytes = ftell(fp);	// file size = end loc from the start
        if ( *pNbytes <= 0 ) return NULL;	// no element

        // to the beginning again
        rewind(fp);

        // allocate memory and read the file
        B = (char*)malloc_c(*pNbytes+1);
        n = fread(B, 1, *pNbytes, fp);

        if ( n != *pNbytes ) {
            fprintf(stderr, "file %s, %ld/%ld bytes read\n", infile, n, *pNbytes);
        }
        B[*pNbytes] = '\0';
        //fprintf(stderr, "file %s, %ld/%ld bytes read\n",infile,n,*pNbytes);

        // close file 
        fclose(fp);
        return B;
    }
}

/////////////////////////////////////////////////////////////
// write memory to a text file 
/////////////////////////////////////////////////////////////
void write_bytes( const char outfile[], const char B[], long Nbytes )
// write the char array of size Nbytes
// to file whose name given by outfile[]
{
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
        n = fwrite(B, 1, Nbytes, fp);
        if ( n != Nbytes ) {
            fprintf(stderr, "file %s, %ld/%ld bytes written\n",outfile,n,Nbytes);
        }
        //fprintf(stderr, "file %s, %ld/%ld bytes written\n",outfile,n,Nbytes);

        // close file 
        fclose(fp);
    }
}

/////////////////////////////////////////////////////////////
// FILL: YOUR AREA
// DEFINE YOUR OWN FUNCTIONS
/////////////////////////////////////////////////////////////

// Merge sort
void merge(char** arr, char** temp, int left, int mid, int right){
    int i = left;
    int j = mid + 1;
    int k = left;

    int t;
 
    // Problem ****
    while(i <= mid && j <= right){
        if(strcmp(arr[i], arr[j]) <= 0){
            temp[k] = arr[i];
            i++;
        }
        else{
            temp[k] = arr[j];
            j++;
        }
        k++;
    }

    if(i > mid){
        for(t = j; t <= right; t++){
            temp[k] = arr[t];
            k++;
        }
    }
    else{
        for(t = i; t <= mid; t++){
            temp[k] = arr[t];
            k++;
        }
    }

    for( t = left; t <= right; t++){
        arr[t] = temp[t];
    }
}

void merge_sort(char** arr, char** temp, int left, int right){
    if(left < right){
        int mid = (left + right) / 2;

        merge_sort(arr, temp, left, mid);
        merge_sort(arr, temp, mid + 1, right);
        merge(arr, temp, left, mid, right);
    }
}

// Quick sort
void quick_sort(char** arr, int start, int end){
    if(start >= end){
        return;
    }

    int pivot = start;
    int i = start + 1;
    int j = end;
    char* temp;

    while(i <= j){

        while(i <= end && strcmp(arr[pivot], arr[i]) >= 0){
            i++;
        }
        
        while(j > start && strcmp(arr[pivot], arr[j]) <= 0){
            j--;
        }
        if(i > j){
            temp = arr[j];
            arr[j] = arr[pivot];
            arr[pivot] = temp;
        }
        else{
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    quick_sort(arr, start, j - 1);
    quick_sort(arr, j + 1, end);
}



/////////////////////////////////////////////////////////////
// main function
/////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
// 1: method  2: word_length  3: input_file  4: output_file
{
    long numBytes;
    char *B;	// to store data to be sorted
    int method;	// which sorting method
    int fixdotwordsize;

    #ifdef MEASURE_TIME
        clock_t start, end;
        double cpu_time_used;
    #endif

    if ( argc != 5 ) {
        fprintf(stderr, "usage: %s method fixedwordsize infile outfile\n",argv[0]);
        fprintf(stderr, " method 1: mergesort, 2: quicksort\n");
        exit(0);
    }

    method = atoi(argv[1]);
    fixdotwordsize = atoi(argv[2]) + 1;	// including last '.'
    B = read_bytes(argv[3], &numBytes);


    int i;
    int left = 0;
    int right = 0;
    for(int i = 0; i < strlen(B); i++){
        if(B[i] == '.'){
            right++;
        }
    }


    char* ptr = strtok(B, ".");
    char** list = (char**)malloc_c(sizeof(char*) * right);
    char** temp = (char**)malloc_c(sizeof(char*) * right);
    for(i = 0; i < right; i++){
        list[i] = (char*)malloc_c(sizeof(char) * fixdotwordsize);
        temp[i] = (char*)malloc_c(sizeof(char) * fixdotwordsize);
        
        strcpy(list[i], ptr);
        temp[i] = NULL;
        ptr = strtok(NULL, ".");
    }   

    

    #ifdef MEASURE_TIME
        start = clock();
    #endif

    int check = 0;
    if ( B != NULL && numBytes > 0 ) {
        switch ( method ) {
            case 1:	// merge sort

                merge_sort(list, temp, left, right - 1);

                // move list to B
                for(i = 0; i < right; i++){
                    if(i == 0){
                        strcpy(B, list[i]);     
                        check += strlen(list[i]);          
                    }
                    else{
                        strcpy(B + (check * sizeof(char)), ".");
                        strcpy(B + ((check + 1) * sizeof(char)), list[i]);
                        check += strlen(list[i]) + 1;
                    }  
                }
                strcpy(B + check, ".");
                break;

            case 2:	// quick sort

                quick_sort(list, left, right - 1);

                // move list to B
                for(i = 0; i < right; i++){
                    if(i == 0){
                        strcpy(B, list[i]);     
                        check += strlen(list[i]);          
                    }
                    else{
                        strcpy(B + (check * sizeof(char)), ".");
                        strcpy(B + ((check + 1) * sizeof(char)), list[i]);
                        check += strlen(list[i]) + 1;
                    }  
                }
                strcpy(B + check, ".");
                break;

            default:
                break;
        }

        #ifdef MEASURE_TIME
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            fprintf(stdout,"%.5f seconds\n",cpu_time_used);
        #endif
    
        #ifdef MEASURE_MEMORY
            fprintf(stdout,"%lu bytes ( %.3f x %ld )\n",
            used_memory, (double)used_memory/(double)numBytes, numBytes);
            //used_memory,(double)used_memory/(double)(numBytes-1),numBytes-1);
        #endif

        // save results
        write_bytes(argv[4], B, numBytes);

        // once allocated, should be freed after use
        free(list);
        free(temp);
        free(B);
    }
}
