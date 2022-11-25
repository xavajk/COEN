//COEN 146L : Lab3, step 3: concurrent TCP server that accepts and responds to multiple client connection requests, each requesting a file transfer
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>

//Define the number of clients/threads that can be served
#define N 5
int threadCount = 0;
pthread_t clients[N]; //declaring N threads

// Declare socket and connection file descriptors.
int sockfd, connfd, rb, sin_size;

// Declare receiving and sending buffers of size 10 bytes
char rbuf[10], sbuf[10];

// Declare server address to which to bind for receiving messages and client address to fill in sending address
struct sockaddr_in servAddr, clntAddr;

// Connection handler (thread function) for servicing client requests for file transfer
void* connectionHandler(void* sock){
   // Declare buffer holding the name of the file from client
   char fclient[50];
   
   // Connection established, server begins to read and write to the connecting client
   printf("Connection Established with client IP: %s and Port: %d\n", inet_ntoa(clntAddr.sin_addr), ntohs(clntAddr.sin_port));
   
	// Receive name of the file from the client
	if (recv(connfd, fclient, sizeof(fclient), 0) < 0) {
      perror("File could not be read from client");
      exit(1);
   }

   // Open file and send to client
   int fd = open(fclient, O_RDWR | O_CREAT);
   send(fd, sbuf, sizeof(sbuf), 0);
   
   // Read file and send to connection descriptor
   while ((rb = read(connfd, sbuf, sizeof(sbuf))) > 0){
      write(connfd, sbuf, sizeof(sbuf));
   }
   printf("File transfer complete\n");
   
   // Close file
   close(fd);
   
   // Close connection descriptor
   close(connfd);
   
   return 0;
}

int main(int argc, char *argv[]) {
   // Get from the command line, server IP, src and dst files.
   if (argc != 2){
	   printf ("Usage: %s <port #> \n",argv[0]);
	   exit(0);
   } 
   
   // Open a TCP socket, if successful, returns a descriptor
   if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("Failure to set up an endpoint socket");
      exit(1);
   }

   // Setup the server address to bind using socket addressing structure
   servAddr.sin_family = AF_INET;
   servAddr.sin_port = htons(atoi(argv[1]));
   servAddr.sin_addr.s_addr = INADDR_ANY;

   // Bind IP address and port for server endpoint socket
   if ((bind(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) < 0) {
      perror("Failure to bind server address to the endpoint socket");
      exit(1);
   }

   // Server listening to the socket endpoint, and can queue 5 client requests
   printf("Server listening/waiting for client at port %s\n", argv[1]);
   listen(sockfd, 5);
   sin_size = sizeof(struct sockaddr_in);
 
   while (1) {
   // Server accepts the connection and call the connection handler
   if ((connfd = accept(sockfd, (struct sockaddr *)&clntAddr, (socklen_t *)&sin_size)) < 0) {
      perror("Failure to accept connection to client");
      exit(1);
   }
   if (pthread_create(&clients[threadCount], NULL, connectionHandler, (void*) &connfd) < 0) {
      perror("Unable to create a thread");
      exit(0);
   }
   else 
      printf("Thread %d has been created to service client request\n",++threadCount);
   }
   for (int i = 0; i < threadCount; ++i) {
      pthread_join(clients[i], NULL);
   }
   return 0;
}