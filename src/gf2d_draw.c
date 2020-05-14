#include "gf2d_draw.h"
#include "gf2d_graphics.h"
#include "simple_logger.h"
#include <SDL_TTF.h>

void gf2d_draw_lines(Vector2D *p1,Vector2D *p2, Uint32 lines,Vector4D color)
{
    int i;
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    for (i = 0; i < lines;i++)
    {
        SDL_RenderDrawLine(gf2d_graphics_get_renderer(),
                           p1[i].x,
                           p1[i].y,
                           p2[i].x,
                           p2[i].y);
    }
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           255,
                           255,
                           255,
                           255);
}

void gf2d_draw_line(Vector2D p1,Vector2D p2, Vector4D color)
{
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    SDL_RenderDrawLine(gf2d_graphics_get_renderer(),
                       p1.x,
                       p1.y,
                       p2.x,
                       p2.y);
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           255,
                           255,
                           255,
                           255);
}

void gf2d_draw_rect(SDL_Rect rect,Vector4D color)
{
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    SDL_RenderDrawRect(gf2d_graphics_get_renderer(),(const struct SDL_Rect *)&rect);
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           255,
                           255,
                           255,
                           255);
}

void gf2d_draw_rects(SDL_Rect *rects,Uint32 count,Vector4D color)
{
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    SDL_RenderDrawRects(gf2d_graphics_get_renderer(),rects,count);
}

void gf2d_draw_pixel(Vector2D pixel,Vector4D color)
{
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    SDL_RenderDrawPoint(gf2d_graphics_get_renderer(),
                        pixel.x,
                        pixel.y);
}

void gf2d_draw_pixel_list(SDL_Point * pixels,Uint32 count,Vector4D color)
{
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    SDL_RenderDrawPoints(gf2d_graphics_get_renderer(),
                        pixels,
                        count);
}

/*
 * credit circle drawing algorith:
 * http://groups.csail.mit.edu/graphics/classes/6.837/F98/Lecture6/circle.html
 */

static int gf2d_draw_circle_points(SDL_Point *p,Vector2D center, Vector2D point)
{  
  if (point.x == 0)
  {
    vector2d_set(p[0],center.x, center.y + point.y);
    vector2d_set(p[1],center.x, center.y - point.y);
    vector2d_set(p[2],center.x + point.y, center.y);
    vector2d_set(p[3],center.x - point.y, center.y);
    return 4;
  }
  if (point.x == point.y)
  {
    vector2d_set(p[0],center.x + point.x, center.y + point.y);
    vector2d_set(p[1],center.x - point.x, center.y + point.y);
    vector2d_set(p[2],center.x + point.x, center.y - point.y);
    vector2d_set(p[3],center.x - point.x, center.y - point.y);
    return 4;
  }
  if (point.x < point.y)
  {
    vector2d_set(p[0],center.x + point.x, center.y + point.y);
    vector2d_set(p[1],center.x - point.x, center.y + point.y);
    vector2d_set(p[2],center.x + point.x, center.y - point.y);
    vector2d_set(p[3],center.x - point.x, center.y - point.y);
    vector2d_set(p[4],center.x + point.y, center.y + point.x);
    vector2d_set(p[5],center.x - point.y, center.y + point.x);
    vector2d_set(p[6],center.x + point.y, center.y - point.x);
    vector2d_set(p[7],center.x - point.y, center.y - point.x);
    return 8;
  }
  return 0;
}

void gf2d_draw_circle(Vector2D center, int radius, Vector4D color)
{
    SDL_Point *pointArray;
    int i = 0;
    Vector2D point = {0,0};
    int p = (5 - radius*4)/4;
    point.y = radius;
    pointArray = (SDL_Point*)malloc(sizeof(SDL_Point)*radius*8);
    if (!pointArray)
    {
        slog("gf2d_draw_circle: failed to allocate points for circle drawing");
        return;
    }
    i = gf2d_draw_circle_points(&pointArray[i],center, point);
    while (point.x < point.y)
    {
        point.x++;
        if (p < 0)
        {
            p += 2*point.x+1;
        }
        else
        {
            point.y--;
            p += 2*(point.x-point.y)+1;
        }
        i += gf2d_draw_circle_points(&pointArray[i],center, point);
        if (i + 8 >= radius*8)
        {
            slog("too many points for circle");
            break;
        }
    }
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    SDL_RenderDrawPoints(gf2d_graphics_get_renderer(),pointArray,i);
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                            255,
                            255,
                            255,
                            255);
    free(pointArray);
}
void gf2d_font_draw_line(char *text, char *fontname,int size, Vector4D color, Vector2D position)
{
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_Color col = { 255, 255,255};
	SDL_Rect dst = { 0 };
	TTF_Font *font = TTF_OpenFont(fontname, size);
	//slog("%s,%s", text, fontname);
	if (!text)
	{
		slog("cannot draw text, none provided");
		return;
	}
	if (!font)
	{
		slog("cannot draw text, no font provided");
		return;
	}

	surface = TTF_RenderText_Solid(font, text, col);
	if (!surface)
	{
		slog("failed to render text");
		return;
	}
	surface = gf2d_graphics_screen_convert(&surface);
	if (!surface)
	{
		slog("failed to convert text surface to screen format");
		return;
	}

	texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surface);
	if (!texture)
	{
		slog("failed to convert text surface to texture");
		SDL_FreeSurface(surface);
		return;
	}
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_UpdateTexture(texture,
		NULL,
		surface->pixels,
		surface->pitch);
	vector2d_copy(dst, position);
	dst.w = surface->w;
	dst.h = surface->h;
	SDL_RenderCopy(
		gf2d_graphics_get_renderer(),
		texture,
		NULL,
		&dst);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

/*eol@eof*/
