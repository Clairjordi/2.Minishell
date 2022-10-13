/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorcery <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 18:47:13 by clorcery          #+#    #+#             */
/*   Updated: 2022/09/28 09:40:33 by mcloarec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_verif_pipe(t_shell *shell, char *s)
{
	int	i;

	i = 0;
	if (ft_check_pipe(s) == -1)
		return (-1);
	while (s[i])
	{
		if (ft_sep(s[i]) == 2)
			ft_skip_quote(&i, &s);
		if (s[i] == '|' && s[i + 1] == '|')
		{
			ft_putendl_fd("Wrong pipes syntax", 2);
			return (-1);
		}
		if (s[i] == '|')
			shell->argc++;
		i++;
	}
	return (0);
}

int	ft_check_pipe(char *s)
{
	int	i;

	i = 0;
	if (s[i] == '|')
	{
		ft_putendl_fd("Wrong pipes syntax", 2);
		return (-1);
	}
	while (s[i])
	{
		if (s[i] == '|' && s[i + 1] == '\0')
		{
			ft_putendl_fd("Wrong pipes syntax", 2);
			return (-1);
		}
		i++;
	}
	return (0);
}