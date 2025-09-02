#ifndef MIDIONE_H
# define MIDIONE_H

#include "synth.h"
#include <sys/time.h>	// gettimeofday()
#include <strings.h>	// bzero()
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

void	sequencer(t_info *info, t_mixer *mixer);

#endif
