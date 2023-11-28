/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_new.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartos <mbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 15:02:15 by mbartos           #+#    #+#             */
/*   Updated: 2023/11/28 17:51:43 by mbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	execute(char *argv, char **env)
{
	char	*path;
	char	**cmd;

	path = get_path(argv, env);
	cmd = get_cmd(argv);
	if (path == NULL)
	{
		ft_putstr_fd("Command not found\n", 2);
		exit(1);
	}
	execve(path, cmd, env);
	perror("Error execve");
	exit(1);
}

void	pipe_fork(char *argv, char **env)
{
	int	p_fd[2];
	int	p_id;

	if (pipe(p_fd) == -1)
		perror("Pipe error");
	p_id = fork();
	if (p_id == -1)
		perror("Fork error");
	if (p_id == 0)
	{
		close(p_fd[0]);
		dup2(p_fd[1], STDOUT_FILENO);
		execute(argv, env);
	}
	else
	{
		close(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
	}
}

// void	here_doc_put_in(char *argv, int *p_fd)
// {
// 	char	*ret;

// 	ft_putchar_fd('B', 1);
// 	close(p_fd[0]);
// 	ft_putchar_fd('C', 1);
// 	while (1)
// 	{
// 		ft_putchar_fd('D', 1);
// 		ret = get_next_line(0);
// 		ft_putstr_fd(ret, 1);
// 		ft_putchar_fd('E', 1);
// 		if (ft_strncmp(ret, argv, ft_strlen(argv)) == 0)
// 		{
// 			free(ret);
// 			close(p_fd[1]);
// 			return ;
// 		}
// 		ft_putstr_fd(ret, p_fd[1]);
// 		free(ret);
// 	}
// }

// void	here_doc(char *argv)
// {
// 	int		p_fd[2];
// 	pid_t	pid;

// 	if (pipe(p_fd) == -1)
// 		exit(0);
// 	pid = fork();
// 	if (pid == -1)
// 		exit(0);
// 	if (pid == 0)
// 	{
// 		ft_putchar_fd('b', 1);
// 		here_doc_put_in(argv, p_fd);
// 	}
// 	else
// 	{
// 		close(p_fd[1]);
// 		dup2(p_fd[0], STDIN_FILENO);
// 		wait(NULL);
// 		ft_putchar_fd('c', 1);
// 	}
// }

/* COPIED CODE FROM OREZEK FOR TESTING*/
int	ft_pip_strncmp(const char *s1, const char *s2, size_t size)
{
	unsigned char		start;
	const unsigned char	*str1;
	const unsigned char	*str2;

	str1 = (const unsigned char *) s1;
	str2 = (const unsigned char *) s2;
	start = 0;
	if (ft_strlen(s1) != ft_strlen(s2))
		return (-1);
	while (start < size)
	{
		if (*(str1 + start) == '\0' && *(str2 + start) == '\0')
			break ;
		else if (*(str1 + start) < *(str2 + start))
			return (*(str1 + start) - *(str2 + start));
		else if (*(str1 + start) > *(str2 + start))
			return (*(str1 + start) - *(str2 + start));
		start++;
	}
	return (0);
}

char	*ft_sanitize_line(char *str)
{
	int		len;
	char	*new_line;
	int		i;

	if (str == NULL)
		return (NULL);
	len = ft_strlen(str);
	if (str[len - 1] == '\n')
	{
		new_line = malloc (len * sizeof(char));
		i = 0;
		while (i < len - 1)
		{
			new_line[i] = str[i];
			i++;
		}
		new_line[i] = '\0';
		return (new_line);
	}
	return (str);
}

// read heredoc and save the text to a temp file
void	ft_read_heredoc(char *limiter)
{
	char	*line;
	int		heredoc_fd;
	char	*clean_line;

	heredoc_fd = open("here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	ft_putstr_fd("heredoc> ", STDOUT_FILENO);
	line = get_next_line(STDIN_FILENO);
	clean_line = ft_sanitize_line(line);
	while (ft_pip_strncmp(clean_line, limiter, ft_strlen(limiter)) != 0)
	{
		write(heredoc_fd, line, ft_strlen(line) * sizeof(char));
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
		free(line);
		free(clean_line);
		line = get_next_line(STDIN_FILENO);
		clean_line = ft_sanitize_line(line);
	}
	close(heredoc_fd);
}
/* COPIED CODE FROM OREZEK FOR TESTING*/

int	main(int argc, char **argv, char **env)
{
	int	i;
	int	fd_in;
	int	fd_out;

	if (argc < 4)
	{
		ft_putstr_fd("Error format: ./pipex file1 cmd1 .. cmdn file2\n", 2);
		ft_putstr_fd("./pipex here_doc LIMITER cmd1 .. cmdn file2\n", 2);
		return (1);
	}
	if (ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")) == 0 && argc > 5)
	{
		ft_read_heredoc(argv[2]);
		fd_in = open("here_doc", O_RDONLY, 0777);
		fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (fd_in < 0 || fd_out < 0)
		{
			perror("File error");
			return (1);
		}
		dup2(fd_in, STDIN_FILENO);
		i = 3;
	}
	else if (argc > 4)
	{
		fd_in = open(argv[1], O_RDONLY, 0777);
		fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (fd_in < 0 || fd_out < 0)
		{
			perror("File error");
			return (1);
		}
		dup2(fd_in, STDIN_FILENO);
		i = 2;
	}
	else
	{
		ft_putstr_fd("Error format: ./pipex file1 cmd1 .. cmdn file2\n", 2);
		ft_putstr_fd("./pipex here_doc LIMITER cmd1 .. cmdn file2\n", 2);
		return (1);
	}
	while (i < (argc - 2))
	{
		pipe_fork(argv[i], env);
		i++;
	}
	unlink("here_doc");
	dup2(fd_out, STDOUT_FILENO);
	execute(argv[i], env);
	return (1);
}
