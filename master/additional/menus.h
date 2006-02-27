//to do: from arrays to pointers

//all menus must be assigned an integer name with newMenu() in main().
//menuSetup() must be run once before use

  //maximum 20-character words
struct word{
  char *Ch = new char[20];
  word *next;
};

//int const rowcols=10;
struct menu{
  int rows;
  int cols;
  int wide;
  int tall;
  pixel cor;
  int place;
  //int matrix[rowcols][rowcols]; //maximum 10 rows, 10 columns.
  word *firstword = new word;
  menu *next;
};

/*
  //note: a layout is only intended for the menu which it was created for,
  //and will not function correctly if used on a menu which has a different
  //number of rows or colums than the original.
int const maxlayouts=20;
//word *layoutlist[maxlayouts];
int const maxwords=200;
char *wordList[maxwords];
int layoutList[maxlayouts][2]; // layoutList[i][0] is the first word in label i
                    // layoutList[i][1] is the number of words in label i.
int mln=0, wln=0;
//int nobuttons=1;
 
int const maxmenus=10;
Menu menu[maxmenus];  //stores the data of each individual menu
int menuList[maxmenus]; //stores the area group name of each menu
int menuplace=0;
int activeMenu=0;*/
int boxM=0;
int menus=0;  //total number of menus

menu *activeMenu;

int backMC, foreMC, textMC;

pixel backB, quitB, stopB;

//preliminary declarations-------------------------------------------------

pixel addMenu(char *);
int backM();
int chosen(pixel);
void clearMenu();
void getChoiceM(int);
int gotChoiceM(int);
int getMenu();
pixel lastM(int);
int menuNumber(pixel);
void menuSetup();
int mNumber(int, pixel);
int mplace(int);
int newLayout();
int newMenu(int, int, int, int, int, int);
//int onmenu(int, int, int);
void putMenu(pixel, char *);
void putMenu2(pixel, char *);
int quitM();
void realizeLayout(int);
void resetMenu();
int setMenu();
void setMenuColors(int, int, int);
int stopM();
void writeBackM();
void writeQuitM();
void writeStopM();

//=========================================================================

/*pixel addMenu(){
  pixel thispix=menu[activeMenu].place;
  menu[activeMenu].place.x++;
  if (menu[activeMenu].place.x >= menu[activeMenu].cols){
    menu[activeMenu].place.x=0;
    menu[activeMenu].place.y++;
  };

  return thispix;
  }*/

pixel addMenu(char *label){
  activeMenu->place++;
  if (activeMenu->place >= (activeMenu->cols)*(activeMenu->rows)){
    //! error message
    activeMenu->place=0;
  };
  pixel thispix;
  thispix.y=(activeMenu->place)/(activeMenu->cols);
  thispix.x=(activeMenu->place)%(activeMenu->cols);
  pixel thispix=menu[activeMenu].place;

  word *thisword = activeMenu->firstword;
  while (thisword->next != NULL) thisword = thisword->next;
  word *thatword = new word;
  thatword->next = NULL;
  thatword->Ch = label;
  thisword->next = thatword;
 
  return thispix;
}

int backM(){
  return (mNumber(activeMenu, backB)==boxM);
}

int chosen(pixel chosenpix){ 
  return (mNumber(activeMenu, chosenpix)==boxM);
}

void clearMenu(){
  int m=activeMenu;
  setColor( backMC );
  fillRectangle(pix, menu[m].cor.x, menu[m].cor.y, menu[m].wide, menu[m].tall);

  setColor( foreMC );
  drawAreas( pix, menuList[activeMenu], 0, 0 );
  exit(1);
}

 //automatically activates the menu in question, and leaves it activated
void getChoiceM(int gplace){
  activeMenu=mplace(gplace);
  boxM=getChoice(menuList[activeMenu]);
}

int getMenu(){
  return menuList[activeMenu];
}

  //does not change activeMenu
int gotChoiceM(int gplace){
  int thismenu=mplace(gplace);
  return gotChoice(menuList[thismenu]);
}

  //returns the "tolast" to last pixel of a menu, e.g. the second to last
  //to large of tolasts will pile up at (0, 0)!
pixel lastM(int tolast){
  int mlx=menu[activeMenu].cols-1;
  int mly=menu[activeMenu].rows-1;
  for (int mli=1; mli<tolast; mli++){
    mlx=mlx-1;
    if (mlx < 0){ mlx=menu[activeMenu].cols-1; mly=mly-1; };
    if (mly < 0){ mlx=0; mly=0; mli=tolast; };
  };
  pixel mlpix;
  mlpix.x=mlx; mlpix.y=mly;
  return mlpix;
}

  //usable function, employing the active menu
int menuNumber(pixel thispix){
  return menu[activeMenu].matrix[thispix.x][thispix.y];
}

void menuSetup(){
  //for (int ii=0; ii<maxlayouts; ii++){
    //layoutlist[ii]=new word;
    //layoutlist[ii]->Ch="";
    //layoutlist[ii]->next=NULL;
 // };
}

  //internal function
