/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcloarec <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 10:56:35 by mcloarec          #+#    #+#             */
/*   Updated: 2022/11/16 11:44:15 by clorcery         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
//A DEPLACER
int	ft_verif_file(t_shell *shell)
{
	int	i;
	int	fd;

	i = 1;
	while (shell->exec->cmd[i] != NULL)
	{
		fd = open(shell->exec->cmd[i], O_RDONLY, 0644);
		if (fd > 2)
		{
			close(fd);
			return (FALSE);
		}
		i++;
	}
	return (TRUE);
}

void	ft_add_file_to_tab_cmd(t_shell *shell, t_cmds *lst)
{
	int	i;

	i = 0;
	if (ft_verif_file(shell) == FALSE)
		return ;
	while (lst->value_split[i] != NULL)
	{
		if (i > 0 && ft_strcmp(lst->value_split[i - 1], "<") == 0)
		{
			shell->exec->cmd = ft_realloc_tab_char(shell->exec->cmd,
					lst->value_split[i]);
			if (shell->exec->infile != -1)
				close(shell->exec->infile);
			shell->exec->infile = 0;
			g_minishell.status = 0;
			return ;
		}
		i++;
	}
}
//
static int	ft_sort_check_built(t_shell *shell, t_cmds *lst, int *i)
{
	if ((*i == 0
			|| (*i > 0 && ft_valid_redirect(lst->value_split[*i - 1]) == FALSE))
		&& shell->exec->cmd == NULL)
	{
		if (ft_check_is_builtins(shell, lst->value_split, i) == TRUE)
			return (TRUE);
	}
	return (FALSE);
}

static void	ft_sort_cmd_bis(t_shell *shell, t_cmds *lst)
{
	int	i;

	i = 0;
	while (lst->value_split[i])
	{
		if (ft_sort_check_built(shell, lst, &i) == TRUE)
			break ;
		if (shell->exec->is_dir == 0)
			shell->exec->is_dir = ft_is_directory(lst->value_split[i]);
		if (ft_check_infile(shell->exec, lst->value_split, i) == FALSE)
			break ;
		if (lst->hdoc == TRUE && lst->value_split[i + 1] == NULL)
		{
			if (shell->exec->infile > 2)
				close(shell->exec->infile);
			shell->exec->infile = open(".heredoc", O_RDONLY, 0644);
		}
		if (ft_check_outfile(shell, lst->value_split, i) == FALSE)
			break ;
		if (shell->exec->builtins == NULL && shell->exec->is_dir == 0
			&& lst->cmd_found == TRUE)
			lst->cmd_found = ft_check_cmd(shell, lst->value_split, i);
		if (lst->value_split[i] != NULL)
			i++;
	}
}

void	ft_sort_cmd(t_shell *shell, t_cmds *lst)
{
	int	wstatus;

	wstatus = 0;
	ft_sort_cmd_bis(shell, lst);
	if (shell->exec->cmd != NULL && shell->exec->infile != 0)
		ft_add_file_to_tab_cmd(shell, lst);
	if (shell->exec->builtins != NULL
		&& ft_check_builtins_without_fork(shell) == TRUE)
	{
		ft_free_close(shell);
		return ;
	}
	if ((shell->exec->cmd != NULL || shell->exec->builtins != NULL))
	{
		wstatus = ft_execute_cmd(shell, wstatus);
		ft_status_child(wstatus);
	}
	ft_free_close(shell);
}

static void	ft_sort_cmd_pipe_bis(t_shell *shell, t_cmds *lst)
{
	if (shell->exec->cmd != NULL && shell->exec->infile != 0)
		ft_add_file_to_tab_cmd(shell, lst);
	if (shell->exec->builtins != NULL || shell->exec->cmd != NULL)
		ft_execute_pipe(shell, shell->exec, lst);
	ft_free_close_pipe(shell, lst);
}

void	ft_sort_cmd_pipe(t_shell *shell, t_cmds *lst)
{
	int	i;

	i = 0;
	while (lst->value_split[i])
	{
		if (ft_sort_check_built(shell, lst, &i) == TRUE)
			break ;
		if (shell->exec->is_dir == 0 && shell->exec->cmd == NULL)
			shell->exec->is_dir = ft_is_directory(lst->value_split[i]);
		if (ft_check_infile(shell->exec, lst->value_split, i) == FALSE)
			break ;
		if (lst->hdoc == TRUE && lst->value_split[i + 1] == NULL)
		{
			if (shell->exec->infile > 2)
				close(shell->exec->infile);
			shell->exec->infile = open(".heredoc", O_RDONLY, 0644);
		}
		if (ft_check_outfile(shell, lst->value_split, i) == FALSE)
			break ;
		if (shell->exec->is_dir == 0 && lst->cmd_found == TRUE)
			lst->cmd_found = ft_check_cmd(shell, lst->value_split, i);
		if (lst->value_split[i] != NULL)
			i++;
	}
	ft_sort_cmd_pipe_bis(shell, lst);
}
