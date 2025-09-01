#include "sequencer.h"

static double	get_current_time(double start_time);
static int		play_music(int num_tracks, double start_time, t_track *tracks, t_mixer *mixer);
static void		play_first_note(int num_tracks, t_track *tracks, t_synth **synths);

void	sequencer(t_info *info, t_mixer *mixer)
{
	double	start_time;
	printf("Sequencing...\n");
	// initialize variables to zero:
	// important since it is passed to get_current_time()
	start_time = 0;

	// set the starting time
	start_time = get_current_time(start_time);
	if (start_time == -1)
	{
		write(2, GET_TIME_FAILURE, sizeof(GET_TIME_FAILURE) - 1);
		synth_destroy(mixer);
		exit (free_info(info));
	}

	// main music loop
	if (play_music(info->num_tracks, start_time, info->tracks, mixer) == -1)
	{
		write(2, GET_TIME_FAILURE, sizeof(GET_TIME_FAILURE) - 1);
		synth_destroy(mixer);
		exit (free_info(info));
	}
}

// When called for the starting time, returned time will be in microseconds
// since the Epoch.
// For all other calls, it will be in microseconds since start_time.
static double	get_current_time(double start_time)
{
	struct timeval	time;

	bzero(&time, sizeof(struct timeval));
	if (gettimeofday(&time, NULL))
		return (-1);
	return ((double)time.tv_sec * 1000000 + time.tv_usec - start_time);
}

// returns 0 upon success, and -1 upon gettimeofday() failure
static int	play_music(int num_tracks, double start_time, t_track *tracks, t_mixer *mixer)
{
	int		n_done_playing;
	double	current_time;
	int		i;
	t_track	*current_track;
	t_synth	*current_synth;

	n_done_playing = 0;
	i = 0;

	play_first_note(num_tracks, tracks, mixer->synths);
	while (n_done_playing < num_tracks)
	{
		current_time = get_current_time(start_time);
		if (current_time == -1)
			return (-1);
		while (i < num_tracks)
		{
			current_track = &tracks[i];

			if (current_track->temp)
			{
				if ((current_time - current_track->time_last_note_began)
					>= current_track->temp->duration)
				{
					current_synth = mixer->synths[i];
					set_note(current_synth, current_track->temp->f, 0.0f); // cut the present note, even if it is a rest.
					current_track->temp = current_track->temp->next;
					if (current_track->temp)
					{
						if (current_track->temp->pitch != 'r')
							set_note(current_synth, current_track->temp->f, 1.0f);
						else
							set_note(current_synth, current_track->temp->f, 0.0f);
						current_track->time_last_note_began = current_time;
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

static void	play_first_note(int num_tracks, t_track *tracks, t_synth **synths)
{
	int	i;

	i = 0;
	while (i < num_tracks)
	{
		if (tracks[i].temp->pitch != 'r')
			set_note(synths[i], tracks[i].temp->f, 1.0f);
		else
			set_note(synths[i], tracks[i].temp->f, 0.0f);
		i++;
	}
}
