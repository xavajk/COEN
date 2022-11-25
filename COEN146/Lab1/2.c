/*	# Name: Xavier 
	# Date: 3/30/22 
	# Title: Lab1 – Basic Linux, network commands, and C programming skills
	# Description: This program copies one file (src_filename) to another (dst_filename) using functions
    # such as read(), write(), open(), close().
*/

#include <stdio.h>   // fprintf(), fread(), fwrite(), fopen(), fclose()
#include <stdlib.h>  // malloc(), free()
#include <pthread.h> // pthread_create()
#include <unistd.h>  // read(), write()
#include <fcntl.h>   // open(), close()
#include <errno.h>   // errno
#include <time.h>    // clock()
#include <sys/mman.h> // mmap()
#include<sys/types.h>
#include<sys/stat.h>

#define BUF_SIZE 2048 //buffer size

// copies a files form src_filename to dst_filename using syscalls open(), read(), write(), close()
int syscall_copy(char* src_filename, char* dst_filename)  {
	
	int src_fd = open(src_filename, O_RDONLY);	// opens a file for reading
	if (src_fd < 0) { // open() error checking
		fprintf(stderr, "unable to open %s for reading: %i\n", src_filename, errno);
		close(src_fd);
		exit(0);
	}
	// O_WRONLY - write only
	// O_CREAT - create file if it doesn't exist
	// O_TRUNC - truncates file to length 0 if it already exists (deletes existing file)
	// opens a file for writing; erases old file/creates a new file
	int dst_fd = open(dst_filename, O_WRONLY | O_CREAT | O_TRUNC);	
	if (dst_fd < 0) { // open() error checking
		fprintf(stderr, "unable to open/create %s for writing: %i\n", dst_filename, errno);
		close(dst_fd);
		exit(0);
	}


	// read/write loop
	char* buf = malloc((size_t)BUF_SIZE);   // allocate a buffer to store read data
	// reads up to BUF_SIZE bytes from src_filename
    ssize_t bytes_read = read(src_fd, buf, BUF_SIZE);
	// writes bytes_read to dst_filename
	write(dst_fd, buf, bytes_read);
	// closes src_fd file descriptor
    close(src_fd);
	// closes dst_fd file descriptor
    close(dst_fd);
	// frees memory used for buf
    free(buf);

	return 0;
}

int main(int argc, char* argv[]) {
	if (argc != 3) {  // check correct usage
		fprintf(stderr, "usage: %s <src_filename> <dst_filename>\n", argv[0]);
		exit(1);
	}
	//get the source and destination files from the command line arguments
	char* src_filename = argv[1];
	char* dst_filename = argv[2];

	return 0;
}