int mNumber(int m, pixel thispix){
  return menu[m].matrix[thispix.x][thispix.y];
}
  //takes the area group name of a menu and returns the menuplace of that menu
int mplace(int groupname){
  for (int ii=0; ii<maxmenus; ii++){
    if (menuList[ii]==groupname) return ii;
  };
  return -1;
}

/*
int newLayout(){
  menu[activeMenu].place.x=0;
  menu[activeMenu].place.y=0;
  layoutList[mln][0]=wln;
  layoutList[mln][1]=0;
  //layoutlist[mln]=new word;
  mln++;
  return mln-1;
  }*/

int newMenu(int mcorx, int mcory, int mwide, int mtall, int mrows, int mcols){
  menu *newmenu = new menu;
  newmenu->next = NULL;
  if (!menus) firstmenu = newmenu;
  else{
    menu *thismenu = firstmenu;
    while (thismenu->next != NULL) thismenu = thismenu->next;
    thismenu->next = newmenu;
  };
  menus++;

  newmenu->cor.x=mcorx;
  newmenu->cor.y=mcory;
  newmenu->wide=mwide;
  newmenu->tall=mtall;
  newmenu->rows=mrows;
  newmenu->cols=mcols;kkk,

  int aa, bb;
  aa=mwide/mcols;
  bb=mtall/mrows;
  int groupname=startGroup();
  menuList[menuplace]=groupname;
  int ii, jj;
  for (ii=0; ii < mcols; ii++){
    for (jj=0; jj < mrows; jj++){
      menu[menuplace].matrix[ii][jj]=addList( mcorx+(aa*ii), mcory+(bb*jj), aa, bb );
    };
  };
  menu[menuplace].cor.x=mcorx;
  menu[menuplace].cor.y=mcory;
  menu[menuplace].wide=mwide;
  menu[menuplace].tall=mtall;
  menu[menuplace].rows=mrows;
  menu[menuplace].cols=mcols;
  menuplace++;
  return groupname; 
}

/*
int onMenu(int m, int omx, int omy){
  if (areaName(m, omx, omy) > -1) return 1;
  return 0;
}*/

void putMenu(pixel pm1, char *pm2 ){
  setColor(textMC);
  int pmx=activeMenu->cor.x+(pm1.x*activeMenu->wide/activeMenu->cols)+lwidth;
  int pmy=activeMenu->cor.y+(pm1.y*activeMenu->tall/activeMenu->rows)+letterheight;
  drawString( win, pmx, pmy, pm2 );
  drawString( pix, pmx, pmy, pm2 );
}

void putMenu2( pixel pm1, char *pm2, char *pm3 ){
  setColor(textMC);
  int pmx=activeMenu->cor.x + (pm1.x*activeMenu->wide/activeMenu->cols)+lwidth;
  int pmy=activeMenu->cor.y+(pm1.y*activeMenu->tall/activeMenu->rows)+letterheight;
  drawString( win, pmx, pmy, pm2 );
  drawString( win, pmx, pmy+letterheight, pm3);
  drawString( pix, pmx, pmy, pm2 );
  drawString( pix, pmx, pmy+letterheight, pm3);
}
 
int quitM(){
  return (boxM == (activeMenu->cols * quitB.y)+quitB.x);
}

/*
void realizeLayout(int layoutnumber){
  pixel thispix;
  thispix.x=0; thispix.y=0;
  clearMenu();

  int firstword=layoutList[layoutnumber][0];  
  int words=layoutList[layoutnumber][1];
  
  for (int ii=firstword; ii<=firstword+words; ii++){
    putMenu( thispix, wordList[ii]);
    thispix.x++;
    if (thispix.x >= menu[activeMenu].cols){
      thispix.x=0;
      thispix.y++;
    };
  };
  
  /*
  word *word1=layoutlist[layoutnumber];
  while (word1->next != NULL){
    putMenu(thispix, word1->Ch);

    word1=word1->next;
  };
  putMenu(thispix, word1->Ch); 
  */    
}*/

void resetMenu(){
  clearMenu();
  activeMenu->place=0;
}

  //uses the last menu in the list if gplace is an invalid menu name.
void setMenu(int gplace){
  activeMenu=firstMenu;
  while((activeMenu->label != gplace)&&(activeMenu->next != NULL))
    activeMenu=activeMenu->next;
}

void setMenuColors( int pbackMC, int pforeMC, int ptextMC ){
  backMC=pbackMC; foreMC=pforeMC; textMC=ptextMC;
}

int stopM(){
  return (boxM == (activeMenu->cols * stopB.y)+stopB.x);
}

 //note: the menu must have at least 2 columns for backM to be used
void writeBackM(){
  backB.x=activeMenu->cols-2;
  backB.y=activeMenu->rows-1;
  putMenu(backB, "back"); 
}
 
void writeQuitM(){
  quitB.x=activeMenu->cols-1;
  quitB.y=activeMenu->rows-1;
  putMenu(quitB, "quit"); 
}

void writeStopM(){
  stopB.x=activeMenu->cols-1;
  stopB.y=activeMenu->rows-1;
  putMenu(stopB, "stop"); 
}
