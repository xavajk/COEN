/* 
* Name: Xavier Kuehn
* Date: 04/06/2022
* Title: Lab2 – Step 4 
* Description: computes RTT times for retrieving files (objects) with n parallel connections; persistent
and non-persistent HTTP 
*/

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
        if (argc != 2) {  // check correct usage
		fprintf(stderr, "usage: %s <n connections> \n", argv[0]);
		exit(1);
	}
        // Declare and values to n (n connections), np (np parralel connections), RTT0, RTT1, RTT2, RTTHTTP, RTTDNS, .. 
        int n = atoi(argv[1]), np;
        double RTT0 = 3;
        double RTT1 = 20;
        double RTT2 = 26;
        double RTTDNS = 49;
        double RTTHTTP = 47;

        printf("One object: %f msec\n", RTTDNS + 2*RTTHTTP);
        printf("Non-Persistent 6 objects: %f msec\n", RTTDNS + 2*(6 + 1)*RTTHTTP);

        // find how many np (parralel connections)
        // number of parallel connections equals number of objects + 1
        np = n + 1;
        printf("%d parallel connections - Persistent: %f msec\n", n, RTTDNS + 2*(n - 1)*RTTHTTP);
        printf("%d parallel connections - Non-Persistent: %f msec\n", np, RTTDNS + 2*(np + 1)*RTTHTTP);
        return 0;
}