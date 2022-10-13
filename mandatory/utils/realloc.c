/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcloarec <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 10:59:37 by mcloarec          #+#    #+#             */
/*   Updated: 2022/10/13 14:18:30 by mcloarec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_cdup(char c)
{
	char	*dest;

	dest = malloc(sizeof(char) * 2);
	if (!dest)
		return (NULL);
	dest[0] = c;
	dest[1] = '\0';
	return (dest);
}

char	*ft_charjoin(char *s1, char c)
{
	char	*result;
	int		len;
	int		i;

	result = NULL;
	if (!s1 && !c)
		return (NULL);
	else if (!s1)
	{
		result = ft_cdup(c);
		if (result == NULL)
			return (NULL);
		return (result);
	}
	len = ft_strlen(s1);
	result = malloc(sizeof(char) * len + 2);
	if (!result)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	result[i] = c;
	i++;
	result[i] = '\0';
	free(s1);
	return (result);
}

char	**ft_realloc_tab_char(char **old_tab, char *new_var)
{
	char	**new_tab;
	int		i;
	int		size;

	i = 0;
	size = 0;
	while (old_tab[size] != NULL)
		size++;
	new_tab = ft_calloc(sizeof(char *), (size + 2));
	if (!new_tab)
		return (NULL);
	while (old_tab[i] != NULL)
	{
		new_tab[i] = old_tab[i];
		i++;
	}
	new_tab[i] = ft_strdup(new_var);
	if (new_tab[i] == NULL)
		return (NULL);
	i++;
	new_tab[i] = NULL;
	new_var = NULL;
	free(old_tab);
	return (new_tab);
}