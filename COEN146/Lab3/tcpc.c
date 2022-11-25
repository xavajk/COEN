//COEN 146L : Lab3, step 1: TCP client requesting a file transfer from the server.
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
  // Get from the command line, server IP, src and dst files.
  if (argc != 5){
	  printf ("Usage: %s <ip of server> <port #> <src_file> <dst_file>\n",argv[0]);
	  exit(0);
  } 
  // Declare socket file descriptor and buffer
  int sockfd, rb;
  char* buf = malloc((size_t)10);

  // Declare server address to accept
  struct sockaddr_in servAddr;

  // Declare host
  struct hostent *host;

  // get hostname
  host = (struct hostent *) gethostbyname(argv[1]);

  // Open a socket, if successful, returns
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Failure to set up endpoint socket");
    exit(0);
  }

  // Set the server address to send using socket addressing structure
  servAddr.sin_addr = *((struct in_addr *)host->h_addr);
  servAddr.sin_port = htons(atoi(argv[2]));
  servAddr.sin_family = AF_INET;

  // Connect to the server
  if ((connect(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr)))) {
    perror("Faliure to connect to the server");
    exit(1);
  }
    
  // Client sends the name of the file to retrieve from the server
  if ((send(sockfd, argv[3], sizeof(argv[3]), 0)) < 0) {
    perror("Could not send file to be read from to server");
    exit(1);
  }

  // Client begins to write and read from the server
  int src_fd = open(argv[3], O_RDONLY);	// opens a file for reading
	if (src_fd < 0) { // open() error checking
		fprintf(stderr, "unable to open %s for reading: %i\n", argv[3], errno);
		close(src_fd);
		exit(0);
	}

	// opens a file for writing; erases old file/creates a new file
	int dst_fd = open(argv[4], O_RDWR | O_CREAT | O_TRUNC);	
	if (dst_fd < 0) { // open() error checking
		fprintf(stderr, "unable to open/create %s for writing: %i\n", argv[4], errno);
		close(dst_fd);
		exit(0);
	}

	// reads from src file
  ssize_t bytes_read;
  while ((bytes_read = read(sockfd, buf, 10)) > 0){
	  // writes bytes_read to dst file
	  write(dst_fd, buf, bytes_read);
  }

	// closes src_fd file descriptor
  // close(src_fd);
	// closes dst_fd file descriptor
  close(dst_fd);
	// frees memory used for buf
  free(buf);

  // Close socket descriptor
  close(sockfd);
    
  return 0;
}

