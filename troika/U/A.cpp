#include <SDL/SDL.h>

#include "U.h"

#include "include/graphics/paper.h"
#include "include/graphics/paper.c"


#define SCALE 15
#define ROWS 20
#define COLS 20

/*
red(r) green(g) blue(b)
yellow(y) orange(o) violet(v)
black(0) white(1) gray(5)
*/

color *white_c, *black_c, *gray_c;
color *red_c, *green_c, *blue_c;
color *yellow_c, *orange_c, *violet_c;

color *sky_c;

void pre(universe *u)
{
  white_c = new color(u, 0xFF,0xFF,0xFF);
  black_c = new color(u, 0x00,0x00,0x00);
  gray_c = new color(u, 0x7F,0x7F,0x7F);
  red_c = new color(u, 0xFF,0x00,0x00);
  green_c = new color(u, 0x00,0xFF,0x00);
  blue_c = new color(u, 0x00,0x00,0xFF);
  yellow_c = new color(u, 0xFF,0xFF,0x00);
  orange_c = new color(u, 0xFF,0x7F,0x00);
  violet_c = new color(u, 0x7F,0x00,0xFF);

  sky_c = new color(u, 0xE8,0xF4,0xFF);
}

// draw modes
#define COLOR 0
#define BOOLEAN 1
#define SILHOUETTE 2

universe *data_u, *work_u;
char *s;
paper *p, *p2;

void bail(char *out_name)
{
  delete p;
  ofstream out (out_name);
  data_u->serialize(out);
  out.close();
  delete data_u;
  //delete work_u;
  delete s;
  exit(0);
}



int main(int argc, char *argv[])
{
  /* Sundry SDL stuff *********************************************/
  SDL_Surface *screen;
  if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 )
  {
    printf("SDL Oops: %s\n", SDL_GetError());
    exit(1);
  }
  screen=SDL_SetVideoMode(COLS*SCALE,ROWS*SCALE,32,SDL_SWSURFACE|SDL_HWPALETTE) ;
  /* do this now so we can use the surface info to set transparency */
  if ( screen == NULL )
  {
    printf("Unable to set %dx%d video: %s\n", SCALE*COLS, SCALE*ROWS, SDL_GetError());
    exit(1);
  }

  /* Note: this is the ONLY error-checking we do on the arguments... */
  if ((argc < 2)||(argc > 3)) {
    cout << "Error: wrong number of arguments." << endl;
    exit(1);
  }
  ifstream in;
  char *out_name;
  if (argc == 2) {
    data_u = new universe("test_data");
    out_name = argv[1];
  } else {
    in.open(argv[1]);
    data_u = new universe(in);
    in.close();
    out_name = argv[2];
  }

  work_u = data_u;
  //work_u = NULL;
cout << "-1" << endl; cout.flush();
  pre(data_u);

cout << "0" << endl; cout.flush();
  srand(static_cast<unsigned>(time(0)));
cout << "1" << endl; cout.flush();
  p = new paper(work_u, screen, white_c, SCALE, ROWS, COLS);
  p->ECT();

  void (paper::*draw_silhouette)(color *,color *, color *) = &paper::draw_silhouette;
  //void (paper::*exper)(color *,color *, color *) = &paper::draw_silhouette(white_c,sky_c,blue_c);

  s = new char[100];

  list *patterns = new list(data_u);
  list *pattern_names = new list(data_u);
  list *asciis = new list(data_u);

  while(1)
  {
    SDL_Event event;

    while ( SDL_PollEvent(&event) )
    {
      /* quit on SDL_QUIT */
      if ( event.type == SDL_QUIT )
        bail(out_name);

      if ( event.type == SDL_KEYDOWN )
      {
	switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            bail(out_name);
            break;
          case SDLK_SPACE:
            p->ECT();
            break;
          case SDLK_LEFT:
            p->pen_move(0,-1);
            break;
          case SDLK_RIGHT:
            p->pen_move(0,1);
            break;
          case SDLK_UP:
            p->pen_move(-1,0);
            break;
          case SDLK_DOWN:
            p->pen_move(1,0);
            break;
          case 'r':
	    p->pen_color(red_c);
            break;
          case 'g':
            p->pen_color(green_c);
            break;
          case 'b':
            p->pen_color(blue_c);
            break;
          case 'y':
            p->pen_color(yellow_c);
            break;
          case 'o':
            p->pen_color(orange_c);
            break;
          case 'v':
            p->pen_color(violet_c);
            break;
          case '0':
            p->pen_color(black_c);
            break;
          case '1':
            p->pen_color(white_c);
            break;
          case '5':
            p->pen_color(gray_c);
            break;
          case 'c':
            //p->conway(black_c);
            p->draw_color();
            break;
          case '/':
            //p->draw_silhouette(white_c,sky_c,blue_c);
            (p->draw_silhouette)(white_c,sky_c,blue_c);
            break;
          case SDLK_F2:
            //p->flex(violet_c, black_c);
            p->draw_color();
            break;
          case SDLK_F1:
/*
            strcpy(filename_in, pl->random());
            strcpy(filename, "patterns/");
            strcat(filename, filename_in);
            strcat(filename, ".A");
            cout << "attempting to open " << filename << "..." << endl;
            p2 = new paper(screen, SCALE, filename);
            p->embed_center(p2);
            delete p2;
*/

            break;
          case '`':

/*
            if (pl->lookup(filename_in)) {
              strcpy(filename, "patterns/");
              strcat(filename, filename_in);
              strcat(filename, ".A");
              cout << "attempting to open " << filename << "..." << endl;
              p2 = new paper(screen, SCALE, filename);
              p->embed_center(p2);
              delete p2;
            }
*/
            break;
          case 'a':
            cout << "ascii_key = ";
            cin >> s;
            asciis->enqueue((item* ) new ascii(data_u, s));
            break;
          case '=':
            cout << "name of pattern = ";
            cin >> s;
            pattern_names->enqueue((item *) new ascii(data_u, s));
            p->save(patterns);
/*
            cout << "name of pattern = ";
            cin >> filename_in;
            pl->append(filename_in);
            strcpy(filename, "patterns/");
            strcat(filename, filename_in);
            strcat(filename, ".A");
            cout << "attempting to save " << filename << "..." << endl;
            p->save(filename);
*/
            break;
        }

      }
    }
  }

  return 0;
}
