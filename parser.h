#ifndef PARSER_H
# define PARSER_H

#include "portaudio.h"
#include "parser.h"
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>

#define SAMPLE_RATE			48000
#define FRAMES_PER_BUFFER	256
#define TABLE_SIZE			2048
//#define NUM_VOICES			4

// Type of supported wave forms
typedef enum e_track_type
{
	SINE,
	SAW,
	TRIANGLE,
	SQUARE
}	t_track_type;

// File reading position indicator - for parsing use
typedef enum e_file_pos
{
	NAME,
	TEMPO,
	TRACKS,
	NOTES
}	t_file_pos;


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


// Information for one note
typedef struct s_note
{
	float			f;  // frequency in Hz
	char			pitch; // from 'a' to 'g', or 'r' representing a rest
	char			alteration; // '#' or 'b', '-' if no alteration
	int				octave; // 0 to 9
	double			duration; // duration in microseconds
	struct s_note	*next;
	struct s_note	*temp; // serves the sequencer: it needs to keep track of each voice's current note, without losing the original *note.
}	t_note;

// Description and notes contained in each track
typedef struct s_track
{
	int				id;
	t_track_type	type;
	char			*sidenote;
	t_note			*note;
	int				num_notes;
	int				begin;  //beginning of line, for octave and duration reset in parsing
	double			time_last_note_began;
}	t_track;

// Information provided by the music sheet
typedef struct s_info
{
	FILE		*fd;
	char		*line;  //current line been read
	char		*name;
	int			tempo;
	double		beat_to_usec;
	int			num_tracks;
	int			now_track;  // current track the reading position at
	t_track		*tracks;
	t_file_pos	file_pos;
}	t_info;

int		free_info(t_info *info);
void	parser(t_info *info);
void	processor(t_info *info);
char	*ft_strjoin(char const *s1, char const *s2);
void	sequencer(t_info *info, t_mixer *mixer);

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

