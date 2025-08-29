#include "parser.h"

void	parser(t_info *info)
{
	size_t	len;
	char	*line;

	len = 0;
	line = NULL;
	while (getline(&line, &len, info->fd) != -1){
		parse_line(&info, line);
		free(line);
		line = NULL;
	}
	if (!feof(info->fd)){
		perror("getline failed");
		free(line);
		free_info(info);
		//fclose(fd);
		exit(EXIT_FAILURE);
	}
	free(line);
	fclose(info->fd);
}

int	main(int argc, char **argv)
{
	t_info	info;
	char	*line;
	size_t	len;

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
	sound_generator(info);
	return (0);
}
