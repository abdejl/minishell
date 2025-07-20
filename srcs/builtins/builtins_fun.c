/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_fun.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 09:21:16 by brbaazi           #+#    #+#             */
/*   Updated: 2025/07/20 10:22:24 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	save_oldpwd(char *cwd, char **oldpwd)
{
	if (!getcwd(cwd, 1024))
		return (1);
	*oldpwd = ft_strdup(cwd);
	if (!*oldpwd)
		return (1);
	return (0);
}
