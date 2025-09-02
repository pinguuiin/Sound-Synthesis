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

void	sequencer(t_info *info, t_mixer *mixer);

#endif
