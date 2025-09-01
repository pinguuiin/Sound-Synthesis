#ifndef SYNTH_H
#define SYNTH_H

#include "portaudio.h"
#include "parser.h"
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SAMPLE_RATE			48000
#define FRAMES_PER_BUFFER	256
#define TABLE_SIZE			2048
//#define NUM_VOICES			4

typedef struct s_synth
{
	double	phase;
	double	phaseIncrement;
	float	*wavetable;
	double	frequency;
	double	amplitude;
}	t_synth;

typedef struct s_mixer
{
	float		*mixbuffer;
	int			num_voices;
	t_synth		**synths;
	PaStream	*stream;
}	t_mixer;

t_mixer	*synth_init(t_info *info);
void	set_note(t_synth *synth, float freq, double amplitude);
void	synth_destroy(t_mixer *mixer);

// void	synth(t_info *info);
t_mixer	*create_mixer(t_info *info);
void	add_synth_to_mixer(t_mixer *mixer, t_synth *synth, int voice_index);
void	choose_waveform(float *wavetable, t_track_type waveform_type);
void	render_synth_to_buffer(t_synth *synth, t_mixer *mixer);
void	destroy_mixer_and_synths(t_mixer *mixer);
#endif
