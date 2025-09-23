/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 21:55:42 by maja              #+#    #+#             */
/*   Updated: 2025/09/23 20:11:33 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <signal.h>

static void	handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();
    // rl_replace_line("", 0);
    rl_redisplay();
}

static void	handle_sigquit(int sig)
{
    (void)sig;
    // Do nothing for Ctrl+\ as per bash behavior
}

void	setup_signals(void)
{
    // Set up the signal handlers
    signal(SIGINT, handle_sigint);   // Ctrl+C
    signal(SIGQUIT, handle_sigquit); // Ctrl+backslash
}
