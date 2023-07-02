/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuramot <tkuramot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 19:19:49 by tkuramot          #+#    #+#             */
/*   Updated: 2023/07/02 13:44:22 by tkuramot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static int	g_received;

static void	send_char(pid_t pid, unsigned char c)
{
	unsigned char	mask;

	mask = 0b10000000;
	while (mask)
	{
		if (mask & c)
		{
			if (kill(pid, SIGUSR1) < 0)
				return ;
		}
		else
		{
			if (kill(pid, SIGUSR2) < 0)
				return ;
		}
		mask >>= 1;
		usleep(50);
		while (!(g_received == BIT_RECEIVED))
			pause();
		usleep(50);
	}
}

static void	send_str(pid_t pid, char *c)
{
	size_t	index;
	size_t	len;

	index = 0;
	len = ft_strlen(c);
	while (index < len)
	{
		send_char(pid, c[index]);
		index++;
	}
}

static void	handler(int signal, siginfo_t *server, void *con)
{
	(void)server;
	(void)con;
	if (signal == SIGUSR1)
		g_received = BIT_RECEIVED;
}

static void	initialize_sigaction(void)
{
	struct sigaction	act;

	ft_bzero(&act, sizeof(struct sigaction));
	act.sa_sigaction = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
}

int	main(int argc, char **argv)
{
	int pid;

	if (argc != 3)
	{
		ft_printf("Usage: ./client pid message\n");
		exit(1);
	}
	pid = ft_atoi(argv[1]);
	if (ft_strlen(argv[1]) != (size_t)get_digit_count(pid, 10)
			|| pid < 100 || 99998 < pid)
	{
		ft_printf("The process id is invalid\n");
		exit(1);
	}
	initialize_sigaction();
	send_str((pid_t)ft_atoi(argv[1]), argv[2]);
	return (0);
}
