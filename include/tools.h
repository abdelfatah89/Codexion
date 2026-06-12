#ifndef TOOLS_H
#define TOOLS_H

typedef struct s_args {
    int     number_of_coders;
    int     time_to_burnout;
    int     time_to_compile;
    int     time_to_debug;
    int     time_to_refactor;
    int     number_of_compiles_required;
    int     dongle_cooldown;
    char *scheduler;
}   t_args;

t_args *parser(int argc, char **argv);

#endif
