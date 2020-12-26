#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <pthread.h>

//Encrypted password to be cracked
char *passwords_enc ="$6$AS$qCWu88.ZluOwA.i2IHWzccFJtGTCRCzJLFln4nEczR1Cq8iGQ8I.3cte77q2X8chQjtO/YNDJ9bjwug2DI5ow0";

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}


void myThread(){
int i;
pthread_t SS1, SS2;

void *kernel_function_1();
void *kernel_function_2();

pthread_create(&SS1, NULL,kernel_function_1,passwords_enc);
pthread_create(&SS2, NULL,kernel_function_2,passwords_enc);

pthread_join(SS1, NULL);
pthread_join(SS2,NULL);
}

void *kernel_function_1(char *salt_and_encrypted){
  int S,H,R;  
  char salt[7];   
  char plain[7]; 
  char *enc;   
  int count = 0; 
  substr(salt, salt_and_encrypted, 0, 6);

  for(S='A';S<='M'; S++){
    for(H='A'; H<='Z'; H++){
      for(R=0; R<=99; R++){
        sprintf(plain, "%c%c%02d", S,H,R); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
	    printf("#%-8d%s %s\n", count, plain, enc);  
		//uncomment this line if you want to speed-up the running time, program will find you the cracked password only without exploring all possibilites
        } 
      }
    }
  }
}

void *kernel_function_2(char *salt_and_encrypted){
  int g,a,m;     
  char salt[7];    
  char plain[7]; 
  char *enc;  
  int count = 0;

  substr(salt, salt_and_encrypted, 0, 6);

  for(g='N';g<='Z'; g++){
    for(a='A'; a<='Z'; a++){
      for(m=0; m<=99; m++){
        sprintf(plain, "%c%c%02d", g,a,m); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
	    printf("#%-8d%s %s\n", count, plain, enc);   
		//uncomment this line if you want to speed-up the running time, program will find you the cracked password only without exploring all possibilites
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


	myThread();


	clock_gettime(CLOCK_MONOTONIC, &endTime);
	calculateTimeDifference(&startTime, &endTime, &time_elapsed);
	printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
		(time_elapsed / 1.0e9));


	return 0;
}


