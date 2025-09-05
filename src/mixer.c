#include "minisynth.h"

void	generate_envelope_table(t_mixer *mixer)
{
	int	i;

	for (i = 0; i < STAGE_TIME; i++)
		mixer->envelope_table[0][i] = 1.0f - expf(STAGE_SLOPE / STAGE_TIME * (float)i);
	for (; i < STAGE_TIME * 2; i++)
		mixer->envelope_table[0][i] = 0.6f + 0.4f * expf(STAGE_SLOPE /STAGE_TIME * (float)(i - STAGE_TIME));
	for (; i < FRAMES_PER_BUFFER; i++)
		mixer->envelope_table[0][i] = 0.6f;
	for (i = 0; i < FRAMES_PER_BUFFER; i++)
		mixer->envelope_table[1][i] = 0.6f;
	for (i = 0; i < STAGE_TIME; i++)
		mixer->envelope_table[2][i] = 0.6f * expf(STAGE_SLOPE / STAGE_TIME * (float)i);
	for (; i < FRAMES_PER_BUFFER; i++)
		mixer->envelope_table[2][i] = 0.0f;
	for (i = 0; i < FRAMES_PER_BUFFER; i++)
		mixer->envelope_table[3][i] = 0.0f;
}

void	create_mixer(t_info *info, t_mixer *mixer)
{
	mixer->info = info;
	mixer->mixbuffer = NULL;
	mixer->synths = NULL;
	mixer->mixbuffer = malloc(FRAMES_PER_BUFFER * sizeof(float));
	mixer->synths = malloc(info->num_tracks * sizeof(t_synth));
	if (!mixer->mixbuffer || !mixer->synths)
		exit(destroy_mixer_and_info(mixer));
	for (int i = 0; i < info->num_tracks; i++)
		mixer->synths[i].wavetable = NULL;
	generate_envelope_table(mixer);
}

int	destroy_mixer_and_info(t_mixer *mixer)
{
	int	i;

	i = 0;
	if (mixer->synths)
	{
		while(i < mixer->info->num_tracks)
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
