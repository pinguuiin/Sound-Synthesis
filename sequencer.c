#include <sequencer.h>

static int64_t	get_current_time(int64_t start_time);
static int		play_music(t_info *info, int64_t start_time, float beat_to_usec);

// WARN: For each and every note:
// 		Determine the "attack" moment and the "release / cut" moment.
// 		Note that the moment of release should be a little bit BEFORE the
// 		next note.
// 		NOTE: some music sheets have repeated notes, non-separated by rests!!
// 		See for example Nightcall.synth, track number 14!
// 		This implies we have to cut the note before playing the next one, which
// 		in any case is not that bad.
void	sound_generator(t_info *info)
{
	float	beat_to_usec;
	int64_t	start_time;

	// initialize variables to zero.
	start_time = 0;

	// calculate the factor which will allow conversion of note duration into
	// milliseconds
	beat_to_usec = 60 / info->tempo * 1000000;


	// set the starting time
	start_time = get_current_time(start_time);
	if (start_time == -1)
	{
		write(2, GET_TIME_FAILURE, sizeof(GET_TIME_FAILURE) - 1);
		free_info(info);
		exit (1);
	}

	// main music loop
	if (play_music(info, start_time, beat_to_usec) == -1)
	{
		write(2, GET_TIME_FAILURE, sizeof(GET_TIME_FAILURE) - 1);
		free_info(info);
		exit (1);
	}
}

// returns 0 upon success, and -1 upon gettimeofday() failure
static int	play_music(t_info *info, int64_t start_time, float beat_to_usec)
{
	size_t	n_done_playing;
	int64_t	current_time;
	size_t	i;
	t_note	*temp;

	n_done_playing = 0;
	i = 0;
	current_time = start_time;

	// main loop
	while (n_done_playing < info->num_tracks)
	{
		// go thorugh each track, and check if that track should be playing a new note.
		// If it finished its note, move the t_note linked list pointer into the next pointer.
		// then check if that pointer is valid;
		// 		- if it is:	call set_note() to play that note.
		// 		- else:		increment n_done_playing!

		// NOTE: usage of the sound function:  void	set_note(<instrument>, <volume>);
		// If volume is 0.5, the instrument will play the note.
		// If volume is 0, the instrument will stop playing the note.


		while (i < info->num_tracks)
		{
			if (info->tracks[i]->note)
			{
				// TODO: how do I keep track of the notes, without moving the
				// initial note pointer which we need in order to free the
				// memory properly?

			}



			// increment n_done_playing once a track's notes linked list is pointing to NULL.
			// remember that some tracks are shorter than others! That's why this is crucial.
			if (!info->tracks[i].note)
				n_done_playing++;
			i++;
		}
		i = 0;			// reset i to zero, so we can restart the loop (if everybody did not finish playing yet)
		usleep(1000);	// sleep in order to avoid unnecessary spinning by system resources
	}
	return (0);
}

// used for:	- getting the starting time
// 				- getting the current time
static int64_t	get_current_time(int64_t start_time)
{
	struct timeval	time;

	bzero(&time, sizeof(struct timeval));
	if (gettimeofday(&time, NULL))
		return (-1);
	return (time.tv_sec * 1000000 + time.tv_usec - start_time);
}
