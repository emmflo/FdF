/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eflorenz <eflorenz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/08 22:55:49 by eflorenz          #+#    #+#             */
/*   Updated: 2017/08/08 23:28:54 by eflorenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

int			ft_lstlen(t_list *list)
{
	int	i;

	i = 0;
	while (list != NULL)
	{
		list = list->next;
		i++;
	}
	return (i);
}

static void	add_to_result(t_list **r, t_list **result, t_list **ab)
{
	if (*result == NULL)
	{
		*result = *ab;
		*r = *result;
	}
	else
	{
		(*result)->next = *ab;
		*result = (*result)->next;
	}
	*ab = (*ab)->next;
}

t_list		*ft_merge(t_list *a, t_list *b, int cmp(void *, void *))
{
	t_list	*r;
	t_list	*result;

	r = NULL;
	result = NULL;
	while (a != NULL && b != NULL)
	{
		if (cmp(a->content, b->content) <= 0)
			add_to_result(&r, &result, &a);
		else
			add_to_result(&r, &result, &b);
	}
	if (a != NULL)
		result->next = a;
	else if (b != NULL)
		result->next = b;
	return (r);
}

t_list		*ft_merge_sort(t_list *list, int cmp(void *, void *))
{
	t_list	*left;
	t_list	*right;
	t_list	*ptr;
	int		i;
	int		len;

	if (list == NULL || list->next == NULL)
		return (list);
	i = 0;
	ptr = list;
	len = ft_lstlen(list);
	while (i < (len / 2) - 1)
	{
		ptr = ptr->next;
		i++;
	}
	right = ptr->next;
	ptr->next = NULL;
	left = list;
	left = ft_merge_sort(left, cmp);
	right = ft_merge_sort(right, cmp);
	return (ft_merge(left, right, cmp));
}
