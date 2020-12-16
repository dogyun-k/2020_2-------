/* ID : 2016116256
 * NAME : Kim Dpgyun
 * OS : linux, Ubuntu 16.04
 * Compiler version : gcc 5.4.0 20160609
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[]){
	FILE *fp;
	int i, data_num;
		

	fp = fopen(argv[1], "r");
	fscanf(fp, "%d", &data_num);

	
	int data[data_num];

	for(i = 0; i < data_num; i++){
		fscanf(fp, "%d", &data[i]);
	}
		
	int min = data[0];
	int max = data[0];
	double summary = data[0], mean, variance;
	
	for(i = 1; i < data_num; i++){
		if(min > data[i]){
			min = data[i];
		}
		if(max < data[i]){
			max = data[i];
		}
		summary += data[i];
	}
	mean = summary / data_num;
	
	for(i = 0; i < data_num; i++){
		variance += (data[i] - mean)*(data[i] - mean);
	}
	variance /= data_num;

	printf("#data\tmin\tmax\tmean\tvariance\n");
	printf("%d\t%d\t%d\t%.1lf\t%.1lf\n", data_num, min, max, mean, variance);

	

	fclose(fp);
}
