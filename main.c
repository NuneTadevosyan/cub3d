#include "cub.h"
t_data s_img;
t_data n_img;
t_data e_img;
t_data w_img;
t_data sp_img;

int	init_sound(t_map *win_infos)
{
	if (!(win_infos->sound = malloc(sizeof(t_sound))))
		return (-1);
	ft_bzero(win_infos->sound, sizeof(t_sound));
	win_infos->sound->last_start_song = clock();
	system("afplay -v 0.30 includes/ACDC-Highway-to-Hell.mp3 &>/dev/null &");
	return (-1);
}

void	play_music(t_map *win_infos)
{
	long	clk_tck;
	clock_t	actual_time;

	actual_time = clock();
	clk_tck = CLOCKS_PER_SEC;
	if ((double)(actual_time - win_infos->sound->last_start_song)
		/ (double)clk_tck >= 129.5)
	{
		system("killall afplay");
		system("afplay -v 0.30 includes/ACDC-Highway-to-Hell.mp3 &>/dev/null &");
		win_infos->sound->last_start_song = actual_time;
	}
}

void    ft_init(t_map *data)
{
    data->mi_ban.w = 0;
    data->mi_ban.a = 0;
    data->mi_ban.d = 0;
    data->mi_ban.s = 0;
    data->mi_ban.l = 0;
    data->mi_ban.r = 0;
}

void            my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char    *dst;

    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

void clear(t_map *m)
{
    int i = 0;
    while (i < m->height)
    {
        int j = 0;
        while (j < m->width)
        {
            my_mlx_pixel_put(m->data, j, i, 0x000000);
            j++;
        }
        ++i;
    }
}

int exit_hook()
{
    system("killall afplay");
    exit(0);
    return(1);
}

unsigned	int	get_pixel(t_data *data, int x, int y)
{
	unsigned int	color;
	char			*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	color = *(unsigned int*)dst;
	return (color);
}

