#include "../include/codexion.h"

t_args *parser(int argc, char **argv){
    t_args *args = malloc(sizeof(t_args));
    int *arr_int[6];
    int i = 0;
    char *schedular_type;
    if (!args) return NULL;

    if (argc < 9) {
        free(args);
        return NULL;
    }
    arr_int[0] = atoi(argv[1]);
    arr_int[1] = atoi(argv[2]);
    arr_int[2] = atoi(argv[3]);
    arr_int[3] = atoi(argv[4]);
    arr_int[4] = atoi(argv[5]);
    arr_int[5] = atoi(argv[6]);
    arr_int[6] = atoi(argv[7]);
    schedular_type = argv[8];

    while (!arr_int[i])
    {
        if (arr_int[i] < 0){
            return NULL;
        }
    }
    

    return args;
}
