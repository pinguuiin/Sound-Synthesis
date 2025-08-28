#ifndef PARSER_H
# define PARSER_H

// Type of supported wave forms
typedef enum e_track_type
{
	SINE,
	SAW,
	TRIANGLE,
	SQUARE
}	t_track_type;

// Information for one note
typedef struct s_note
{
	char	pitch; // from 'a' to 'g', or 'r' representing a rest
	char	alteration; // '#' or 'b', '\0' if same as the previous one
	int		octave; // 0 to 9, -1 if same as the previous one
	float	duration; // duration in beats, 0 if same as the previous one
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
	char	*header;
	int		tempo;
	t_track	*tracks;
}	t_info;

#endif
