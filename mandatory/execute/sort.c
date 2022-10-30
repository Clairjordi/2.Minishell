/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcloarec <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 10:56:35 by mcloarec          #+#    #+#             */
/*   Updated: 2022/10/29 22:13:03 by clorcery         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_sort_cmd(t_shell *shell, t_exec *exec, t_cmds *lst, char **envp)
{
	int	i;
	int	wstatus;

	i = 0;
	wstatus = 0;
	while (lst->value_split[i])
	{	
		if (lst->hdoc == FALSE)
		{
			if (ft_check_first(shell, envp, lst->value_split[0]) == 0 && i == 0)
				break ;
			if (ft_check_infile(exec, lst->value_split, i) == FALSE)
				break ;
		}
		else if (i == 0 && lst->hdoc == TRUE)
		{
			ft_init_heredoc(shell, lst);
			exec->infile = open(".heredoc", O_RDONLY, 0644);
		}
		if (ft_check_outfile(shell, lst->value_split, i) == FALSE)
			break ;
		if (ft_check_cmd(shell, envp, lst->value_split, i) == FALSE)
			break ;
		if (lst->value_split[i] != NULL)
			i++;
	}
	if (shell->exec->cmd != NULL)
		wstatus = ft_execute_cmd(shell, envp, lst, wstatus);
	ft_status_child(wstatus);
	ft_free_close(shell);
}

void	ft_sort_cmd_pipe(t_shell *shell, t_cmds *lst, char **envp)
{
	int	i;

	i = 0;
	while (lst->value_split[i])
	{
		if (lst->hdoc == FALSE)
		{
			if (ft_check_first(shell, envp, lst->value_split[0]) == FALSE
				&& i == 0)
				break ;
			if (ft_check_infile(shell->exec, lst->value_split, i) == FALSE)
				break ;
		}
		else if (i == 0 && lst->hdoc == TRUE)
		{
			ft_init_heredoc(shell, lst);
			shell->exec->infile = open(".heredoc", O_RDONLY, 0644);
		}
		if (ft_check_outfile(shell, lst->value_split, i) == FALSE)
			break ;
		if (ft_check_cmd(shell, envp, lst->value_split, i) == FALSE)
			break ;
		if (lst->value_split[i] != NULL)
			i++;
	}
	if (shell->exec->cmd != NULL)
		ft_execute_pipe(shell, shell->exec, envp, lst);
	ft_free_close_pipe(shell, lst);
}

int ft_check_cmd_pipe(t_shell *shell, t_cmds *lst, char **envp)
{
	int i;

	i = 0;
	while (lst->value_split[i])
	{
		if (ft_get_path(shell, lst->value_split[i], envp) != NULL)
			return (TRUE);
		i++;
	}
	return (FALSE);
}

void	ft_check_execute(t_shell *shell, char **envp)
{
	t_cmds	*lst;

	lst = shell->arg;
	shell->exec->pid = 0;
	if (shell->pipe == 1)
	{
		while (lst)
		{	
			ft_sort_cmd(shell, shell->exec, lst, envp);
			lst = lst->next;
		}
	}
	else
	{
		while (lst)
		{
			if (ft_check_cmd_pipe(shell, lst, envp) == TRUE)
			{
				shell->pipe = ft_check_shell_pipe(shell, lst);
				if (shell->pipe == ERROR)
					break ;
			}
			ft_sort_cmd_pipe(shell, lst, envp);
			ft_add_pid(shell);
			lst = lst->next;
		}
		ft_waitpid_pipe(shell);
	}
}
