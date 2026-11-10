/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhidalgo <fhidalgo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 12:00:39 by fhidalgo          #+#    #+#             */
/*   Updated: 2026/11/09 16:27:53 by fhidalgo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

/* SYSTEM LIBRARIES */

# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>

/* UTILS */

size_t	ft_strlen(const char *s);
int		char_index_finder(char *s, int c, int i);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strdup(const char *src);
char	*ft_strjoin_and_freeold(char const *s1, char const *s2);

/* PROTOTYPES */

char	*get_next_line(int fd);

#endif