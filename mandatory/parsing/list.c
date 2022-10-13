/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorcery <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 18:45:51 by clorcery          #+#    #+#             */
/*   Updated: 2022/10/13 16:35:29 by mcloarec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_create_lst(t_shell *shell)
{
	int		i;
	t_cmds	*new;

	i = 1;
	shell->arg = ft_lstnew_cmd(shell->tab_cmd[0], shell);
	if (shell->arg == NULL)
		ft_free_malloc(shell);
	while (shell->tab_cmd[i] != NULL)
	{
		new = ft_lstnew_cmd(shell->tab_cmd[i], shell);
		if (new == NULL)
			ft_free_malloc(shell);
		ft_lstaddback_cmd(&shell->arg, new);
		i++;
	}
}