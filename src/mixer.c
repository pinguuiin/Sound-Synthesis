#include "midione.h"

void	create_mixer(t_info *info, t_mixer *mixer)
{
	mixer->info = info;
	mixer->mixbuffer = NULL;
	mixer->synths = NULL;
	mixer->mixbuffer = malloc(FRAMES_PER_BUFFER * sizeof(float));
	mixer->synths = malloc(info->num_voices * sizeof(t_synth));
	if (!mixer->mixbuffer || !mixer->synths)
		exit(destroy_mixer_and_info(mixer));
	for (int i = 0; i < info->num_voices; i++)
		mixer->synths[i].wavetable = NULL;
}

int	destroy_mixer_and_info(t_mixer *mixer)
{
	int	i;

	i = 0;
	if (mixer->synths)
	{
		while(i < mixer->info->num_voices)
		{
			if (mixer->synths[i].wavetable)
				free(mixer->synths[i].wavetable);
			i++;
		}
		free(mixer->synths);
	}
	if (mixer->mixbuffer)
		free(mixer->mixbuffer);
	free_info(mixer->info);
	return (1);
}
