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

void	handle_tracks(t_info *info, char *line)
{
	int	i;

	i = 0;
	if (strncmp(line, "tracks ", 7) == 0){
		line += 7;
		info->num_tracks = 1;
		while (line[i]){
			if (line[i++] == ',')
				info->num_tracks++;
		}
		info->tracks = malloc(info->num_tracks * sizeof(t_track));
		if (!info->tracks)
			exit(free_info(info));
		i = 0;
		for (i = 0; i < info->num_tracks && *line;){
			if (strncmp(line, "sin", 3) == 0){
				info->tracks[i].id = i + 1;
				info->tracks[i++].type = SINE;
				line += 4;
			}
			else if (strncmp(line, "saw", 3) == 0){
				info->tracks[i].id = i + 1;
				info->tracks[i++].type = SAW;
				line += 3;
			}
			else if (strncmp(line, "tri", 3) == 0){
				info->tracks[i].id = i + 1;
				info->tracks[i++].type = TRIANGLE;
				line += 8;
			}
			else if (strncmp(line, "squ", 3) == 0){
				info->tracks[i].id = i + 1;
				info->tracks[i++].type = SQUARE;
				line += 6;
			}
			if (*line == ',' || *line == ' ')
				line++;
		}
	}
	info->file_pos = SIDENOTE;
}

// void	handle_sidenote(t_info *info, char *line)
// {
// 	int		len;
// 	char	*s;

// 	len = strlen(line) - 1;
// 	if (!info->sidenote){
// 		info->sidenote = malloc(len * sizeof(char));
// 		if (!info->sidenote)
// 			exit(free_info(info));
// 		memmove(info->sidenote, line + 2, len);
// 	}
// 	else {
// 		s = ft_strjoin(info->sidenote, line + 2);
// 		if (!s)
// 			exit(free_info(info));
// 		free(info->sidenote);
// 		info->sidenote = s;
// 	}
// }

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
