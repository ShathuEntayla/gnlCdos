/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhidalgo <fhidalgo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 19:19:26 by fhidalgo          #+#    #+#             */
/*   Updated: 2026/11/09 17:11:52 by fhidalgo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*get_stash(char *stash, int fd)
{
	char	*buf;
	ssize_t	reader;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (free(stash), NULL);
	reader = 1;
	while (reader > 0 && char_index_finder(stash, '\n', 0) == -1)
	{
		reader = read(fd, buf, BUFFER_SIZE);
		if (reader < 0)
		{
			free(buf);
			free(stash);
			return (NULL);
		}
		buf[reader] = '\0';
		stash = ft_strjoin_and_freeold((const char *)stash, (const char *)buf);
		if (!stash)
			return (free(buf), NULL);
	}
	return (free(buf), stash);
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
	char_pos = char_index_finder(stash, sep, 0);
	len = 0;
	if (char_pos >= 0)
		len = char_pos + 1;
	else if (char_pos == -1)
		len = ft_strlen(stash);
	line = ft_substr((const char *)stash, 0, len);
	return (line);
}

static char	*get_rest(char *stash)
{
	char	*rest;
	int		char_pos;
	size_t	len_rest;

	char_pos = char_index_finder(stash, '\n', 0);
	if (char_pos == -1)
		return (free(stash), NULL);
	len_rest = ft_strlen(stash) - char_pos - 1;
	rest = ft_substr(stash, char_pos + 1, len_rest);
	free(stash);
	if (!rest || rest[0] == '\0')
		return (free(rest), NULL);
	return (rest);
}

char	*get_next_line(int fd)
{
	char		*red_line;
	static char	*stash;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = get_stash(stash, fd);
	red_line = get_line(stash);
	if (!red_line)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	stash = get_rest(stash);
	return (red_line);
}
