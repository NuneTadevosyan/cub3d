#include "cub.h"

int mapp(t_map *m, int fd)
{
    char *line;
    int linelen = 0;
    int count = 0;
    while (get_next_line(fd, &line))
    {
        int i = 0;
        while(line[i] == ' ' || line[i] == '\t')
            ++i;
        if(line[i] == 'R')
        {
            ++i;
            m->width = ft_atoi(line + i);
            if (m->width > 1920)
                m->width = 1920;
            i+= ft_numlen(m->width);
            m->height = ft_atoi(line + i);
            if (m->height > 1080)
                m->height = 1080;
        }
        else if(line[i] == 'N' && line[i + 1] == 'O')
        {
            i += 2;
            while(line[i] == ' ' || line[i] == '\t')
                ++i;
           m->northfd = ft_strcpy(m->northfd, line + i);
        }
        else if(line[i] == 'S' && line[i + 1] == 'O')
        {
            i += 2;
            while(line[i] == ' ' || line[i] == '\t')
                ++i;
            m->southfd = ft_strcpy(m->southfd, line + i);
        }
        else if(line[i] == 'W' && line[i + 1] == 'E')
        {
            i += 2;
            while(line[i] == ' ' || line[i] == '\t')
                ++i;
            m->westfd = ft_strcpy(m->westfd, line + i);
        }
        else if(line[i] == 'E' && line[i + 1] == 'A')
        {
            i += 2;
            while(line[i] == ' ' || line[i] == '\t')
                ++i;
            m->eastfd = ft_strcpy(m->eastfd, line + i);
        }
        else if(line[i] == 'S')
        {
            ++i;
            while(line[i] == ' ' || line[i] == '\t')
                ++i;
            m->spritefd = ft_strcpy(m->spritefd, line + i);
        }
        else if(line[i] == 'F')
        {
            ++i;
            while(line[i] == ' ' || line[i] == '\t')
                ++i;
            int x = 0;
            int y = 0;
            int z = 0;
            x = ft_atoi(line + i);
            i += ft_numlen(x) + 1;
            y = ft_atoi(line + i);
            i += ft_numlen(y) + 1;
            z = ft_atoi(line + i);
            m->floor = create_rgb(x,y,z);
        }
        else if(line[i] == 'C')
        {
            ++i;
            while(line[i] == ' ' || line[i] == '\t')
                ++i;
            int x = 0;
            int y = 0;
            int z = 0;
            x = ft_atoi(line + i);
            i += ft_numlen(x) + 1;
            y = ft_atoi(line + i);
            i += ft_numlen(y) + 1;
            z = ft_atoi(line + i);
            m->ceiling = create_rgb(x,y,z);
        }
        else
        {
            if(line[0] == '\0')
                continue;
            if (linelen < (int)ft_strlen(line))
                linelen = ft_strlen(line);
            count++;
        }
    }
    if(line[0] != '\0')
        count++;
    close(fd);
    fd = open(m->fname, O_RDONLY);
    m->maph = count;
    m->mapw = linelen;
    m->map = malloc(sizeof(char*)*count);
    for (int i = 0; i < count; i++)
        m->map[i] = malloc(linelen);
    count = 0;
    int j = 0;
    int c = 0;
    int s = 0;
    while (get_next_line(fd, &line))
    {
        int i = 0;
        while(line[i] == ' ' || line[i] == '\t')
            ++i;
        if(line[i] == 'R')
            continue;
        else if(line[i] == 'N' && line[i + 1] == 'O')
            continue;
        else if(line[i] == 'S' && line[i + 1] == 'O')
            continue;
        else if(line[i] == 'W' && line[i + 1] == 'E')
            continue;
        else if(line[i] == 'E' && line[i + 1] == 'A')
            continue;
        else if(line[i] == 'S')
            continue;
        else if(line[i] == 'F')
            continue;
        else if(line[i] == 'C')
            continue;
        else
        {
            j = 0;
            c = 0;
            if(line[0] == '\0')
                continue;
            while(line[j] == ' ' || line[j] == '\t')
             {
                    m->map[count][c++] = '0';
                    j++;
             }
            while(line[j])
            {   
                if(line[j] != '\t' && line[j] != ' ')
                {
                   m->map[count][c++] = line[j];
                    if (line[j++] == '2')
                        s++;
                }
                else 
                    j++;
            }
            while(c < linelen)
                m->map[count][c++] = '0';
            count++;
        }
    }
    if(line[0] != '\0')
    {
        c = 0;
        j = 0;
        while(line[j] == ' ' || line[j] == '\t')
        {
             m->map[count][c++] = '0';
             j++;
        }
        while(line[j])
        {   
            if(line[j] != '\t' && line[j] != ' ')
            {
                m->map[count][c++] = line[j];
                if (line[j++] == '2')
                    s++;
            }
            else 
                j++;
        }
        while(c < linelen)
            m->map[count][c++] = '0';
        count++;
    }
    close(fd);
    return (s);
}

void init(t_map *m)
{
    int i;
    i = 0;
    int j;
    int c = 0;
    while(i < m->maph)
    {
        j = 0;
        while(j < m->mapw)
        {
            if(m->map[i][j] == 'S')
            {
                m->p.pos_x = i + 0.5;
                m->p.pos_y = j + 0.5;
                m->p.xdir = 1;
                m->p.ydir = 0;
                m->p.xplane = 0;
                m->p.yplane = 0.66;
                break;
            }
            else if(m->map[i][j] == 'N')
            {
                m->p.pos_x = i + 0.5;
                m->p.pos_y = j + 0.5;
                m->p.xdir = -1;
                m->p.ydir = 0;
                m->p.xplane = 0;
                m->p.yplane = -0.66;
                break;
            }
            else if(m->map[i][j] == 'E')
            {
                m->p.pos_x = i + 0.5;
                m->p.pos_y = j + 0.5;
                m->p.xdir = 0;
                m->p.ydir = 1;
                m->p.xplane = -0.66;
                m->p.yplane = 0;
                break;
            }
            else if(m->map[i][j] == 'W')
            {
                m->p.pos_x = i + 0.5;
                m->p.pos_y = j + 0.5;
                m->p.xdir = 0;
                m->p.ydir = -1;
                m->p.xplane = 0.66;
                m->p.yplane = 0;
                break;
            }
            else if (m->map[i][j] == '2')
            {
                m->s[c].x = i + 0.5;
                m->s[c].y = j + 0.5;
                c++;
            }
            j++;
        }
        i++;
    }
}

int error_check(t_map *m)
{
    int fd = open(m->fname, O_RDONLY);
    char *line;
    while (get_next_line(fd, &line))
    {
        int i = 0;
        if (line[i] == 'R' || (line[i] == 'N' && line[i + 1] == 'O') || (line[i] == 'S' && line[i + 1] == 'O') || 
            (line[i] == 'W' && line[i + 1] == 'E') || (line[i] == 'E' && line[i + 1] == 'A') || line[i] == 'S' || line[i] == 'F' || line[i] == 'C')
            continue;
        else
        {
            while(line[i])
            {
                if (line[i] == '0' || line[i] == '1' || line[i] == '2' ||
                    line[i] == 'W' || line[i] == 'E' || line[i] == 'S' || line[i] == 'N' || line[i] == ' ' || line[i] == '\t')
                    ++i;
                else
                    return (-1);
            }
        }
    }
    close(fd);
    return (1);
}