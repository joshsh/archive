#ifndef EXPANDING_STRING_H
#define EXPANDING_STRING_H

/*+
 * expanding_string.h
 *
 * author: Josh Shinavier
 * last edited: 11/18/04
 */


 
class SNPL_expanding_string
{
private:

  void expand();

  int actual_size();
  char *actual_string();
  
protected:

  int string_size, filled;
  double expansion_factor;
  char *expanding_string;
    
public:

  SNPL_expanding_string(int, double);

  ~SNPL_expanding_string();

  void minimize();  
    
  int size();
  
  double expansion();
  
  void clear();
  
  char *copy();
  
  char get(int);
  
  void set(int, char);

  void append(char);
  
  void append(char *);
  
  void append(void *);
  
  void append(class SNPL_expanding_string *);
  
  char *remove(int);
  
  void rewind(int);
  
  //! temporary debugging method
  //void view_bytes();
};



/*- end of file ---------------------------------------------------------------*/

#endif
