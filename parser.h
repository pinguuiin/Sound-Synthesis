#ifndef PARSER_H
# define PARSER_H

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

// Type of supported wave forms
typedef enum e_track_type
{
	SINE,
	SAW,
	TRIANGLE,
	SQUARE
}	t_track_type;

// File reading position indicator
typedef enum e_file_pos
{
	NAME,
	TRACKS,
	SIDENOTE,
	NOTES
}	t_file_pos;

// Information for one note
typedef struct s_note
{
	char			pitch; // from 'a' to 'g', or 'r' representing a rest
	char			alteration; // '#' or 'b'
	int				octave; // 0 to 9
	float			duration; // duration in beats
	struct s_note	*next;
	struct s_note	*before;
}	t_note;

// Description and notes contained in each track
typedef struct s_track
{
	int				id;
	t_track_type	type;
	char			*sidenote;
	t_note			*notes;
}	t_track;

// Information provided by the music sheet
typedef struct s_info
{
	char		*name;
	int			tempo;
	int			num_tracks;
	t_track		*tracks;
	t_file_pos	file_pos;
}	t_info;

#endif
