#include "parser.h"

static void	pitch_to_frequency(t_note *note)
{
	int	n_semi;

	switch (note->pitch) {
		case 'c':
			n_semi = -9;
			break ;
		case 'd':
			n_semi = -7;
			break ;
		case 'e':
			n_semi = -5;
			break ;
		case 'f':
			n_semi = -4;
			break ;
		case 'g':
			n_semi = -2;
			break ;
		case 'a':
			n_semi = 0;
			break ;
		case 'b':
			n_semi = 2;
			break ;
	}
	if (note->alteration == '#')
		n_semi += 1;
	else if (note->alteration == 'b')
		n_semi -= 1;
	n_semi += (note->octave - 4) * 12;
	note->f = 440 * powf((float)2, (float)n_semi / 12.0);
}

void	processor(t_info *info)
{
	t_note	*temp;

	for (int i = 0; i < info->num_tracks; i++){
		temp = info->tracks[i].note;
		while (temp){
			pitch_to_frequency(temp);
			temp = temp->next;
		}
	}
}