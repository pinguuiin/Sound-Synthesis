#include "minisynth.h"

static double	get_current_time(double start_time);
static int		play_music(int num_tracks, double start_time, t_track *tracks,
					t_mixer *mixer);
static void		play_first_note(int num_tracks, t_track *tracks,
					t_synth *synths);
static void		cut_current_note(t_track *curr_track, t_synth *curr_synth);
static void		play_next_note(t_track *curr_track, double curr_time,
					t_synth *curr_synth, int *n_done_playing);

void	sequencer(t_info *info, t_mixer *mixer)
{
	double	start_time;

	start_time = 0;

	// set the starting time
	start_time = get_current_time(start_time);
	if (start_time == -1)
	{
		perror("gettimeofday");
		destroy_stream(mixer);
		exit(destroy_mixer_and_info(mixer));
	}

	// main music loop
	if (play_music(info->num_tracks, start_time, info->tracks, mixer) == -1)
	{
		perror("gettimeofday");
		destroy_stream(mixer);
		exit(destroy_mixer_and_info(mixer));
	}
}

/*
* When called for the starting time, returned time will be in microseconds
* since the Epoch.
* For all other calls, it will be since start_time (in microseconds as well).
*/
static double	get_current_time(double start_time)
{
	struct timeval	time;

	bzero(&time, sizeof(struct timeval));
	if (gettimeofday(&time, NULL))
		return (-1);
	return ((double)time.tv_sec * 1000000 + time.tv_usec - start_time);
}

// returns 0 upon success, and -1 upon gettimeofday() failure
static int	play_music(int num_tracks, double start_time, t_track *tracks,
				t_mixer *mixer)
{
	int		i;
	int		n_done_playing;
	double	curr_time;
	t_track	*curr_track;
	t_synth	*curr_synth;

	n_done_playing = 0;

	play_first_note(num_tracks, tracks, mixer->synths);
	while (n_done_playing < num_tracks)
	{
		curr_time = get_current_time(start_time);
		if (curr_time == -1)
			return (-1);
		i = 0;
		while (i < num_tracks)
		{
			curr_track = &tracks[i];
			if (curr_track->temp)
			{
				if ((curr_time - curr_track->time_last_note_began)
					>= curr_track->temp->duration - BUFFER_SIZE && mixer->synths[i].is_release == 0)
				{
					curr_synth = &mixer->synths[i];
					cut_current_note(curr_track, curr_synth);
					curr_synth->is_release = 1;
				}
				if ((curr_time - curr_track->time_last_note_began)
					>= curr_track->temp->duration)
				{
					curr_synth = &mixer->synths[i];
					play_next_note(curr_track, curr_time, curr_synth,
						&n_done_playing);
					curr_synth->is_release = 0;
				}
			}
			i++;
		}
	}
	return (0);
}

static void	play_first_note(int num_tracks, t_track *tracks, t_synth *synths)
{
	int	i;

	i = 0;
	while (i < num_tracks)
	{
		if (tracks[i].temp->pitch != 'r')
			set_note(&synths[i], tracks[i].temp->f, 1.0f, 1);
		else
			set_note(&synths[i], tracks[i].temp->f, 0.0f, 1);
		i++;
	}
}

static void	cut_current_note(t_track *curr_track, t_synth *curr_synth)
{
	// cut the presently played note, even if it is a rest
	set_note(curr_synth, curr_track->temp->f, 1.0f, -1);
}

static void	play_next_note(t_track *curr_track, double curr_time,
				t_synth *curr_synth, int *n_done_playing)
{
	// move pointer to the next note to be played
	curr_track->temp = curr_track->temp->next;
	if (curr_track->temp)
	{
		// start playing the next note
		if (curr_track->temp->pitch != 'r')
			set_note(curr_synth, curr_track->temp->f, 1.0f, 1);
		else
			set_note(curr_synth, curr_track->temp->f, 0.0f, 1);
		curr_track->time_last_note_began = curr_time;
	}
	else
		(*n_done_playing)++; // curr_track has played all of its notes!
}
