
#ifndef SEQUENCER_H
# define SEQUENCER_H

// #include <parser.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <strings.h>	// bzero()
#include <inttypes.h>	// to print fixed width integer data types
#include <unistd.h>		// usleep()


#endif


static int64_t	get_current_time(int64_t starting_time);
static int	sound_generator(void);


int	main(void)
{

	if (sound_generator())
		printf("AN ERROR HAS OCCURED?\n\n\n");



	return (0);
}


static int	sound_generator(void)
{
	int64_t	starting_time;
	int64_t	current_time;

	starting_time = 0;
	starting_time = get_current_time(starting_time);
	if (starting_time == -1)
		return (-1);	// TODO: cleanup the memory, return -1
	current_time = starting_time;

	printf("CURRENT TIME IS:	<%"PRId64">\n\n", current_time);

	usleep(10000);

	current_time = get_current_time(starting_time);



	printf("CURRENT TIME IS:	<%"PRId64">\n\n", current_time);

	usleep(10000);

	current_time = get_current_time(starting_time);


	printf("CURRENT TIME IS:	<%"PRId64">\n\n", current_time);

	usleep(10000);

	current_time = get_current_time(starting_time);

	printf("CURRENT TIME IS:	<%"PRId64">\n\n", current_time);

	usleep(10000);

	current_time = get_current_time(starting_time);

	printf("CURRENT TIME IS:	<%"PRId64">\n\n", current_time);

	usleep(10000);

	current_time = get_current_time(starting_time);

	printf("CURRENT TIME IS:	<%"PRId64">\n\n", current_time);

	usleep(10000);

	current_time = get_current_time(starting_time);

	printf("CURRENT TIME IS:	<%"PRId64">\n\n", current_time);

	return (0);

}

static int64_t	get_current_time(int64_t starting_time)
{
	struct timeval	time;

	bzero(&time, sizeof(struct timeval));
	if (gettimeofday(&time, NULL))
		return (-1);
	return (time.tv_sec * 1000000 + time.tv_usec - starting_time);
}
