/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntadevos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 11:26:48 by ntadevos          #+#    #+#             */
/*   Updated: 2021/02/21 12:23:24 by ntadevos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
#define BUFFER_SIZE 1

int		katox(char *remind)
{
	int i;

	i = 0;
	if (!remind)
		return (0);
	while (remind[i])
	{
		if (remind[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

int		ft_strleng(const char *str)
{
	int	i;

	i = 0;
	while (str != NULL && str[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*dest;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	dest = malloc(1 + (ft_strleng(s1) + ft_strleng(s2)) * sizeof(char));
	i = 0;
	j = 0;
	while ((s1 != NULL && s1[i]) || (s2 != NULL && s2[j]))
	{
		if (s1 != NULL && s1[i])
		{
			dest[i] = s1[i];
			i++;
		}
		else
		{
			dest[i + j] = s2[j];
			j++;
		}
	}
	dest[i + j] = '\0';
	free((char *)s1);
	return (dest);
}
char	*fline(char *remind)
{
	int		i;
	char	*str;

	i = 0;
	if (!remind)
		return (0);
	while (remind[i] && remind[i] != '\n')
		i++;
	if (!(str = malloc(sizeof(char) * (i + 1))))
		return (0);
	i = 0;
	while (remind[i] && remind[i] != '\n')
	{
		str[i] = remind[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*fremind(char *remind)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	if (!remind)
		return (0);
	while (remind[i] && remind[i] != '\n')
		i++;
	if (!remind[i])
	{
		free(remind);
		return (0);
	}
	if (!(str = malloc(sizeof(char) * ((ft_strleng(remind) - i) + 1))))
		return (0);
	i++;
	while (remind[i])
		str[j++] = remind[i++];
	str[j] = '\0';
	free(remind);
	return (str);
}

int		get_next_line(int fd, char **line)
{
	static char		*remind;
	int				rd;
	char			*buf;

	rd = 1;
	if (fd < 0 || !line || BUFFER_SIZE <= 0)
		return (-1);
	if (!(buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1))))
		return (-1);
	while (!katox(remind) && rd != 0)
	{
		if ((rd = read(fd, buf, BUFFER_SIZE)) == -1)
		{
			free(buf);
			return (-1);
		}
		buf[rd] = '\0';
		remind = ft_strjoin(remind, buf);
	}
	free(buf);
	*line = fline(remind);
	remind = fremind(remind);
	if (rd == 0)
		return (0);
	return (1);
}
