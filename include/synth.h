#ifndef SYNTH_H
# define SYNTH_H

#include "parser.h"

#define SAMPLE_RATE			48000
#define FRAMES_PER_BUFFER	640  //Need to be larger than ATTACK + DECAY and large than RELEASE
#define BUFFER_SIZE			13333.333  // FRAMES_PER_BUFFER / SAMPLE_RATE * 1000000
#define STAGE_TIME			300
#define STAGE_SLOPE			- 5.0f
#define TABLE_SIZE			2048

typedef struct s_synth
{
	double	phase;
	double	phaseIncrement;
	float	*wavetable;
	double	frequency;
	double	amplitude;
	int		is_press;  // 1: press, 0: sustain pressing, -1: release, -2: released
	int		is_release; // 1: is being released, 0: otherwise
}	t_synth;

typedef struct s_mixer
{
	t_info		*info;
	float		envelope_table[4][FRAMES_PER_BUFFER];
	float		*mixbuffer;
	t_synth		*synths;
	PaStream	*stream;
}	t_mixer;

void	init_synth(t_info *info, t_mixer *mixer);
void	set_note(t_synth *synth, float freq, double amplitude, int press_flag);
void	create_mixer(t_info *info, t_mixer *mixer);
void	choose_waveform(float *wavetable, t_track_type waveform_type);
void	render_synth_to_buffer(t_synth *synth, t_mixer *mixer);
void	destroy_stream(t_mixer *mixer);
int		destroy_mixer_and_info(t_mixer *mixer);

#endif
