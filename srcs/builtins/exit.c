/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brbaazi <brbaazi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:18:33 by brbaazi           #+#    #+#             */
/*   Updated: 2025/07/15 13:18:33 by brbaazi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	exit_with_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR);
	ft_putstr_fd(arg, STDERR);
	ft_putendl_fd(": numeric argument required", STDERR);
	exit(255);
}

static void	too_many_args_error(t_shell *shell)
{
	ft_putendl_fd("minishell: exit: too many arguments", STDERR);
	shell->exit_status = 1;
}

int	mini_exit(t_shell *shell, t_cmd *cmd)
{
	char	**args;
	long	code;

	args = cmd->args;
	if (!cmd->next)
		ft_putendl_fd("exit", STDOUT);
	if (!args[1])
		exit(shell->exit_status);
	if (is_numeric(args[1]) == 0)
		exit_with_error(args[1]);
	if (args[2])
	{
		too_many_args_error(shell);
		return (1);
	}
	code = ft_atoi(args[1]);
	if (code > 255)
		code %= 256;
	else if (code < 0)
		code = 256 + (code % 256);
	exit((unsigned char)code);
}
