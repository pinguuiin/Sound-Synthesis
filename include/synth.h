#ifndef SYNTH_H
#define SYNTH_H

#include "parser.h"

#define SAMPLE_RATE			48000
#define FRAMES_PER_BUFFER	256
#define TABLE_SIZE			2048

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
	t_info	*info;
	float	*mixbuffer;
	int		num_voices;
	t_synth	*synths;
}	t_mixer;

void	init_synth(t_info *info, t_mixer *mixer);
void	synth(t_mixer *mixer);
void	set_note(t_synth *synth, float freq, double amplitude);
void	create_mixer(t_info *info, t_mixer *mixer, int num_voices);
void	add_synth_to_mixer(t_mixer *mixer, t_synth synth, int voice_index);
void	choose_waveform(float *wavetable, t_track_type waveform_type);
void	render_synth_to_buffer(t_synth *synth, t_mixer *mixer);
int		destroy_mixer_and_info(t_mixer *mixer);
#endif
