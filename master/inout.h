//note: graphics.h and strings.h must be included before inout.h
//a function redraw() must be declared before inclusion of inout.h

int disdigits = 6; //decimal places displayed by printNumber(...);
int xcursor=0, ycursor=0;
int textxmin=0, textxmax=2000, textymin=0, textymax=2000;
int textxminl, textxmaxl, textyminl, textymaxl;
int lwidth=6, lheight=10;
int xcor=0, ycor=0, xcorl, ycorl;
int maxrows, maxcols;
int clearC=0;
Window currentWindow;
Pixmap currentPix;
int xchars, ychars;  //the size in characters of the text window.
                    //must be determined with the window.
int xcharsl, ycharsl;
int inoutC=40; //changeable with textCol(...);

void locate( int locx, int locy ){
  int xcursorl=xcursor, ycursorl=ycursor;
  xcursor=xcor+(locx*lwidth);
  ycursor=ycor+((locy+1)*lheight);
  if (xcursor >= textxmax) xcursor=xcursorl;
  if (ycursor >= textymax) ycursor=ycursorl;
}

void clearTextWindow(){
  int lastC=getColor();
  setColor( clearC );
  fillRectangle( currentWindow, textxmin, textymin, textxmax-textxmin, textymax-textymin);
  fillRectangle( currentPix, textxmin, textymin, textxmax-textxmin, textymax-textymin);
  setColor( lastC );
}

void resetWindow(){
  clearTextWindow();
  locate(0, 0);
}

void advance( int ad1 ){
  xcursor=xcursor+(lwidth*ad1);
  if ( xcursor >= textxmax ){
    ycursor=ycursor+lheight;
    xcursor=xcor;
  };
  if ( ycursor >= textymax ){
    clearTextWindow();
    locate( 0, 0 );
  };
}

void newLine(){
  ycursor=ycursor+lheight;
  xcursor=xcor;
  if (ycursor >= textymax){
    clearTextWindow();
    locate( 0, 0 );
  }; 
}

void printChar( char pc1 ){ 
  drawString( currentWindow, xcursor, ycursor, pc1 );
  drawString( currentPix, xcursor, ycursor, pc1 );
  advance(1);
}

void printString( char *psstring ){
  int lastC=getColor();
  setColor( inoutC );
  drawString( currentWindow, xcursor, ycursor, psstring );
  drawString( currentPix, xcursor, ycursor, psstring );
  advance( strlen( psstring ) );
  setColor(lastC);
}

void blank( int blength ){
  int lastC = inoutC;
  inoutC = clearC;
  int ii;
  for (ii=0; ii < blength; ii++) printString( "#" );  //find a filled character
  inoutC=lastC;
}

void printNumber( double number ){
  int lastC=getColor();
  setColor( inoutC );
  if (number == 0) printString( "0" );
  if (number < 0){
    printString( "-" );
    number=-number;
  };
  int intnumber, count=0;
  double number2=number;
  while (number2 > 1){
    number2=number2/10;
    count++;
  };
  intnumber=number;
  if ((count == 0)&&(intnumber != number)) printString( "." );
  while (number2 > 0){
    number2=number2*10;
    intnumber=number2;
    printString( intStr( intnumber ) );
    number2=number2-intnumber;
    count=count-1;
    if (count == 0) printString( "." );
    if (count < -disdigits) number2=0;
  };
  setColor( lastC );
}

void drawNumber( Drawable d, int x, int y, double dn1){
  int xlast=xcursor;
  int ylast=ycursor;
  xcursor=x;
  ycursor=y;
  printNumber( dn1 );
  xcursor=xlast;
  ycursor=ylast;
}

void setLimits( int slxmin, int slxmax, int slymin, int slymax ){
  textxmin=slxmin;
  textxmax=slxmax;
  textymin=slymin;
  textymax=slymax;
  maxrows=fabs((textymax-textymin)/lheight);
  maxcols=fabs((textxmax-textxmin)/lwidth);
  xcor=textxmin;
  ycor=textymin;
  xchars=(slxmax-slxmin)/lwidth;
  ychars=(slymax-slymin)/lheight;
}

void saveLimits(){ 
  textxminl=textxmin;
  textxmaxl=textxmax;
  textyminl=textymin;
  textymaxl=textymax;
  xcorl=xcor;
  ycorl=ycor;
  xcharsl=xchars;
  ycharsl=ychars;
}

void resetLimits(){
  textxmin=textxminl;
  textxmax=textxmaxl;
  textymin=textyminl;
  textymax=textymaxl;
  maxrows=fabs((textymax-textymin)/lheight);
  maxcols=fabs((textxmax-textxmin)/lwidth);
  xcor=xcorl;
  ycor=ycorl;
  xchars=xcharsl;
  ychars=ycharsl;
}

//setCorner() should be used AFTER setLimits().
void setCorner( int scx, int scy ){
  if ((scx < textxmax)&&(scx > textxmin )) xcor=scx;
  if ((scy < textymax)&&(scy > textymin )) ycor=scy;
}

void resetTop( int rt1 ){
  int lastC = getColor();
  setColor( clearC );
  fillRectangle( currentWindow, textxmin, textymin, textxmax-textxmin, lheight*rt1);
  fillRectangle( currentPix, textxmin, textymin, textxmax-textxmin, lheight*rt1);
  locate( 0, 0 );
  setColor( lastC );
}

void printLine( char *pl1 ){
  printString( pl1 );
  newLine();
}

void setWindow( Window swd, Pixmap swp ){
  currentWindow = swd;
  currentPix = swp;
}

void setClearColor( int scc1 ){
  clearC = scc1;
}

void setTextColor( int ncol ){
  inoutC = ncol;
};

double inputNumber( char *instring ){
  int lastC = getColor();
  setColor(inoutC);
  int Return = 0;
  int sign=1;
  int afterpoint = 0;
  double outnumber=0;
  printString( instring );
  Event inev=getNextEvent();
  while ( !Return ){
    if (( inev.type == KeyPress )&&( inev.value == 45 )){
      sign = -sign;
      printString( "-" );
    };
    if (( inev.type == KeyPress )&&( isNumber( inev.value ))){
      if ( inev.value == 46 ){
        afterpoint=1;
        printString( "." );
      }
      else{
        printString( intStr( inev.value-48 ) );
        if (afterpoint == 0){
          outnumber=outnumber*10;
          outnumber=outnumber + (inev.value - 48);
        };
        if (afterpoint > 0){
          outnumber=outnumber + ( (inev.value - 48)*tenpow(-afterpoint) );
          afterpoint++;
        };
      };
    };
    if (( inev.type == KeyPress )&&( inev.value == 13 )) Return = 1;
    inev = nextEvent();
    if (inev.type == Expose) redraw();
  };
  newLine();
  outnumber=sign*outnumber;
  setColor(lastC);
  return outnumber;
}

char inputChar( char *instring ){
  int lastC=getColor();
  printString( instring );
  Event inev;
  while ( checkEvent() ){ 
    inev=nextEvent(); 
    if (inev.type == Expose) redraw();
  };
  inev=nextEvent();
  if (inev.type == Expose) redraw();
  while((inev.type != KeyPress)||(inev.value < 97)||(inev.value > 122)){
    if (inev.type == Expose) redraw();
    inev=nextEvent();
  };
  if (inev.type == Expose) redraw();
  printLine( charStr( char(inev.value) ) );
  setColor( lastC );
  return char( inev.value );
}

