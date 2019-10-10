#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void * thread(){

	while(1){
		printf("%u thread!\n", (unsigned int) pthread_self());
	}
}

int main() {
	pthread_t tid1, tid2;

	pthread_create(&tid1, NULL, thread, NULL);
	pthread_create(&tid2, NULL, thread, NULL);
	pthread_join(tid1, NULL);
	pthread_join(tid1, NULL);

	return 0;

}
