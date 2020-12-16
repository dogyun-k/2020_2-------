/* ID: COMP320      >>> REPLACE WITH YOUR ID
 * NAME: Algorithms 1   >>> REPLACE WITH YOUR NAME
 * OS: linux, Ubuntu 16.04 
 * Compiler version: gcc 5.4.0 20160609
 */

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
#include<stdlib.h>   // atoi, malloc, free
#include<string.h>   // string library
#include<stdint.h>   // fixed-width integers, since C99 --- 1999 ISO standards




#define MEASURE_TIME   // to measure execution time

#define MEASURE_MEMORY   // to measure required memory
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
  else return NULL;
}
#else
void *malloc_c(size_t size) { return malloc(size); }
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
    fseek(fp,0,SEEK_END);   // move to the end of file
    *pNbytes = ftell(fp);   // file size = end loc from the start
    if ( *pNbytes <= 0 ) return NULL;   // no element

    // to the beginning again
    rewind(fp);

    // allocate memory and read the file
    B = (char*)malloc_c(*pNbytes+1);
    n = fread(B,1,*pNbytes,fp);
    if ( n != *pNbytes ) {
      fprintf(stderr, "file %s, %ld/%ld bytes read\n",infile,n,*pNbytes);
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
    n = fwrite(B,1,Nbytes,fp);
    if ( n != Nbytes ) {
      fprintf(stderr, "file %s, %ld/%ld bytes written\n",outfile,n,Nbytes);
    }
    //fprintf(stderr, "file %s, %ld/%ld bytes written\n",outfile,n,Nbytes);

    // close file 
    fclose(fp);
  }
}

/////////////////////////////////////////////////////////////
/*

char* left(char list[], int offset){
    char* re;
    int i;
    if((re = (char*)malloc(sizeof(char)*(offset-1)))==NULL) return NULL;

    for(i=0;i<offset-1;i++) re[i] = list[i];

    re[offset-1] = '\0';

    return re;
}

char* right(char list[], int offset, int numBytes){
    char* re;
    int i, n=0;
    if((re = (char*)malloc(sizeof(char)*(offset-1)))==NULL) return NULL;

    for(i=numBytes-2;i>numBytes-offset-1;i--){
        re[n] = list[i];
        n++;
    }

    re[offset-1] = '\0';

    return re;
}*/

void print_(char** l){
    int i;
    for(i=0;i<9;i++){
        printf("%s\n", l[i]);
    }
    printf("\n\n");
}


void merge(char** l, char** sorted, int left, int mid, int right, int offset){
    int i, j, m, k;
    i = left;
    j = mid+1;
    k = left;
    
    while(i<=mid && j<=right){
        if(strcmp(l[i],l[j])<=0) strcpy(sorted[k++],l[i++]);
        else strcpy(sorted[k++], l[j++]);
    }

    

    if(strcmp(l[i],l[mid])>=0){
        for(m=j;m<=right;m++) strcpy(sorted[k++], l[m]);
        
    }
    else{
        for(m=i;m<=mid;m++) strcpy(sorted[k++], l[m]);
    }

    //for(m=left; m<=right;m++) strcpy(l[m], sorted[m]);

    print_(sorted);

    return;
}

void merge_sort(char** l, char** sorted, int left, int right, int num, int offset){
    int mid;


    if(left<right){
        mid = (left + right)/2;
        merge_sort(l, sorted, left, mid, num, offset);
        merge_sort(l, sorted, mid+1, right, num, offset);
        merge(l, sorted, left, mid, right, offset);
    }

    return;

}

void merge_sort_(char list[], int offset, int numBytes, int num){
    char** l;
    char** sorted;
    char* temp;
    int i, left = 0, right = num;
    if((l = (char**)malloc(sizeof(char*)*9)) == NULL) return;
    for(i=0;i<9;i++) if((l[i] = (char*)malloc(sizeof(char)*(offset-1))) == NULL) return;

    if((sorted = (char**)malloc(sizeof(char*)*9)) == NULL) return;
    for(i=0;i<9;i++){
        if((sorted[i] = (char*)malloc(sizeof(char)*(offset-1))) == NULL) return;
    } 

    if((temp = (char*)malloc(sizeof(char) * (offset - 1))) == NULL) return;
    printf("list : %s\n", list);

    temp = strtok(list, ".");
    for(i=0;i<9;i++){
        strcpy(l[i], temp);
        temp = strtok(NULL, ".");
        
    }

    for(i=0;i<9;i++) printf("list[%d] : %s\n", i, l[i]);

    merge_sort(l, sorted, left, right, num, offset);

    for(i=0;i<=num;i++){
        printf("[%d] : %s\n", i, sorted[i]);
    }

    for(i=0;i<9;i++) free(l[i]);
    free(l);
    for(i=0;i<9;i++) free(sorted[i]);
    free(sorted);
    free(temp);
    
}
/*
void merge_sort_(char list[], int offset, int numBytes){
    char* left;
    char* right;
    int i, n = 0;

    
    //printf("numbytes : %d\n", numBytes);
    //printf("offset : %d\n", offset);


    left = left(list, offset);
    right = right(list, offset, numBytes);


    //printf("first : %s\n", first);
    //printf("last : %s\n", last);

    merge_sort(char list[], char* left, char* right, int offset)

    free(first);
    free(last);

    
}*/

/////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
// main function
/////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
  long numBytes;
  char *B;   // to store data to be sorted
  int method;   // which sorting method
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
  fixdotwordsize = atoi(argv[2])+1;   // including last '.'
  B = read_bytes(argv[3], &numBytes);

#ifdef MEASURE_TIME
  start = clock();
#endif
  if ( B != NULL && numBytes > 0 ) {
    switch ( method ) {
      case 1:   // merge sort

        merge_sort_(B, fixdotwordsize, numBytes, 8);


   break;
      case 2:   // quick sort
   /////////////////////////////////////////////////////////////
   // FILL
   /////////////////////////////////////////////////////////////
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
   used_memory,(double)used_memory/(double)numBytes,numBytes);
   //used_memory,(double)used_memory/(double)(numBytes-1),numBytes-1);
#endif

    // save results
    write_bytes(argv[4], B, numBytes);

    // once allocated, should be freed after use
    free(B);
  }
}