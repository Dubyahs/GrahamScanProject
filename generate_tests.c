#include<stdlib.h>
#include<stdio.h>
#include<time.h>
/*
This program will generate two files.
File one, "rand1.txt" contains 10 random points
File two, "rand2.txt" contains 1000 random points
*/


int get_rand_int();
void ten_points();
void thousand_points();

int main(void) {
    srand(time(NULL));
	ten_points();
	thousand_points();
}

int get_rand_int() {
	double max = 200;
	double r = max * ((double)rand() / (double)RAND_MAX - 0.5);
	return (int)r;
}

void ten_points() {
	FILE *fp;

	fp = fopen("rand1.txt", "w");
	fprintf(fp, "%d\n", 10);
	for (int i = 0; i < 10; i++) {
		fprintf(fp, "%d %d\n", get_rand_int(), get_rand_int());
	}
	fclose(fp);
}
void thousand_points() {
	FILE *fp;

	fp = fopen("rand2.txt", "w");
	fprintf(fp, "%d\n", 1000);
	for (int i = 0; i < 1000; i++) {
		fprintf(fp, "%d %d\n", get_rand_int(), get_rand_int());
	}
	fclose(fp);
}
