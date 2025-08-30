#ifndef SEQUENCER_H
# define SEQUENCER_H

#include "parser.h"
#include <sys/time.h>	// gettimeofday()
#include <strings.h>	// bzero()
// #include <stdint.h>	// for fixed width integer data types

# define GET_TIME_FAILURE "A function which allows calculation of the elapsed \
time has failed. Aborting the program\n"

void	sound_generator(t_info *info);

#endif
