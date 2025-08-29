#include "parser.h"

int	main(int argc, char **argv){
	t_info	info;
	FILE	*fd;
	char	*line;
	size_t	len;

	len = 0;
	line = NULL;
	if (argc != 2){
		write(2, "Invalid input\n", 14);
		return (1);
	}
	fd = fopen(argv[1], "r");
	if (!fd){
		perror("fopen failed");
		return (1);
	}
	while (getline(&line, &len, fd) != -1){
		parser(&info, line);
		free(line);
		line = NULL;
	}
	if (!feof(fd)){
		perror("getline failed");
		free(line);
		free_info(info);
		fclose(fd);
		return (1);
	}
	free(line);
	fclose(fd);
	sound_generator(info);
	return (0);
}
