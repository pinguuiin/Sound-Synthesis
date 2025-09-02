#include "midione.h"

t_synth	create_synth(t_mixer *mixer, t_track_type waveform_type)
{
	t_synth	synth;

	synth.wavetable = malloc(TABLE_SIZE * sizeof(float));
	if (!synth.wavetable)
		exit(destroy_mixer_and_info(mixer));
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
		*output_buffer += synth->wavetable[wt_idx] * synth->amplitude
			* (1.0f / (float) mixer->info->num_voices);
		output_buffer++;
		synth->phase += synth->phaseIncrement;
		if (synth->phase >= 1.0)
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
	for (i = 0; i < mixer->info->num_voices; i++)
		render_synth_to_buffer(&(mixer->synths[i]), mixer);
	memcpy(out, mixer->mixbuffer, framesPerBuffer * sizeof(float));
	return (paContinue);
}

void	init_synth(t_info *info, t_mixer *mixer)
{
	create_mixer(info, mixer);

	// Initialize PortAudio
	Pa_Initialize();

	// Create synths for each track
	for (int i = 0; i < info->num_voices; i++)
		mixer->synths[i] = create_synth(mixer, info->tracks[i].type);

	Pa_OpenDefaultStream(&mixer->stream, 0, 1, paFloat32, SAMPLE_RATE,
		FRAMES_PER_BUFFER, paCallback, mixer);
	Pa_StartStream(mixer->stream);
}

// FIXME: delete this synth() when ready!
// This function will be separated by Yonatan
/*
void	synth(t_mixer *mixer)
{
	PaStream	*stream;

	// This part being called when the Sequencer starts
	Pa_Initialize(); //what if fail?
	Pa_OpenDefaultStream(&stream, 0, 1, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, paCallback, mixer);
	Pa_StartStream(stream);

	//PLACEHOLDERS being called from Sequencer when the key is pressed
	set_note(&(mixer->synths[0]), 440, 1);
	Pa_Sleep(1000);
	set_note(&(mixer->synths[1]), 125, 1);
	Pa_Sleep(500);

	// Being called when all tracks finish playing
	Pa_StopStream(stream);
	Pa_CloseStream(stream);
	Pa_Terminate();
}
*/