void draw_rays(t_player* p, t_map m)
{
    int buffer[m.height][m.width];
    double ZBuffer[m.width];
    int spriteOrder[m.sn];
    double spriteDistance[m.sn];
    int drawStart = 0;
    int drawEnd = 0;
    for(int x = 0; x < m.width; x++)
    {
      double cameraX = 2 * x / (double)m.width - 1;
      double rayDirX = p->xdir + p->xplane * cameraX;
      double rayDirY = p->ydir + p->yplane * cameraX;
      int mapX = (int)(p->pos_x);
      int mapY = (int)(p->pos_y);
      double sideDistX;
      double sideDistY;
      double deltaDistX = fabs(1 / rayDirX);
      double deltaDistY = fabs(1 / rayDirY);
      double perpWallDist;
      int stepX;
      int stepY;
      int hit = 0;
      int side;
      if(rayDirX < 0)
      {
        stepX = -1;
        sideDistX = (p->pos_x - mapX) * deltaDistX;
      }
      else
      {
        stepX = 1;
        sideDistX = (mapX + 1.0 - p->pos_x) * deltaDistX;
      }
      if(rayDirY < 0)
      {
        stepY = -1;
        sideDistY = (p->pos_y - mapY) * deltaDistY;
      }
      else
      {
        stepY = 1;
        sideDistY = (mapY + 1.0 - p->pos_y) * deltaDistY;
      }
      while (hit == 0)
      {
        if(sideDistX < sideDistY)
        {
          sideDistX += deltaDistX;
          mapX += stepX;
          side = 0;
        }
        else
        {
          sideDistY += deltaDistY;
          mapY += stepY;
          side = 1;
        }
       if(m.map[mapX][mapY] == '1') hit = 1;
      }
      if(side == 0) perpWallDist = (mapX - p->pos_x + (1 - stepX) / 2) / rayDirX;
      else          perpWallDist = (mapY - p->pos_y + (1 - stepY) / 2) / rayDirY;

      int lineHeight = (int)(m.height / perpWallDist);
      drawStart = -lineHeight / 2 + m.height / 2;
      if(drawStart < 0)drawStart = 0;
      drawEnd = lineHeight / 2 + m.height / 2;
      if(drawEnd >= m.height)drawEnd = m.height - 1;

      double wallX; 
      if (side == 0) wallX = p->pos_y + perpWallDist * rayDirY;
      else           wallX = p->pos_x + perpWallDist * rayDirX;
      wallX -= floor((wallX));

      int texX = (int)(wallX * (double)texWidth);
      if(side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
      if(side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

      double step = 1.0 * texHeight / lineHeight;
      double texPos = (drawStart - m.height / 2 + lineHeight / 2) * step; 
      
      for(int y = drawStart; y<drawEnd; y++)
      {
        int texY = (int)texPos & (texHeight - 1);
        texPos += step;
        int color;
        if(side == 0)
        {
            if(rayDirX < 0)
                color = get_pixel(&n_img, texX, texY);
            else 
                 color = get_pixel(&s_img, texX, texY);
        }
        else
        {
            if(rayDirY < 0)
                color = get_pixel(&e_img, texX, texY);
            else 
                 color = get_pixel(&w_img, texX, texY);
        }
        buffer[y][x] = color;
      }
      for (int y = 0; y < drawStart; y++)
        my_mlx_pixel_put(m.data, x, y, m.ceiling);
      for (int y = drawStart; y < drawEnd; y++)
         my_mlx_pixel_put(m.data, x, y, buffer[y][x]);
      for (int y = drawEnd; y < m.height; y++)
        my_mlx_pixel_put(m.data, x, y, m.floor);
     ZBuffer[x] = perpWallDist;  
    }
    for(int i = 0; i < m.sn; i++)
    {
      spriteOrder[i] = i;
      spriteDistance[i] = ((m.p.pos_x - m.s[i].x) * (m.p.pos_x - m.s[i].x) + (m.p.pos_y - m.s[i].y) * (m.p.pos_y - m.s[i].y));
    }
    bubble(spriteOrder, spriteDistance, m.sn);
    
    for(int i = 0; i < m.sn; i++)
    {
      double spriteX = m.s[spriteOrder[i]].x - m.p.pos_x;
      double spriteY = m.s[spriteOrder[i]].y - m.p.pos_y;
      double invDet = 1.0 / (m.p.xplane * m.p.ydir - m.p.xdir * m.p.yplane);
      double transformX = invDet * (m.p.ydir * spriteX - m.p.xdir * spriteY);
      double transformY = invDet * (-m.p.yplane * spriteX + m.p.xplane * spriteY);
      int spriteScreenX = (int)((m.width / 2) * (1 + transformX / transformY));

        #define uDiv 1
      #define vDiv 1
      #define vMove 0
      int vMoveScreen = (int)(vMove / transformY);
      int spriteHeight = abs((int)(m.height / (transformY))) / vDiv;
      int drawStartY = -spriteHeight / 2 + m.height /2 + vMoveScreen;
      if(drawStartY < 0) drawStartY = 0;
      int drawEndY = spriteHeight / 2 + m.height / 2 + vMoveScreen;
      if(drawEndY >= m.height) drawEndY = m.height - 1;
      int spriteWidth = abs((int) (m.height / (transformY))) / uDiv;
      int drawStartX = -spriteWidth / 2 + spriteScreenX;
      if(drawStartX < 0) drawStartX = 0;
      int drawEndX = spriteWidth / 2 + spriteScreenX;
      if(drawEndX >= m.width) drawEndX = m.width - 1;

      for(int stripe = drawStartX; stripe < drawEndX; stripe++)
      {
        int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
        if(transformY > 0 && stripe > 0 && stripe < m.width && transformY < ZBuffer[stripe])
            for(int y = drawStartY; y < drawEndY; y++)
            {
            int d = (y - vMoveScreen) * 256 - m.height * 128 + spriteHeight * 128;
            int texY = ((d * texHeight) / spriteHeight) / 256;
            int color = get_pixel(&sp_img, texX, texY);
            if((color & 0x00FFFFFF) != 0) buffer[y][stripe] = color;
            }
        for(int y = drawStartY; y < drawEndY; y++)
          if((buffer[y][stripe] & 0x00FFFFFF) != 0) my_mlx_pixel_put(m.data, stripe, y, buffer[y][stripe]);
      }
    }
    for(int y = 0; y < m.height; y++) for(int x = 0; x < m.width; x++) buffer[y][x] = 0;
    mlx_put_image_to_window(m.mlx_ptr, m.win_ptr, m.data->img, 0, 0);
    if (m.save)
        screenshot(&m);
}
int key_hook(t_map *m)
{
    double moveSpeed = 0.915 * 0.0085 * 10;
    if (m->mi_ban.s)
    {
        if ((m->map[(int)(m->p.pos_x - m->p.xdir * moveSpeed)][(int)(m->p.pos_y - m->p.ydir * moveSpeed)] != '1') && 
        (m->map[(int)(m->p.pos_x - m->p.xdir * moveSpeed)][(int)(m->p.pos_y - m->p.ydir * moveSpeed)] != '2'))
        {
             m->p.pos_x -= m->p.xdir * moveSpeed;
             m->p.pos_y -= m->p.ydir * moveSpeed;
        }
    }
    if (m->mi_ban.w)
    {
        if ((m->map[(int)(m->p.pos_x + m->p.xdir * moveSpeed)][(int)(m->p.pos_y + m->p.ydir * moveSpeed)] != '1') && 
        (m->map[(int)(m->p.pos_x + m->p.xdir * moveSpeed)][(int)(m->p.pos_y + m->p.ydir * moveSpeed)] != '2'))
        {
             m->p.pos_x += m->p.xdir * moveSpeed;
             m->p.pos_y += m->p.ydir * moveSpeed;
        }
    }
    if (m->mi_ban.l)
    {
        double oldirx = m->p.xdir;
        m->p.xdir = m->p.xdir * cos(-moveSpeed) - m->p.ydir * sin(-moveSpeed);
         m->p.ydir = oldirx  * sin(-moveSpeed) + m->p.ydir  * cos(-moveSpeed);
      double oldPlaneX = m->p.xplane;
     m->p.xplane = m->p.xplane * cos(-moveSpeed) - m->p.yplane * sin(-moveSpeed);
      m->p.yplane = oldPlaneX * sin(-moveSpeed) + m->p.yplane* cos(-moveSpeed);
    }
    if (m->mi_ban.r)
    {
         double oldirx = m->p.xdir;
        m->p.xdir = m->p.xdir * cos(moveSpeed) - m->p.ydir * sin(moveSpeed);
         m->p.ydir = oldirx  * sin(moveSpeed) + m->p.ydir  * cos(moveSpeed);
      double oldPlaneX = m->p.xplane;
     m->p.xplane = m->p.xplane * cos(moveSpeed) - m->p.yplane * sin(moveSpeed);
      m->p.yplane = oldPlaneX * sin(moveSpeed) + m->p.yplane* cos(moveSpeed);
    }
    if (m->mi_ban.a)
    {
        if ((m->map[(int)(m->p.pos_x + m->p.ydir * moveSpeed)][(int)(m->p.pos_y - m->p.xdir * moveSpeed)] != '1') && 
        (m->map[(int)(m->p.pos_x + m->p.ydir * moveSpeed)][(int)(m->p.pos_y - m->p.xdir * moveSpeed)] != '2'))
        {
             m->p.pos_x += m->p.ydir * moveSpeed;
             m->p.pos_y -= m->p.xdir * moveSpeed;
        }
    }
    if (m->mi_ban.d)
    {
        if ((m->map[(int)(m->p.pos_x - m->p.ydir * moveSpeed)][(int)(m->p.pos_y + m->p.xdir * moveSpeed)] != '1') && 
        (m->map[(int)(m->p.pos_x - m->p.ydir * moveSpeed)][(int)(m->p.pos_y + m->p.xdir * moveSpeed)] != '2'))
        {
             m->p.pos_x -= m->p.ydir * moveSpeed;
             m->p.pos_y += m->p.xdir * moveSpeed;
        }
    }
    return (0);
}

int frame(t_map *map)
{
   clear(map);
   key_hook(map);
    draw_rays(&map->p, *map);
    mlx_do_sync(map->mlx_ptr);
    return 1;
}

int         key_press(int keycode, t_map *data)
{
    if (keycode == 53)
    {
        system("killall afplay");
        exit(0);
    }
    if (keycode == 13)
        data->mi_ban.w = 1;
    if (keycode == 1)
        data->mi_ban.s = 1;
    if (keycode == 123)
        data->mi_ban.l = 1;
    if (keycode == 124)
        data->mi_ban.r = 1;
    if (keycode == 0)
        data->mi_ban.a = 1;
    if (keycode == 2)
        data->mi_ban.d = 1;
    return (0);
}

int         key_relase(int keycode, t_map *data)
{
    if (keycode == 13)
        data->mi_ban.w = 0;
    if (keycode == 1)
        data->mi_ban.s = 0;
    if (keycode == 123)
        data->mi_ban.l = 0;
    if (keycode == 124)
        data->mi_ban.r = 0;
    if (keycode == 0)
        data->mi_ban.a = 0;
    if (keycode == 2)
        data->mi_ban.d = 0;
    return (0);
}

int             main(int ac, char **av)
{
    t_map givmap;
    givmap.fname = av[1];
    t_data img;
    givmap.data = &img; 
    givmap.width = 0;
    givmap.height = 0;
    int fd = open(av[1], O_RDONLY);
    ft_init(&givmap);
    if (error_check(&givmap) == -1)
    {
        write(1, "Error in file\n", 14);
        return (0);
    }
    int s = mapp(&givmap,fd);
    t_sprite *sprite = malloc(sizeof(t_sprite)*s);
    givmap.s = malloc(sizeof(t_sprite*));
    givmap.s = sprite;
    givmap.sn = s;
    init(&givmap);
    if (ac == 3)
        givmap.save = 1;
    else
        givmap.save = 0;
    givmap.mlx_ptr = mlx_init();
    givmap.win_ptr = mlx_new_window(givmap.mlx_ptr, givmap.width, givmap.height, "cub3d");
    
    img.img = mlx_new_image(givmap.mlx_ptr, givmap.width, givmap.height);
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
                                &img.endian);
    n_img.img = mlx_png_file_to_image(givmap.mlx_ptr, givmap.northfd,&n_img.width,&n_img.length);
    n_img.addr = mlx_get_data_addr(n_img.img, &n_img.bits_per_pixel, &n_img.line_length, &n_img.endian);

    s_img.img = mlx_png_file_to_image(givmap.mlx_ptr, givmap.southfd,&s_img.width,&s_img.length);
    s_img.addr = mlx_get_data_addr(s_img.img, &s_img.bits_per_pixel, &s_img.line_length, &s_img.endian);

    e_img.img = mlx_png_file_to_image(givmap.mlx_ptr, givmap.eastfd,&e_img.width,&e_img.length);
    e_img.addr = mlx_get_data_addr(e_img.img, &e_img.bits_per_pixel, &e_img.line_length, &e_img.endian);

    w_img.img = mlx_png_file_to_image(givmap.mlx_ptr, givmap.westfd,&w_img.width,&w_img.length);
    w_img.addr = mlx_get_data_addr(w_img.img, &w_img.bits_per_pixel, &w_img.line_length, &w_img.endian);

    sp_img.img = mlx_png_file_to_image(givmap.mlx_ptr, givmap.spritefd,&sp_img.width,&sp_img.length);
    sp_img.addr = mlx_get_data_addr(sp_img.img, &sp_img.bits_per_pixel, &sp_img.line_length, &sp_img.endian);
    init_sound(&givmap);
    play_music(&givmap);
    mlx_hook(givmap.win_ptr, 2,(1l<<0), key_press, &givmap);
    mlx_hook(givmap.win_ptr, 3,(1l<<1), key_relase, &givmap);
    mlx_hook(givmap.win_ptr,17,(1l<<17), exit_hook,0);
    mlx_loop_hook(givmap.mlx_ptr, frame, &givmap);
    mlx_loop(givmap.mlx_ptr);
    free(givmap.s);
    free(sprite);
    for (int i = 0; i < givmap.maph; i++)
        free(givmap.map[i]);
    free(givmap.map);
    return (0);
}