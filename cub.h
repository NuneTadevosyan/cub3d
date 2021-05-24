#ifndef CUB_H
#define CUB_H

#include "minilibx/mlx.h"
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
# include <time.h>
#define texWidth 64
#define texHeight 64

typedef struct Sprite
{
  double x;
  double y;
} t_sprite;

typedef struct			s_sound
{
	clock_t				last_start_song;
}						t_sound;

typedef struct  s_bool 
{
    int     w;
    int     a;
    int     d;
    int     s;
    int     l;
    int     r;
}               t_bool;

typedef struct  s_data {
    void        *img;
    char        *addr;
    int         bits_per_pixel;
    int         line_length;
    int         endian;
    int            width;
    int         length;
}               t_data;

typedef struct s_player
{
    float pos_x;
    float pos_y;
    float xdir;
    float ydir;
    float xplane;
    float yplane;
}   t_player;

typedef struct s_map
{
    int width;
    int height;
    void *mlx_ptr;
    void *win_ptr;
    char *southfd;
    char *westfd;
    char *northfd;
    char *eastfd;
    char *spritefd;
    int floor;
    int ceiling;
    t_player p;
    int mapw;
    int maph;
    char **map;
    t_data *data;
    t_sprite *s;
    int sn;
    t_bool mi_ban;
    t_sound *sound;
    char *fname;
    int save;
}               t_map;

int		get_next_line(int fd, char **line);
void	ft_bzero(void *d, size_t size);
void swapp(double * dst, int i,int j);
int		ft_atoi(const char *str);
size_t		ft_strlen(const char *s);
int ft_numlen(int x);
int mapp(t_map *m, int fd);
int		create_rgb(int r, int g, int b);
char *ft_strcpy(char *dest, char *src);
void init(t_map *m);
void bubble(int* order, double* dist, int amount);
int error_check(t_map *m);
void			screenshot(t_map *cub);
unsigned	int	get_pixel(t_data *data, int x, int y);
#endif