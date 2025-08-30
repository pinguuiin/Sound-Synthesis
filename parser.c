#include "parser.h"

void	handle_name(t_info *info, char *line)
{
	int		len;
	char	*s;

	len = strlen(line) - 1;
	if (!info->name){
		info->name = malloc(len * sizeof(char));
		if (!info->name)
			exit(free_info(info));
		memmove(info->name, line + 2, len);
	}
	else {
		s = ft_strjoin(info->name, line + 2);
		if (!s)
			exit(free_info(info));
		free(info->name);
		info->name = s;
	}
}

void	handle_tempo(t_info *info, char *line)
{
	info->tempo = 0;
	if (strncmp(line, "tempo ", 6) == 0){
		line += 6;
		while (*line >= '0' && *line <= '9'){
			info->tempo = info->tempo * 10 + (*line) - '0';
			line++;
		}
	}
	info->file_pos =TRACKS;
}

void	parse_line(t_info  *info, char *line)
{
	if (!line || line[0] == '\0' || line[0] == ' ')
		return ;
	if (info->file_pos == NAME)
		info->file_pos = (line[0] == '#') ? NAME : TEMPO;
	else if (info->file_pos == SIDENOTE)
		info->file_pos = (line[0] == '#') ? SIDENOTE : NOTES;
	switch (info->file_pos) {
		case NAME:
			handle_name(info, line);
			break ;
		case TEMPO:
			handle_tempo(info, line);
			break ;
		case TRACKS:
			handle_tracks(info, line);
			break ;
		case SIDENOTE:
			handle_sidenotes(info, line);
			break ;
		case NOTES:
			handle_notes(info, line);
			break ;
	}
}

void	parser(t_info *info)
{
	size_t	len;

	len = 0;
	info->line = NULL;
	info->name = NULL;
	info->tracks = NULL;
	info->file_pos = NAME;
	while (getline(&info->line, &len, info->fd) != -1){
		parse_line(info, info->line);
		free(info->line);
		info->line = NULL;
	}
	if (!feof(info->fd)){
		perror("getline failed");
		exit(free_info(info));
	}
	free(info->line);
	info->line = NULL;
	fclose(info->fd);
	info->fd = -1;
}
