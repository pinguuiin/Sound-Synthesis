#include "synth.h"

t_synth	create_synth(t_mixer *mixer, t_track_type waveform_type)
{
	t_synth	synth;

	synth.wavetable = malloc(TABLE_SIZE * sizeof(float));
	if (!synth.wavetable)
		destroy_mixer_and_synths(mixer);
	synth.phase = 0.0;
	synth.phaseIncrement = 0.1;
	synth.amplitude = 0.0;
	synth.frequency = 0.1;
	choose_waveform(synth.wavetable, waveform_type);
	return (synth);
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
	while (sample_index < FRAMES_PER_BUFFER)
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

	mixer = (t_mixer *)userData;
	out = (float *)outputBuffer;
	memset(mixer->mixbuffer, 0, framesPerBuffer * sizeof(float));
	for (i = 0; i < mixer->num_voices; i++)
		render_synth_to_buffer(&(mixer->synths[i]), mixer);
	memcpy(out, mixer->mixbuffer, framesPerBuffer * sizeof(float));
	return (paContinue);
}

void	synth(t_info *info)
{
	int			i;
	t_synth 	synth;
	PaStream	*stream;
	t_mixer		mixer;

	Pa_Initialize(); //what if fail?
	create_mixer(info, &mixer, info->num_tracks);
	for (i = 0; i < info->num_tracks; i++)
	{
		synth = create_synth(&mixer, info->tracks[i].type);
		add_synth_to_mixer(&mixer, synth, i);
	}
	Pa_OpenDefaultStream(&stream, 0, 1, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, paCallback, &mixer);
	Pa_StartStream(stream);
	//PLACEHOLDERS will be called from Sequencer
	set_note(&(mixer.synths[0]), 440, 1);
	Pa_Sleep(1000);
	set_note(&(mixer.synths[1]), 125, 1);
	Pa_Sleep(500);

	Pa_StopStream(stream);
	Pa_CloseStream(stream);
	Pa_Terminate();
	destroy_mixer_and_synths(&mixer);
}
