/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcloarec <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 11:23:42 by mcloarec          #+#    #+#             */
/*   Updated: 2022/10/29 14:41:28 by clorcery         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_check_error_redirect(t_shell *shell)
{
	int		i;
	t_cmds	*lst;

	i = 0;
	lst = shell->arg;
	while (lst)
	{
		i = 0;
		while (lst->value_split[i] != NULL)
		{
			if (ft_valid_redirect(lst->value_split[i]) != FALSE
				&& (lst->value_split[i + 1] == NULL
					|| ft_valid_redirect(lst->value_split[i + 1]) != FALSE))
			{
				ft_free(shell, "Syntax error near unexpected token");
				g_g.status = 2;
				return (FALSE);
			}
			i++;
		}
		lst = lst->next;
	}
	return (TRUE);
}

int	ft_check_infile(t_exec *exec, char **tab, int i)
{
	if (i != 0 && ft_valid_redirect(tab[i - 1]) == 2)
	{
		if (exec->infile > 2)
			close(exec->infile);
		exec->infile = open(tab[i], O_RDONLY, 0644);
		if (exec->infile == -1)
		{
			perror("File error");
			g_g.status = 2;
			return (FALSE);
		}
	}
	return (TRUE);
}

static int	ft_check_outfile_append(t_shell *shell, char **tab, int i)
{
	if (shell->exec->outfile > 2)
		close(shell->exec->outfile);
	if (ft_check_q(tab[i][0]) == 1 && ft_is_not_redirection(tab[i]) == TRUE)
		tab[i] = ft_delete_quotes_redirect(shell, tab[i]);
	shell->exec->outfile = open(tab[i], (O_RDWR | O_APPEND | O_CREAT), 0644);
	if (shell->exec->outfile == -1)
	{
		perror("File error");
		g_g.status = 2;
		return (FALSE);
	}
	return (TRUE);
}

int	ft_check_outfile(t_shell *shell, char **tab, int i)
{
	if (i != 0 && ft_valid_redirect(tab[i - 1]) == 3)
		ft_check_outfile_append(shell, tab, i);
	else if (i != 0 && ft_valid_redirect(tab[i - 1]) == 4)
	{
		if (shell->exec->outfile > 2)
			close(shell->exec->outfile);
		if (ft_check_q(tab[i][0]) == 1 && ft_is_not_redirection(tab[i]) == TRUE)
			tab[i] = ft_delete_quotes_redirect(shell, tab[i]);
		shell->exec->outfile = open(tab[i], (O_RDWR | O_TRUNC | O_CREAT), 0644);
		if (shell->exec->outfile == -1)
		{
			perror("File error");
			g_g.status = 2;
			return (FALSE);
		}
	}
	return (TRUE);
}

int	ft_check_first(t_shell *shell, char **envp, char *s)
{
	if (ft_get_path(shell, s, envp) == NULL
		&& ft_valid_redirect(s) == FALSE)
	{
		ft_putendl_fd("First arg must be cmd or redirection", 2);
		g_g.status = 127;
		return (FALSE);
	}
	return (TRUE);
}
