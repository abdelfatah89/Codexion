#include "../include/codexion.h"

int main(int ac, char **av){

    t_args *args;

    args = parser(ac, av);
    if (!args)
        return (-1);
    printf("%d\n", args->dongle_cooldown);
    free(args);

}
