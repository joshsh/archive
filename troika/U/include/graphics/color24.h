/*+
 * color24.h
 *
 * A helper class (not user-accessible) to store and compare color values for
 * SDL graphics.
 *
 * author: Josh Shinavier
 * last edited: 6/6/04
 */

class color24
{
protected:

  // ba ^ "unsigned char"
  unsigned char color_red, color_green, color_blue;

public:

  color24();

  color24(char *);

  color24(int, int, int);

  ~color24();

  int red(), green(), blue();

  bool equals(color24 *);

  void decode(char *);

  char *encode();

};

/*- end of file --------------------------------------------------------------*/
