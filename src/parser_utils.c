#include "minisynth.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		len1;
	int		len2;
	int		i;
	char	*ptr;

	i = 0;
	len1 = strlen(s1);
	len2 = strlen(s2);
	ptr = malloc((len1 + len2 + 1) * sizeof(char));
	if (ptr == NULL)
		return (NULL);
	while (i < len1)
	{
		ptr[i] = s1[i];
		i++;
	}
	while (i < len1 + len2)
	{
		ptr[i] = s2[i - len1];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

void	parse_waveform_list(t_info *info, char **line, int i)
{
	if (strncmp(*line, "saw", 3) == 0){
		info->tracks[i].type = SAW;
		*line += 3;
	}
	else if (strncmp(*line, "sine", 4) == 0){
		info->tracks[i].type = SINE;
		*line += 4;
	}
	else if (strncmp(*line, "square", 6) == 0){
		info->tracks[i].type = SQUARE;
		*line += 6;
	}
	else if (strncmp(*line, "triangle", 8) == 0){
		info->tracks[i].type = TRIANGLE;
		*line += 8;
	}
	while (**line == ',' || **line == ' ')
		(*line)++;
}

void	switch_to_real_track(t_info *info, int real_track)
{
	if (info->i_now != real_track){
		info->tracks[real_track].sidenote = info->tracks[info->i_now].sidenote;
		info->tracks[info->i_now].sidenote = NULL;
		info->i_now = real_track;
	}
}
