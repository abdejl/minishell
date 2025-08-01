/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 11:59:52 by abjellal          #+#    #+#             */
/*   Updated: 2025/07/20 11:59:53 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(char *prefix, char *target, char *message)
{
	char	error_buffer[1024]; // A buffer to build the string
	// Safely build the full error message in the buffer
	ft_strlcpy(error_buffer, prefix, sizeof(error_buffer));
	if (target)
		ft_strlcat(error_buffer, target, sizeof(error_buffer));
	if (message)
		ft_strlcat(error_buffer, message, sizeof(error_buffer));
	
	// Write the entire buffer in a single system call
	write(STDERR_FILENO, error_buffer, ft_strlen(error_buffer));
}