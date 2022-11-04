/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_utils_bis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorcery <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 07:57:34 by clorcery          #+#    #+#             */
/*   Updated: 2022/11/04 08:00:01 by clorcery         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_is_a_path(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '/')
			return (0);
		i++;
	}
	return (1);
}

int	ft_is_directory(char *str)
{
	struct stat	*s;

	if (ft_is_a_path(str) == 1)
		return (0);
	s = malloc(sizeof(struct stat));
	if (stat(str, s) == -1)
	{
		perror(str);
		if (errno == EACCES)
			g_g.status = 126;
		else
			g_g.status = 127;
		free(s);
		return (1);
	}
	if (S_ISDIR(s->st_mode))
	{
		ft_putstr_fd(str, 2);
		ft_putendl_fd(": Is a directory", 2);
		g_g.status = 126;
		free(s);
		return (1);
	}
	free(s);
	return (0);
}