/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcloarec <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 11:25:34 by mcloarec          #+#    #+#             */
/*   Updated: 2022/10/29 18:27:59 by clorcery         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_add_opt_arg(t_shell *shell, char **tab, int i)
{
	if (ft_check_q(tab[i][0]) == 1 && ft_is_not_redirection(tab[i]) == TRUE)
		tab[i] = ft_delete_quotes_redirect(shell, tab[i]);
	shell->exec->cmd = ft_realloc_tab_char(shell->exec->cmd, tab[i]);
	if (shell->exec->cmd == NULL)
		ft_free_malloc(shell);
}

int	ft_add_cmd(t_shell *shell, char *s, char **envp)
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

int	ft_check_cmd(t_shell *shell, char **envp, char **tab, int i)
{
	if (i == 0 && shell->exec->cmd_path == NULL
		&& ft_valid_redirect(tab[i]) == FALSE)
		ft_get_path(shell, tab[i], envp);
	else if (i > 0 && shell->exec->cmd_path == NULL
		&& ft_valid_redirect(tab[i]) == FALSE
		&& ft_valid_redirect(tab[i - 1]) == FALSE)
		ft_get_path(shell, tab[i], envp);
	if (i == 0 && ft_valid_redirect(tab[i]) == FALSE)
		ft_add_cmd(shell, tab[i], envp);
	else if (i > 0 && shell->exec->cmd_path != NULL
		&& shell->exec->cmd == NULL
		&& ft_valid_redirect(tab[i - 1]) == FALSE
		&& ft_valid_redirect(tab[i]) == FALSE)
		ft_add_cmd(shell, tab[i], envp);
	else if (i > 0 && shell->exec->cmd != NULL
		&& ft_valid_redirect(tab[i]) == FALSE
		&& ft_valid_redirect(tab[i - 1]) == FALSE)
		ft_add_opt_arg(shell, tab, i);
	return (TRUE);
}
