//note: graphics.h and char.h must be included before page.h
//note: no more color conservation!  this will just have to be done manually
//a function redraw() must be declared before inclusion of page.h
//all pages are colored according to the same scheme, set with setPageColors()

int disdigits = 6; //decimal places displayed by printNumber(...);
int lwidth=6, lheight=10;
//int maxrows, maxcols;
int backPC=0, textPC;
//int xchars, ychars;  //the size in characters of the text window.
                    //must be determined with the window.
//int xcharsl, ycharsl;

struct Page{
  Window win;
  Pixmap pix;
  int xmin, xmax, ymin, ymax;
  int x, y;  //cursor position
  int p, q;  //corner coordinates
};

int const maxpages=5;
Page page[maxpages];  //the page list. Maximum 5 pages.
int cp, Cp=0;  //index number of the current Page and the next Page

//preliminary declarations------------------------------------------------

 void setLimits(int, int, int, int);
 //void saveLimits();
 //void resetLimits();
 //void setCorner(int, int);

 void clearPage();
 int getPage(); 
 int initPage(Window, Pixmap, int, int, int, int);
 void resetPage();
 void resetTop(int);
 void setPage(int);
 void setPageColors(int, int);
 
 void advance(int);
 void locate(int, int);
 void newLine();
 
 char inputChar(char *);
 int inputInt(char *);
 double inputNumber(char *);
 char *inputStr(char *);
 
 void blank(int);
 void drawnumber(Drawable, int, int, double);
 void printChar(char);
 void printInt(int);
 void printLine(char *);
 void printNumber(double);
 void printString(char *);
 

//------------------------------------------------------------------------

void setLimits(int slxmin, int slxmax, int slymin, int slymax ){
  page[cp].xmin=slxmin;
  page[cp].xmax=slxmax;
  page[cp].ymin=slymin;
  page[cp].ymax=slymax;
  //maxrows=fabs((page[cp].ymax-page[cp].ymin)/lheight);
  //maxcols=fabs((page[cp].xmax-page[cp].xmin)/lwidth);
  page[cp].p=page[cp].xmin+lwidth;
  page[cp].q=page[cp].ymin+lheight;
  //xchars=(slxmax-slxmin)/lwidth;
  //ychars=(slymax-slymin)/lheight;
}

/*
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

  setCorner() should be used AFTER setLimits().
void setCorner( int scx, int scy ){
  if ((scx < textxmax)&&(scx > textxmin )) xcor=scx;
  if ((scy < textymax)&&(scy > textymin )) ycor=scy;
}*/

//external functions#######################################################

void clearPage(){
  setColor( backPC );
  int textxmin=page[cp].xmin, textxmax=page[cp].xmax;
  int textymin=page[cp].ymin, textymax=page[cp].ymax;
  fillRectangle( page[cp].win, textxmin, textymin, textxmax-textxmin, textymax-textymin);
  fillRectangle( page[cp].pix, textxmin, textymin, textxmax-textxmin, textymax-textymin);
}

int getPage(){
 return cp;
}

int initPage(Window swd, Pixmap swp, int slxmin, int slxmax, int slymin, int slymax){
  cp=Cp;
  Cp++;
  page[cp].win = swd;
  page[cp].pix = swp;
  setLimits(slxmin, slxmax, slymin, slymax);
  return cp;
}

void resetPage(){
  clearPage();
  locate( 0, 0 );
}

void resetTop( int rt1 ){
  setColor( backPC );
  int textxmin=page[cp].xmin, textxmax=page[cp].xmax;
  int textymin=page[cp].ymin, textymax=page[cp].ymax;
  fillRectangle( page[cp].win, textxmin, textymin, textxmax-textxmin, lheight*rt1);
  fillRectangle( page[cp].pix, textxmin, textymin, textxmax-textxmin, lheight*rt1);
  locate( 0, 0 );
}

void setPage( int pcp ){
  if (pcp<Cp) cp=pcp;
}

void setPageColors( int scci, int ncol ){
  backPC=scci;
  textPC = ncol;
}

//cursor location==========================================================

void advance( int ad1 ){
  page[cp].x=page[cp].x+(lwidth*ad1);
  if ( page[cp].x >= page[cp].xmax ){
    page[cp].y=page[cp].y+lheight;
    page[cp].x=page[cp].p;
  };
  if ( page[cp].y >= page[cp].ymax ){
    clearPage();
    locate( 0, 0 );
  };
}

