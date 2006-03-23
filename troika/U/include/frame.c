/*+
 * frame.c
 *
 * Defines a region of a grid. Useful when "tokenizing" a bitmap, and whenever
 * it's important to isolate or keep track of different elements within a grid.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */



/**
 * frame::frame(int id0)
 *
 *
 */
frame::frame(int id0)
       : item(id0)
{
  #ifdef FRAME_DEBUG
    cout << "<> frame::frame(int)" << endl;
    cout.flush();
  #endif
}



/**
 * frame::frame(universe *u, int x0, int y0, int width0, int height0)
 *
 *
 */
frame::frame(universe *u, int x0, int y0, int width0, int height0)
       : item(u, item_classe),
         frame_x(x0),
         frame_y(y0),
         frame_width(width0),
         frame_height(height0)
{
  #ifdef FRAME_DEBUG
    cout << "<> frame::frame(universe *, int, int, int, int)" << endl;
    cout.flush();
  #endif
}



/**
 * frame::~frame()
 *
 *
 */
frame::~frame()
{
  #ifdef FRAME_DEBUG
    cout << "<> frame::~frame()" << endl;
    cout.flush();
  #endif
}



/**
 * int frame::x()
 *
 * Return the frame's leftmost x coordinate.
 */
int frame::x()
{
  return frame_x;
}



/**
 * int frame::y()
 *
 * Return the frame's uppermost y coordinate.
 */
int frame::y()
{
  return frame_y;
}



/**
 * int frame::width()
 *
 * Return the frame's horizontal dimension.
 */
int frame::width()
{
  return frame_width;
}



/**
 * int frame::height()
 *
 * Return the frame's vertical dimension.
 */
int frame::height()
{
  return frame_height;
}



/**
 * void frame::serialize(ofstream &out)
 *
 *
 */
void frame::serialize(ofstream &out)
{
  #ifdef FRAME_DEBUG
    cout << "<> void frame::serialize(ofstream &)" << endl;
    cout.flush();
  #endif

  out << '\t' << id() << ")\t" << frame_x << ' ' << frame_y << ' ' << frame_width << ' ' << frame_height << '\n';
}



/**
 * void frame::deserialize(ifstream &in)
 *
 *
 */
void frame::deserialize(ifstream &in)
{
  #ifdef FRAME_DEBUG
    cout << "<> void frame::deserialize(ifstream &)" << endl;
    cout.flush();
  #endif

  char *s = new char[10];
  in >> s >> frame_x >> frame_y >> frame_width >> frame_height;
  delete s;
}

/*- end of file --------------------------------------------------------------*/
