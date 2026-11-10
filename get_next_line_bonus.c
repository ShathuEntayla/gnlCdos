/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhidalgo <fhidalgo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 19:19:26 by fhidalgo          #+#    #+#             */
/*   Updated: 2026/11/09 17:07:33 by fhidalgo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

#include <stdio.h>

static char	*get_stash(char **stash, int fd)
{
	char	*buf;
	ssize_t	reader;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (free(stash[fd]), NULL);
	reader = 1;
	while (reader > 0
		&& (!stash[fd] || char_index_finder(&stash[fd], '\n', 0) == -1))
	{
		reader = read(fd, buf, BUFFER_SIZE);
		if (reader < 0)
		{
			free(buf);
			free(stash);
			return (NULL);
		}
		buf[reader] = '\0';
		stash[fd] = ft_strjoin_and_freeold(stash[fd], (const char *)buf);
		if (!stash[fd])
			return (free(buf), NULL);
	}
	return (free(buf), stash[fd]);
}

static char	*get_line(char *stash)
{
	char	*line;
	int		sep;
	int		len;
	int		char_pos;

	if (!stash || stash[0] == '\0')
		return (NULL);
	sep = '\n';
	char_pos = char_index_finder(&stash, sep, 0);
	len = 0;
	if (char_pos >= 0)
		len = char_pos + 1;
	else if (char_pos == -1)
		len = ft_strlen((const char **)&stash);
	line = ft_substr(stash, 0, len);
	return (line);
}

static char	*get_rest(char *stash)
{
	char	*rest;
	int		char_pos;
	size_t	len_rest;

	char_pos = char_index_finder(&stash, '\n', 0);
	if (char_pos == -1)
		return (free(stash), NULL);
	len_rest = ft_strlen((const char **)&stash) - char_pos - 1;
	rest = ft_substr(stash, char_pos + 1, len_rest);
	free(stash);
	if (!rest || rest[0] == '\0')
		return (free(rest), NULL);
	return (rest);
}

char	*get_next_line(int fd)
{
	char		*red_line;
	static char	*stash[FD_SETSIZE];

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= FD_SETSIZE)
		return (NULL);
	stash[fd] = get_stash(stash, fd);
	red_line = get_line(stash[fd]);
	if (!red_line)
	{
		free(stash[fd]);
		stash[fd] = NULL;
		return (NULL);
	}
	stash[fd] = get_rest(stash[fd]);
	return (red_line);
}

// int	main(void)
// {
//     #include <fcntl.h>
// 	#include <stdio.h>

// 	int		fd1;
// 	int		fd2;
// 	char	*line;
// 	char	*line2;

// 	fd1 = open("test.txt", O_RDONLY);
// 	fd2 = open("test2.txt", O_RDONLY);
// 	printf("%s\n", "fds definidos");
// 	line = get_next_line(fd1);
// 	printf("%s\n", "line definido");
// 	line2 = get_next_line(fd2);
// 	printf("%s\n", "line2 definido");
// 	while (line && line2) // Mientras exista la línea
// 	{	
// 		free(line);						// Y luego la liberas
// 		line = get_next_line(fd1); 		//Definimos siguiente linea	
// 		free(line2);						// Y luego la liberas
// 		line2 = get_next_line(fd2); 	
// 	}
// 	close(fd1);                         // Luego cierra el archivo
// 	close(fd2);                         // Luego cierra el archivo
// 	return (0);
// }
