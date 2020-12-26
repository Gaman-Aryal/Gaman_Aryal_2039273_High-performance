#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

int count=0; 

void crackPassword(char *salt_and_encrypted){
  int g,a, m, n;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0 // incase you have modified the salt value, then should modifiy the number accordingly
  char plain[7];   // The combination of letters currently being checked // Please modifiy the number when you enlarge the encrypted password.
  char *enc;       // Pointer to the encrypted password

  substr(salt, salt_and_encrypted, 0, 6);

  for(g='A'; g<='Z'; g++){
    for(a='A'; a<='Z'; a++){
    for(m='A'; m<='Z'; m++){
      for(n=0; n<=99; n++){
        sprintf(plain, "%c%c%c%02d",g, a, m, n); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
	    printf("#%-8d%s %s\n", count, plain, enc);	    
		return;	//uncomment this line if you want to speed-up the running time, program will find you the cracked password only without exploring all possibilites
				// Program will end after cracking the password
		} 
      }
    }
    }
  }
}

int calculateTimeDifference(struct timespec *start, struct timespec *finish, 
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main() {
	int i;
	struct timespec startTime, endTime;
	long long int time_elapsed;

	clock_gettime(CLOCK_MONOTONIC, &startTime);

	crackPassword("$6$AS$CwBrLQ1CyrpbD4bPvLORPEK6engXmdFgjYWyVB4LPiaIYQ6uWbbdMcsjh8a9TdrQmeWeU4z3JxKHLIjvf.XIy/");

	printf("%d solutions explored\n", count);

	clock_gettime(CLOCK_MONOTONIC, &endTime);
	calculateTimeDifference(&startTime, &endTime, &time_elapsed);
	printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
		(time_elapsed / 1.0e9));


	return 0;
}
