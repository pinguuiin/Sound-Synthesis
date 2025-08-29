#include "parser.h"

int	main(int argc, char **argv)
{
	t_info	info;
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
