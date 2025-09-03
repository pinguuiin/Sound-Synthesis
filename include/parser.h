#ifndef PARSER_H
# define PARSER_H

#include <stdio.h>

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

// Information for one note
typedef struct s_note
{
	float			f;  // frequency in Hz
	char			pitch; // from 'a' to 'g', or 'r' representing a rest
	char			alteration; // '#' or 'b', '-' if no alteration
	int				octave; // 0 to 9
	double			duration; // duration in microseconds
	struct s_note	*next;
}	t_note;

// Description and notes contained in each track
typedef struct s_track
{
	int				id;
	t_track_type	type;
	char			*sidenote;
	t_note			*note;
	int				begin;  //beginning of line, for octave and duration reset in parsing
	double			time_last_note_began;
	struct s_note	*temp;
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
	int			i_now;  // current track the reading position at
	t_track		*tracks;
	t_file_pos	file_pos;
}	t_info;

int		free_info(t_info *info);
void	parser(t_info *info);
void	processor(t_info *info);
char	*ft_strjoin(char const *s1, char const *s2);
void	parse_waveform_list(t_info *info, char **line, int i);
void	switch_to_real_track(t_info *info, int real_track);

#endif
