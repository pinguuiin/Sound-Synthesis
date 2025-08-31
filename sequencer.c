#include "sequencer.h"

static int64_t	get_current_time(int64_t start_time);
static int		play_music(t_info *info, int64_t start_time, t_track *tracks);
static void		play_first_note(int num_tracks, t_track *tracks);

void	sound_generator(t_info *info)
{
	int64_t	start_time;

	// initialize variables to zero:
	// important since it is passed to get_current_time()
	start_time = 0;

	// set the starting time
	start_time = get_current_time(start_time);
	if (start_time == -1)
	{
		write(2, GET_TIME_FAILURE, sizeof(GET_TIME_FAILURE) - 1);
		exit (free_info(info));
	}

	// main music loop
	if (play_music(info, start_time) == -1)
	{
		write(2, GET_TIME_FAILURE, sizeof(GET_TIME_FAILURE) - 1);
		exit (free_info(info));
	}
}

// When called for the starting time, returned time will be in microseconds
// since the Epoch.
// For all other calls, it will be in microseconds since start_time.
static int64_t	get_current_time(int64_t start_time)
{
	struct timeval	time;

	bzero(&time, sizeof(struct timeval));
	if (gettimeofday(&time, NULL))
		return (-1);
	return (time.tv_sec * 1000000 + time.tv_usec - start_time);
}

// returns 0 upon success, and -1 upon gettimeofday() failure
static int	play_music(t_info *info, int64_t start_time, t_track *tracks)
{
	size_t	n_done_playing;
	int64_t	current_time;
	size_t	i;
	t_note	*temp;

	n_done_playing = 0;
	i = 0;

	play_first_note(info->num_tracks, tracks);
	while (n_done_playing < info->num_tracks)
	{
		// NOTE: usage of the sound function:
		// void	set_note(mixer->synths[voice_number], pitch, volume);
		// If volume is 0.5, the instrument will play the note.
		// If volume is 0, the instrument will stop playing the note.

		while (i < info->num_tracks)
		{
			current_time = get_current_time(start_time);
			if (current_time == -1)
				return (-1);
			if (tracks[i].note->temp)
			{
				if ((current_time - tracks[i].time_last_note_began)
					>= tracks[i].note->duration)
				{
					set_note(????, 0.0); // cut the present note, even if it is a rest.  // TODO: get the prototype and complete
					tracks[i].note->temp = tracks[i].note->next;
					if (tracks[i].note->temp)
					{
						if (tracks[i].note->pitch != "r")
							set_note(tracks[i].note->temp, 0.5); // TODO: get the prototype and complete
						else
							set_note(tracks[i].note->temp, 0.0); // TODO: get the prototype and complete
						tracks[i].time_last_note_began = current_time;
					}
					else
						n_done_playing++;
				}
			}
			i++;
		}
		i = 0;
		usleep(1000);	// sleep in order to avoid unnecessary spinning by system resources
	}
	return (0);
}

static void	play_first_note(int num_tracks, t_track *tracks)
{
	size_t	i;

	i = 0;
	while (i < num_tracks)
	{
		if (tracks[i].note->pitch != "r")
			set_note(tracks[i].note->temp, 0.5); // TODO: get the prototype and complete
		else
			set_note(tracks[i].note->temp, 0.0); // TODO: get the prototype and complete
		i++;
	}
}
