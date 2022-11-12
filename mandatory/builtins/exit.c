/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorcery <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 15:15:09 by clorcery          #+#    #+#             */
/*   Updated: 2022/11/12 18:15:40 by clorcery         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_check_long_arg_exit(t_shell *shell, char *arg_exit)
{
	int	size;

	(void) shell;
	size = 0;
	while (arg_exit[size])
		size++;
	if (arg_exit[0] == '-' || arg_exit[0] == '+')
	{
		if (size > 20)
			return (FALSE);
	}
	else if (size > 19)
		return (FALSE);
	if (arg_exit[0] == '-' && arg_exit[size - 1] > '8')
		return (FALSE);
	if (arg_exit[0] != '-' && arg_exit[size - 1] > '7')
		return (FALSE);
	return (TRUE);
}

int	ft_check_arg_exit(t_shell *shell, char *arg_exit)
{
	int	i;

	(void) shell;
	i = 0;
	if (arg_exit[i] == '+' || arg_exit[i] == '-')
		i++;
	while (arg_exit[i])
	{
		if (ft_isdigit(arg_exit[i]) == 0
			|| ft_check_long_arg_exit(shell, arg_exit) == FALSE)
		{
			ft_putendl_fd("numeric argument required", 2);
			g_g.status = 2;
			return (FALSE);
		}
		i++;
	}
	return (TRUE);
}

void	ft_exit(t_shell *shell, char **tab_exit)
{
	int			size;
	long long	status;
	int			arg;

	size = ft_size_tab(tab_exit);
	ft_putendl_fd("exit", 1);
	arg = ft_check_arg_exit(shell, tab_exit[1]);
	if (size > 2 && arg == TRUE)
	{
		ft_putendl_fd("Too many arguments", 2);
		g_g.status = 1;
		return ;
	}
	if (arg == TRUE)
	{
		status = ft_atoll(tab_exit[1]);
		status = status % 256;
		g_g.status = status;
	}
	ft_free_envcpy(shell);
	ft_free(shell, NULL);
	exit (g_g.status);
}

void	ft_exit_fork(t_shell *shell, char **tab_exit)
{
	int			size;
	long long	status;

	size = 0;
	size = ft_size_tab(tab_exit);
	if (tab_exit[1] == NULL)
		return ;
	status = ft_check_arg_exit(shell, tab_exit[1]);
	if (status == FALSE)
		return ;
	if (size > 2 && status == TRUE)
	{
		ft_putendl_fd("Too many arguments", 2);
		g_g.status = 1;
		return ;
	}
	g_g.status = ft_atoll(tab_exit[1]);
}
