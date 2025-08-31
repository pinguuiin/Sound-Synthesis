#include "parser.h"

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

	if (argc != 2){
		write(2, "Invalid input\n", 14);
		return (1);
	}
	info.fd = fopen(argv[1], "r");
	if (!info.fd){
		perror("fopen failed");
		return (1);
	}
	parser(&info);
	processor(&info);
	sequencer(&info);
	synth(); // WARN: check!
	free_info(&info);
	return (0);
}
