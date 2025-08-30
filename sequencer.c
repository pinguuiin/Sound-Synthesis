#include <sequencer.h>

static int64_t	get_current_time(void);

// WARN: For each and every note:
// 		1.	Determine the "attack" moment and the "release / cut" moment.
// 			Note that the moment of release should be a little bit BEFORE the
// 			next note.
// 		2.	Send those two to the sound-machine part
// 			OR:
// 			send the note itself, including the information about the 'attack'
// 			and 'release'?
int	sound_generator(t_info info)
{
	int64_t	starting_time;
	int64_t	current_time;

	starting_time = 0;
	starting_time = get_current_time(starting_time);
	if (starting_time == -1)
		return (-1);	// TODO: cleanup the memory, return -1
	current_time = starting_time;


}

// used for:	- getting the starting time
// 				- getting the current time
static int64_t	get_current_time(int64_t starting_time)
{
	struct timeval	time;

	bzero(&time, sizeof(struct timeval));
	if (gettimeofday(&time, NULL))
		return (-1);
	return (time.tv_sec * 1000000 + time.tv_usec - starting_time);
}



// NOTE: the sound function is: void	set_note(<instrument>, <volume>);
// If volume is 0.5, the instrument will play the note.
// If volume is 0, the instrument will stop playing the note.
