#include <pthread.h>
#include <stdio.h>

void* thread_func() {
    char *result = "NULL";
    printf("Hello from the thread!\n");
    return (void*)result;
}


int main()
{
    pthread_t t;
    char *result;

    pthread_create(&t, NULL, thread_func, NULL);
    pthread_join(t, result);
    printf("%s", result);
}
