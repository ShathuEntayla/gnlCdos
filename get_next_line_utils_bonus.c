/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhidalgo <fhidalgo@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 19:19:49 by fhidalgo          #+#    #+#             */
/*   Updated: 2026/11/09 17:03:10 by fhidalgo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	ft_strlen(const char **s)
{
	size_t	i;

	i = 0;
	while ((*s)[i])
		i++;
	return (i);
}

int	char_index_finder(char **s, int c, int i)
{
	if (!s || i < 0 || ft_strlen((const char **)s) < (size_t)i)
		return (-1);
	while ((*s)[i] && (*s)[i] != (char)c)
		i++;
	if ((*s)[i] == (char)c)
		return (i);
	return (-1);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	s_len;
	size_t	i;

	if (!s)
		return (NULL);
	s_len = ft_strlen((const char **)&s);
	if (len > s_len - start)
		len = s_len - start;
	if (start >= s_len)
		return (ft_strdup(""));
	sub = (char *)malloc((len + 1));
	if (!sub)
		return (NULL);
	i = 0;
	while ((i < len) && s[start + i])
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

char	*ft_strdup(const char *src)
{
	int		i;
	char	*copy;

	if (!src)
		return (NULL);
	copy = (char *)malloc((ft_strlen((const char **)&src) + 1) * sizeof(char));
	if (copy == NULL)
		return (NULL);
	i = 0;
	while (src[i])
	{
		copy[i] = src[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

char	*ft_strjoin_and_freeold(char const *s1, char const *s2)
{
	char	*s3;
	size_t	i;
	size_t	j;
	size_t	k;

	if (!s1)
		s3 = malloc(ft_strlen(&s2) + 1);
	else
		s3 = malloc(ft_strlen(&s1) + ft_strlen(&s2) + 1);
	if (!s3)
		return (free((void *)s1), NULL);
	i = 0;
	j = 0;
	k = 0;
	if (s1)
	{
		while (s1[i])
			s3[k++] = s1[i++];
	}
	while (s2[j])
		s3[k++] = s2[j++];
	s3[k] = '\0';
	free((void *)s1);
	return (s3);
}
