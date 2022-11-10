/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcloarec <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 14:35:26 by mcloarec          #+#    #+#             */
/*   Updated: 2022/11/10 09:52:47 by mcloarec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_exec_builtins(t_shell *shell)
{
	if (ft_strcmp(shell->exec->builtins[0], "pwd") == 0)
		ft_pwd();
	if (ft_strcmp(shell->exec->builtins[0], "env") == 0)
		ft_print_env(shell);
	if (ft_strcmp(shell->exec->builtins[0], "echo") == 0)
		ft_echo(shell->exec->builtins);
	if (ft_strcmp(shell->exec->builtins[0], "cd") == 0)
		ft_cd(shell->exec->builtins);
	/* if (ft_strcmp(shell->exec->builtins[0], "export") == 0) */
	/* 	ft_export(shell->exec->builtins); */
	/* if (ft_strcmp(shell->exec->builtins[0], "exit") == 0) */
	/* 	ft_cd(shell->exec->builtins); */
	/* if (ft_strcmp(shell->exec->builtins[0], "unset") == 0) */
	/* 	ft_unset(shell->exec->builtins); */
}

int	ft_execute_cmd(t_shell *shell, char **envp, int wstatus)
{
	shell->exec->pid = fork();
	if (shell->exec->pid == ERROR)
		perror("ERROR pid");
	if (shell->exec->pid == 0)
	{
		g_g.is_in_loop = 3;
		signal(SIGQUIT, SIG_DFL);
		ft_child_cmd(shell, shell->exec, envp);
	}
	g_g.is_in_loop = 2;
	if (waitpid(shell->exec->pid, &wstatus, 0) == ERROR)
		perror("ERROR waitpid");
	g_g.is_in_loop = 0;
	return (wstatus);
}

void	ft_execute_pipe(t_shell *shell, t_exec *exec, char **envp, t_cmds *lst)
{
	exec->pid = fork();
	if (exec->pid == ERROR)
		perror("ERROR pid");
	if (exec->pid == 0)
	{
		g_g.is_in_loop = 3;
		signal(SIGQUIT, SIG_DFL);
		if (lst->prev != NULL && lst->prev->pipe_fd[1] > 2)
			close(lst->prev->pipe_fd[1]);
		if (lst->pipe_fd[0] > 2)
			close(lst->pipe_fd[0]);
		ft_check_child_execute(shell, envp, lst);
	}
	g_g.is_in_loop = 2;
	if (lst->next != NULL)
	{
		close(lst->pipe_fd[1]);
		lst->pipe_fd[1] = 0;
	}
}

void	ft_minishell(t_shell *shell, char **envp)
{
	if (ft_check_error_redirect(shell) == FALSE)
		return ;
	ft_count_heredoc(shell);
	if (ft_init_heredoc(shell) == ERROR)
		return ;
	ft_check_execute(shell, envp);
}
