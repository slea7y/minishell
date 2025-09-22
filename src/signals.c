/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 21:55:42 by maja              #+#    #+#             */
/*   Updated: 2025/09/21 22:44:44 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <signal.h>

static void	handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    write(1, "minishell$ ", 11);
}

void	setup_signals(void)
{
    // Set up the signal handlers
    signal(SIGINT, handle_sigint);   // Ctrl+C
    signal(SIGQUIT, SIG_IGN);        // Ctrl+
}
