/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcloarec <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 14:35:26 by mcloarec          #+#    #+#             */
/*   Updated: 2022/10/26 19:05:07 by mcloarec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <unistd.h>

int	ft_valid_redirect(char *s)
{
	if (ft_strcmp(s, "<<") == 0)
		return (1);
	if (ft_strcmp(s, "<") == 0)
		return (2);
	if (ft_strcmp(s, ">>") == 0)
		return (3);
	if (ft_strcmp(s, ">") == 0)
		return (4);
	return (FALSE);
}

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
		exec->fd_in = ft_strdup(tab[i]);
	}
	return (TRUE);
}

int	ft_check_outfile(t_exec *exec, char **tab, int i)
{
	if (i != 0 && ft_valid_redirect(tab[i - 1]) == 3)
	{
		if (exec->outfile > 2)
			close(exec->outfile);
		exec->outfile = open(tab[i], (O_RDWR | O_APPEND | O_CREAT), 0644);
		if (exec->outfile == -1)
		{
			perror("File error ");
			g_g.status = 2;
			return (FALSE);
		}
		exec->fd_out = ft_strdup(tab[i]);
	}
	else if (i != 0 && ft_valid_redirect(tab[i - 1]) == 4)
	{
		if (exec->outfile > 2)
			close(exec->outfile);
		exec->outfile = open(tab[i], (O_RDWR | O_TRUNC | O_CREAT), 0644);
		if (exec->outfile == -1)
		{
			perror("File error ");
			g_g.status = 2;
			return (FALSE);
		}
		exec->fd_out = ft_strdup(tab[i]);
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

int	ft_add_opt_arg(t_shell *shell, char *s, char **envp)
{
	if (ft_get_path(shell, s, envp) == NULL)
	{
		shell->exec->cmd = ft_realloc_tab_char(shell->exec->cmd, s);
		if (shell->exec->cmd == NULL)
			ft_free_malloc(shell);
		return (TRUE);
	}
	else
		return (FALSE);
}

int ft_add_cmd(t_shell *shell, char *s, char **envp)
{
	if (shell->exec->cmd != NULL
		&& ft_get_path(shell, s, envp) != NULL)
	{
		perror("ERROR cmd");
		g_g.status = 1;
		return (FALSE);
	}
	else if (shell->exec->cmd == NULL)
	{
		shell->exec->cmd = ft_calloc(sizeof(char *), 2);
		if (!shell->exec->cmd)
			ft_free_malloc(shell);
		shell->exec->cmd[0] = ft_strdup(shell->exec->cmd_path);
		if (shell->exec->cmd[0] == NULL)
			ft_free_malloc(shell);
		shell->exec->cmd[1] = NULL;
	}
	return (TRUE);
}

int	ft_check_cmd(t_shell *shell, char *name_cmd, char **envp, char **tab, int i)
{
	if (shell->exec->cmd_path == NULL && ft_valid_redirect(name_cmd) == FALSE)
		ft_get_path(shell, name_cmd, envp);
	if (shell->exec->cmd_path != NULL)
		ft_add_cmd(shell, name_cmd, envp);
	if (i > 0 && shell->exec->cmd != NULL && ft_valid_redirect(name_cmd) == FALSE 
			&& ft_valid_redirect(tab[i - 1]) == FALSE)
		ft_add_opt_arg(shell, name_cmd, envp);
	return (TRUE);
}

void	ft_execute_cmd(t_exec *exec, char **envp)
{
	exec->pid = fork();
	if (exec->pid == ERROR)
		perror("ERROR pid");
	 if (exec->pid == 0)
	{
		if (dup2(exec->infile, STDIN_FILENO) == ERROR)
			perror("ERROR dup");
		if (exec->outfile > 2)
		{
			if (dup2(exec->outfile, STDOUT_FILENO) == ERROR)
				perror("ERROR dup");

		}
		if (execve(exec->cmd_path, exec->cmd, envp) == ERROR)
			perror("ERROR execve");
	}
	else
	{
		if (waitpid(exec->pid, NULL, 0) == ERROR)
			perror("ERROR waitpid");
	}
}

void	ft_check_execute(t_shell *shell, char **envp)
{
	t_cmds	*lst;
	int		i;
	t_exec	*exec;

	lst = shell->arg;
	exec = shell->exec;
	while (lst)
	{	
		i = 0;
		while (lst->value_split[i])
		{
			if (ft_check_first(shell, envp, lst->value_split[0]) == FALSE
				&& i == 0)
				break ;
			if (lst->hdoc == TRUE)
				ft_init_heredoc(shell);
			else if (ft_check_infile(exec, lst->value_split, i) == FALSE)
				break ;
			else if (ft_check_outfile(exec, lst->value_split, i) == FALSE)
				break ;
			else if (ft_check_cmd(shell, lst->value_split[i], envp, lst->value_split, i) == FALSE)
			 	break ;
			if (lst->value_split[i] != NULL)
			i++;
		}
		/* ft_printf("%s\n", exec->cmd[0]); */
		/* ft_printf("%s\n", exec->cmd[1]); */
		/* ft_printf("%s\n", exec->cmd[2]); */
		/* ft_printf("%s\n", exec->cmd[3]); */
		if (shell->exec->cmd != NULL)
			ft_execute_cmd(shell->exec, envp);
		if (exec->infile > 2)
			close(exec->infile);
		if (exec->outfile > 2)
			close(exec->outfile);
		if (shell->exec != NULL)
			ft_free_exec(shell);
		lst = lst->next;
	}
}

void	ft_minishell(t_shell *shell, char **envp)
{
	if (ft_check_error_redirect(shell) == FALSE)
		return ;
	//ft_init_heredoc(shell);
	ft_count_heredoc(shell);
	ft_check_execute(shell, envp);
}
