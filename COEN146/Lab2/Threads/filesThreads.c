/* 
* Name: Xavier Kuehn
* Date: 04/06/2022
* Title: Lab2 – Step 1 
* Description: copy 10 files by creating threads, each of which will 
be responsible of copying one file 
*/

#include <stdio.h>   // fprintf(), fread(), fwrite(), fopen(), fclose()
#include <stdlib.h>  // malloc(), free()
#include <pthread.h> // pthread_create()
#include <unistd.h>  // read(), write()
#include <fcntl.h>   // open(), close()
#include <errno.h>   // errno
#include <time.h>    // clock()

#define BUF_SIZE 2048 //buffer size

// data structure to hold copy
struct copy_struct {
	int thread_id; 
	char* src_filename;
	char* dst_filename; 
};

// copies a files from src_filename to dst_filename using functions fopen(), fread(), fwrite(), fclose()
int func_copy(char* src_filename, char* dst_filename) {
	
	FILE * src = fopen(src_filename, "r");	// opens a file for reading
	if (src == NULL) { // fopen() error checking
		fprintf(stderr, "unable to open %s for reading: %i\n", src_filename, errno);
		exit(0);
	}
	FILE * dst = fopen(dst_filename, "w");	// opens a file for writing; erases old file/creates a new file
	if (dst == NULL) { // fopen() error checking
		fprintf(stderr, "unable to open/create %s for writing: %i\n", dst_filename, errno);
		exit(0);
	}

	char * buf = (char *)malloc((size_t)BUF_SIZE);  // allocate a buffer to store read data
	// reads content of file is loop iterations until end of file
	size_t bytes_read = fread(buf, 1, BUF_SIZE, src);
	// writes bytes_read to dst_filename 
	fwrite(buf, 1, bytes_read, dst);
	// closes src file pointer
    fclose(src);
	// closes dst file pointer
    fclose(dst);
	// frees memory used for buf
    free(buf);
	return 0;
}

// thread function to copy one file
void* copy_thread(void* arg) {
	struct copy_struct params = *(struct copy_struct*)arg;  // cast/dereference void* to copy_struct
	printf("thread[%i] - copying %s to %s\n", params.thread_id, params.src_filename, params.dst_filename);
	// call file copy function
	func_copy(params.src_filename, params.dst_filename);
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	// check correct usage of arguments in command line
	if (argc != 21) {  
		fprintf(stderr, "usage: %s <10 connections> ........\n", argv[0]);
		exit(1);
	}
	char* src_filename;
	char* dst_filename;
	int num_threads = (argc - 1) / 2; // number of threads to create
	char* src_filenames[num_threads]; // array of source files
	char* dst_filenames[num_threads]; // array of desintation files

	src_filenames[0] = argv[1];
	src_filenames[1] = argv[3];
	src_filenames[2] = argv[5];
	src_filenames[3] = argv[7];
	src_filenames[4] = argv[9];
	src_filenames[5] = argv[11];
	src_filenames[6] = argv[13];
	src_filenames[7] = argv[15];
	src_filenames[8] = argv[17];
	src_filenames[9] = argv[19];

	dst_filenames[0] = argv[2];
	dst_filenames[1] = argv[4];
	dst_filenames[2] = argv[6];
	dst_filenames[3] = argv[8];
	dst_filenames[4] = argv[10];
	dst_filenames[5] = argv[12];
	dst_filenames[6] = argv[14];
	dst_filenames[7] = argv[16];
	dst_filenames[8] = argv[18];
	dst_filenames[9] = argv[20];
	
	pthread_t threads[num_threads]; // initialize threads
	struct copy_struct thread_params[num_threads]; // structure for each thread
	int i;
	for (i = 0; i < num_threads; ++i) {
		// initialize thread parameters
		thread_params[i].thread_id = i;
		thread_params[i].src_filename = src_filenames[i];
		thread_params[i].dst_filename = dst_filenames[i];
		
		// create each copy thread
		// use pthread_create(.....);
		if (pthread_create(&threads[i], NULL, copy_thread, &thread_params[i]) != 0) {
			perror("pthread_create error");
			exit(3);
		}
	}

	// wait for all threads to finish
	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
	}
	pthread_exit(NULL);
}