# include <curses.h>
# include <term.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <sys/types.h>
# include <signal.h>
# include <sys/wait.h>
# include <termios.h>
# include <termcap.h>
# include <ncurses.h>


int			ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] && s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

size_t	ft_strlen(const char *str)
{
	size_t i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char		*ft_strjoin(char const *s1, char const *s2)
{
	size_t	total_len;
	size_t	s1_len;
	size_t	i;
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	total_len = ft_strlen(s1) + ft_strlen(s2);
	s1_len = ft_strlen(s1);
	if (!(str = malloc(sizeof(char) * (total_len + 1))))
		return (NULL);
	i = 0;
	while (i < total_len)
	{
		str[i] = (i < s1_len) ? s1[i] : s2[i - s1_len];
		i++;
	}
	str[i] = '\0';
	return (str);
}

void    disable_enter_key(struct termios *info)
{
    info->c_lflag &= ~(ECHO);      /* disable enter key */
}

void    init_termios(struct termios *info, int disable)
{
	tcgetattr(0, info);          /* get current terminal attirbutes; 0 is the file descriptor for stdin */
	info->c_lflag &= ~ICANON;      /* disable canonical mode */
    if (disable)
        disable_enter_key(info);
	info->c_cc[VMIN] = 1;          /* wait until at least one keystroke available */
	info->c_cc[VTIME] = 0;         /* no timeout */
    // info->c_cc[VERASE] = 27;
    // info->c_cc[VEOF] = 4;
	tcsetattr(0, TCSANOW, info); /* set immediately */
}

void    restore_termios(struct termios *info)
{
	tcgetattr(0, info);
	info->c_lflag |= ICANON;
	tcsetattr(0, TCSANOW, info);
}


int		main(int ac, char **av)
{
	char	*buffer;
    char	*prev_buffer;
    char    *prev_tmp;
    int     i;

	struct termios info;
    init_termios(&info, 1);
	// tcgetattr(0, &info);          /* get current terminal attirbutes; 0 is the file descriptor for stdin */
	// info.c_lflag &= ~ICANON;      /* disable canonical mode */
	// info.c_lflag &= ~(ECHO);      /* disable enter key */
	// info.c_cc[VMIN] = 1;          /* wait until at least one keystroke available */
	// info.c_cc[VTIME] = 0;         /* no timeout */
	// tcsetattr(0, TCSANOW, &info); /* set immediately */


	buffer = malloc(2048);
    prev_buffer = malloc(2048);
    i = 0;
	while (1)
	{
		read(0, buffer, 1);
		if (buffer[0] == 27)
		{
			read(0, buffer, 1);
			if (buffer[0] == '[')
			{
				read(0, buffer, 1);
				if (buffer[0] == 'A')  // ^[[A
					printf("fleche du haut\n");
				else if (buffer[0] == 'B')  // ^[[B
					printf("fleche du bas\n");
				else if (buffer[0] == 'C')  // ^[[C
                {
                    if (i > ft_strlen(prev_buffer))
                        i = ft_strlen(prev_buffer);
                    else if (i < ft_strlen(prev_buffer))
                        i++;
					// printf("fleche droite\n");
                    write (1, " ", 0);
                }
				else if (buffer[0] == 'D')  // ^[[D
                {
                    if (i > ft_strlen(prev_buffer))
                        i = ft_strlen(prev_buffer);
                    else
                        i++;
					// printf("fleche gauche\n");
                    // write (1, "\b", 1);
                    // write (1, prev_buffer - i, 1);
                    // write (1, "\b", 1);

                    write (1, "\b", 1);
                }
				// memset(buffer, 0, 1);
			}
		}
        else if (buffer[0] == 127)
        {
            write (1, "\b \b", 3);
        }
        else if (buffer[0] == 4)
        {
            write (1, "\n", 1);
        } 
		else
		{
			// info.c_lflag &= ~ICANON;      /* disable canonical mode */
			// info.c_cc[VMIN] = 1;          /* wait until at least one keystroke available */
			// info.c_cc[VTIME] = 0;         /* no timeout */
			// tcsetattr(0, TCSANOW, &info); /* set immediately */
            // init_termios_without_enter_key(&info);
            init_termios(&info, 0);

			write(0, buffer, 1);
            prev_tmp = prev_buffer;
            prev_buffer = ft_strjoin(prev_buffer, buffer);
            free(prev_tmp);


            disable_enter_key(&info);
			// info.c_lflag &= ~(ECHO);
		}
	}
	// tcgetattr(0, &info);
	// info.c_lflag |= ICANON;
	// tcsetattr(0, TCSANOW, &info);
    restore_termios(&info);
	free(buffer);
}