void locate( int locx, int locy ){
  int xcursorl=page[cp].x, ycursorl=page[cp].y;
  page[cp].x=page[cp].p+(locx*lwidth);
  page[cp].y=page[cp].q+((locy+1)*lheight);
  if (page[cp].x >= page[cp].xmax) page[cp].x=xcursorl;
  if (page[cp].y >= page[cp].ymax) page[cp].y=ycursorl;
}

void newLine(){
  page[cp].y=page[cp].y+lheight;
  page[cp].x=page[cp].p;
  if (page[cp].y >= page[cp].ymax){
    clearPage();
    locate( 0, 0 );
  }; 
}

//input functions==========================================================

char inputChar( char *instring ){
  printString( instring );
  Event inev;
  while ( checkEvent() ){ 
    inev=nextEvent(); 
    if (inev.type == Expose) redraw();
  };
  inev=nextEvent();
  if (inev.type == Expose) redraw();
  while (inev.type != KeyPress){
    if (inev.type == Expose) redraw();
    inev=nextEvent();
  };
  if (inev.type == Expose) redraw();
  printLine( charStr(inev.value));
  return inev.value;
}

int inputInt( char *instring ){
  char *numstring=inputStr(instring);
  while (!isNumStr(numstring)){
    printLine("inproper number");
    numstring=inputStr(instring);
  };
  
  return strInt(numstring);
}

double inputNumber( char *instring ){
  char *numstring=inputStr(instring);
  while (!isNumStr(numstring)){
    printLine("inproper number");
    numstring=inputStr(instring);
  };
  
  cout << strNum(numstring); //!
  return strNum(numstring);
}

char *inputStr( char *instring ){
  char *Ch1=new char, *Ch=Ch1;
  printString( instring );
  Event inev;
  while ( checkEvent() ){ 
    inev=nextEvent(); 
    if (inev.type == Expose) redraw();
  };
  inev=nextEvent();
  int stop=0;
  if (inev.type == Expose) redraw();
  while (!stop){
    while(inev.type != KeyPress){
      if (inev.type == Expose) redraw();
      inev=nextEvent();
    };
    if (inev.value != 13){
      *Ch=inev.value;
      Ch++;
      printString( charStr(inev.value));
      inev=nextEvent();
    }else stop=1;
  };
  Ch++;
  *Ch='\0';
  return Ch1;
}

//output functions========================================================

void blank( int blength ){
  int lastC = textPC;
  textPC = backPC;
  int ii;
  for (ii=0; ii < blength; ii++) printString( "#" );
  textPC=lastC;
}

void drawNumber( Drawable d, int dnx, int dny, double dn1){
  int xlast=page[cp].x;
  int ylast=page[cp].y;
  page[cp].x=dnx;
  page[cp].y=dny;
  printNumber( dn1 );
  page[cp].x=xlast;
  page[cp].y=ylast;
}

void printChar( char pc1 ){
  char *Ch=new char;
  *Ch=pc1;
  setColor(textPC); 
  drawString( page[cp].win, page[cp].x, page[cp].y, Ch );
  drawString( page[cp].pix, page[cp].x, page[cp].y, Ch );
  advance(1);
}

void printInt( int number ){
  if (!number) printString("0");
  char *ChA=new char;
  char *Ch=ChA;
  int power=0, tens=10;
  int boredom;
  if (number < 0){ *Ch='-'; Ch++; number=-number; };
  while (number >= tens){ tens=tens*10; power++; };
  tens=tens/10;
  for (int ii=0; ii<=power; ii++){
    boredom=number/tens;
    *Ch=boredom+48;
    Ch++;
    number=number-(boredom*tens);
    tens=tens/10;
  };
  *Ch='\0';
  printString(ChA);
}

void printLine( char *pl1 ){
  printString( pl1 );
  newLine();
}

void printNumber( double number ){
  int intnum=int(number);
  printInt(intnum);
  if(intnum == number) return;
  int tens=1;
  for (int ii=0; ii<disdigits; ii++) tens=tens*10;
  printString(".");
  printInt(int((number-intnum)*tens));
}

void printString( char *psstring ){
  setColor( textPC );
  drawString( page[cp].win, page[cp].x, page[cp].y, psstring );
  drawString( page[cp].pix, page[cp].x, page[cp].y, psstring );
  advance( strlen( psstring ) );
}