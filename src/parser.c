#include "minisynth.h"

static void	handle_name(t_info *info, char *line)
{
	int		len;
	char	*s;

	while (*line == '#' || *line == ' ')
		line++;
	len = strlen(line) + 1;
	if (!info->name){
		info->name = malloc(len * sizeof(char));
		if (!info->name)
			exit(free_info(info));
		memmove(info->name, line, len);
	}
	else {
		s = ft_strjoin(info->name, line);
		if (!s)
			exit(free_info(info));
		free(info->name);
		info->name = s;
	}
}

static void	handle_tempo(t_info *info, char *line)
{
	info->tempo = 0;
	if (strncmp(line, "tempo ", 6) == 0){
		line += 6;
		while (*line >= '0' && *line <= '9'){
			info->tempo = info->tempo * 10 + (*line) - '0';
			line++;
		}
		info->beat_to_usec = ((double)60 / info->tempo) * 1000000;
	}
	info->file_pos =TRACKS;
}

static void	handle_tracks(t_info *info, char *line)
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
		for (i = 0; i < info->num_tracks && *line;)
		{
			info->tracks[i].sidenote = NULL;
			info->tracks[i].note = NULL;
			info->tracks[i].time_last_note_began = 0;
			info->tracks[i].id = i + 1;
			parse_waveform_list(info, &line, i);
			i++;
		}
	}
	info->file_pos = NOTES;
}

static void	handle_sidenote(t_info *info, char *line)
{
	int		i;
	int		len;
	char	*s;

	i = info->i_now;
	while (*line == '#' || *line == ' ')
		line++;
	len = strlen(line) + 1;
	if (!info->tracks[i].sidenote){
		info->tracks[i].sidenote = malloc(len * sizeof(char));
		if (!info->tracks[i].sidenote)
			exit(free_info(info));
		memmove(info->tracks[i].sidenote, line, len);
	}
	else {
		s = ft_strjoin(info->tracks[i].sidenote, line);
		if (!s)
			exit(free_info(info));
		free(info->tracks[i].sidenote);
		info->tracks[i].sidenote = s;
	}
}

static void	handle_one_note(t_info *info, char *line)
{
	t_note	*last;
	t_note	*new_note;

	new_note = malloc(sizeof(t_note));
	if (!new_note)
		exit(free_info(info));
	new_note->next = NULL;
	new_note->pitch = *line;
	line++;
	if (*line == '#' || *line == 'b')
		new_note->alteration = *line++;
	else
		new_note->alteration = '-';
	last = info->tracks[info->i_now].note;
	while (last && last->next)
		last = last->next;
	if (isdigit(*line))
		new_note->octave = *line++ - '0';
	else if (info->tracks[info->i_now].begin == 1)  // first note of the line
		new_note->octave = 4;
	else
		new_note->octave = last->octave;
	if (*line == '/')
		new_note->duration = strtof(line + 1, NULL) * info->beat_to_usec;
	else if (info->tracks[info->i_now].begin == 1)
		new_note->duration = info->beat_to_usec;
	else
		new_note->duration = last->duration;
	if (last)
		last->next = new_note;
	else
		info->tracks[info->i_now].note = new_note;
}

static void	handle_notes(t_info *info, char *line)
{
	int	real_track;

	real_track = atoi(line) - 1;
	switch_to_real_track(info, real_track);
	info->tracks[real_track].begin = 1;
	while (*line && !isalpha(*line))
		line++;
	while (*line){
		if (isalpha(*line)){
			handle_one_note(info, line);
			info->tracks[real_track].begin = 0;
		}
		info->tracks[real_track].temp = info->tracks[real_track].note; // set the temp pointer to the head of the linked list
		while (*line && !isspace(*line) && *line != '|')
			line++;
		while (isspace(*line) || *line == '|')
			line++;
	}
	if (info->i_now < info->num_tracks - 1)
		info->i_now++;
}

static void	handle_notes_info(t_info *info, char *line)
{
	if (line[0] == '#')
		handle_sidenote(info, line);
	else if (line[0] >= '1' && line[0] <= '9')
		handle_notes(info, line);
	return ;
}

static void	parse_line(t_info  *info, char *line)
{
	int	i;

	i = 0;
	if (!line || line[0] == '\0' || line[0] == ' ' || line[0] == '\n')
		return ;
	while (line[i]){
		if (line[i] == '\n')
			line[i] = '\0';
		i++;
	}
	if (info->file_pos == NAME)
		info->file_pos = (line[0] == '#') ? NAME : TEMPO;
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
		case NOTES:
			handle_notes_info(info, line);
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
	info->i_now = 0;
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
	info->fd = NULL;
}
