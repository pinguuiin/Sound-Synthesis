#ifndef MIDIONE_H
# define MIDIONE_H

#include "portaudio.h"
#include "parser.h"
#include "synth.h"
#include <sys/time.h>	// gettimeofday()
#include <strings.h>	// bzero()
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

# define GET_TIME_FAILURE "A function which allows calculation of the elapsed \
time has failed. Aborting the program\n"

void	sequencer(t_info *info, t_mixer *mixer);

#endif
