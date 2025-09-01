#include "parser.h"
#include "synth.h"

int	free_info(t_info *info)
{
	t_note	*temp;

	if (info->fd)
		fclose(info->fd);
	free(info->line);
	if (info->name)
		free(info->name);
	if (info->tracks){
		for (int i = 0; i < info->num_tracks; i++){
			if (info->tracks[i].sidenote)
				free(info->tracks[i].sidenote);
			while (info->tracks[i].note){
				temp = info->tracks[i].note->next;
				free(info->tracks[i].note);
				info->tracks[i].note = temp;
			}
		}
		free(info->tracks);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_info	info;

	if (argc != 2)
	{
		write(2, "Invalid input\n", 14);
		return (1);
	}
	info.fd = fopen(argv[1], "r");
	if (!info.fd)
	{
		perror("fopen failed");
		return (1);
	}
	parser(&info);
	processor(&info);
	t_mixer	*mixer = synth_init(&info);
	//sequencer(&info);
	if (info.num_tracks > 0 && info.tracks[0].note)
	{
	    printf("Playing first note: freq=%f\n", info.tracks[0].note->f);
	    set_note(mixer->synths[0], info.tracks[0].note->f, 1.0);
	    usleep(1000000);
		set_note(mixer->synths[1], info.tracks[1].note->f, 1.0);
	    usleep(1000000);
	    set_note(mixer->synths[0], info.tracks[0].note->f, 0.0);
		set_note(mixer->synths[1], info.tracks[1].note->f, 0.0);
	}
	synth_destroy(mixer);
	free_info(&info);
	return (0);
}
