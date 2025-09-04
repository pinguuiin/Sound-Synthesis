#ifndef MINISYNTH_H
# define MINISYNTH_H

#include "portaudio.h"
#include "parser.h"
#include "synth.h"
#include <sys/time.h>	// gettimeofday()
#include <strings.h>	// bzero()
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

void	sequencer(t_info *info, t_mixer *mixer);

#endif
