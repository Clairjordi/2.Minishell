/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstadd_front.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorcery <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 18:41:04 by clorcery          #+#    #+#             */
/*   Updated: 2022/07/14 18:43:09 by clorcery         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../all_h.h"

void	ft_dlstadd_front(t_dlist **list, t_dlist *new)
{
	new->next = *list;
	new->prev = NULL;
	*list = new;
}