#include "synth.h"

t_synth	*create_synth(t_track_type waveform_type)
{
	t_synth	*synth;

	synth = malloc(sizeof(t_synth));
	synth->wavetable = malloc(TABLE_SIZE * sizeof(float));
	synth->phase = 0.0;
	synth->phaseIncrement = 0.1;
	synth->amplitude = 0.0;
	synth->frequency = 0.1;
	choose_waveform(synth->wavetable, waveform_type);
	return (synth);
}

t_synth	*create_synth_from_type(t_track_type type)
{
	if (type == SINE)
		return create_synth(SINE);
	else if (type == SQUARE)
		return create_synth(SQUARE);
	else if (type == TRIANGLE)
		return create_synth(TRIANGLE);
	else if (type == SAW)
		return create_synth(SAW);
	else
		return create_synth(SINE);
}

void	set_note(t_synth *synth, float freq, double amplitude)
{
	synth->frequency = freq;
	synth->phaseIncrement = freq / SAMPLE_RATE;
	synth->amplitude = amplitude;
}

void	render_synth_to_buffer(t_synth *synth, t_mixer *mixer)
{
	int		sample_index;
	float	*output_buffer;

	sample_index = 0;
	output_buffer = mixer->mixbuffer;
	while(sample_index < FRAMES_PER_BUFFER)
	{
		int wt_idx = (int)(synth->phase * TABLE_SIZE) % TABLE_SIZE;
		*output_buffer += synth->wavetable[wt_idx] * synth->amplitude * (1.0f / (float) mixer->num_voices);
		output_buffer++;
		synth->phase += synth->phaseIncrement;
		if(synth->phase >= 1.0)
			synth->phase -= 1.0;
		sample_index++;
	}
}

static int paCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags, void *userData)
{
	(void)inputBuffer;
	(void)timeInfo;
	(void)statusFlags;

	t_mixer	*mixer;
	float	*out;
	int		i;

	mixer = (t_mixer*)userData;
	out = (float*)outputBuffer;
	memset(mixer->mixbuffer, 0, framesPerBuffer * sizeof(float));
	i = 0;
	while(i < mixer->num_voices)
	{
		if(mixer->synths[i] != NULL)
			render_synth_to_buffer(mixer->synths[i], mixer);
		i++;
	}
	memcpy(out, mixer->mixbuffer, framesPerBuffer * sizeof(float));
	return (paContinue);
}

t_mixer	*synth_init(t_info *info)
{
	int	i;
	t_synth *synth;
	t_mixer	*mixer;
	
	// Create mixer with parsed num_tracks
	mixer = create_mixer(info);
	
	// Initialize PortAudio
	Pa_Initialize();
	
	// Create synths for each track
	i = 0;
	while(i < info->num_tracks)
	{
		synth = create_synth(info->tracks[i].type);
		add_synth_to_mixer(mixer, synth, i);
		i++;
	}
	Pa_OpenDefaultStream(&mixer->stream, 0, 1, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, paCallback, mixer);
	Pa_StartStream(mixer->stream);
	return (mixer);
}

void	synth_destroy(t_mixer *mixer)
{
	if (!mixer) return ;
	
	Pa_StopStream(mixer->stream);
	Pa_CloseStream(mixer->stream);
	Pa_Terminate();
	destroy_mixer_and_synths(mixer);
}
