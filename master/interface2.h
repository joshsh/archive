#include "graphics.h"

struct pixel{
  int x, y;
};

Window win;
Pixmap pix;

int winwide, wintall, xcor, ycor;
int letterheight=12;

struct area{
  int x, y, length, height;
};

int place=0, groupplace=0;
int const listlen=200;
area areaList[listlen];
int groupList[50][2];  

int addList(int adx, int ady, int adlength, int adheight){ 
  if ((!adlength) || (!adheight)) return -1;
  areaList[place].x = adx;
  areaList[place].y = ady;
  areaList[place].length = adlength;
  areaList[place].height = adheight;
  place++;
  groupList[groupplace][1]++;
  return place-1;
}

int startGroup(){
  int ii;
  groupplace++;
  for (ii=0; ii < groupplace; ii++)
    groupList[groupplace][0]=groupList[groupplace][0] + groupList[ii][1];
  return groupplace;
}

int lwidth=6;

int const rowcols=10;
struct Menu{
  int rows;
  int cols;
  int wide;
  int tall;
  pixel cor;
  pixel place;
  int matrix[rowcols][rowcols]; //maximum 10 rows, 10 columns.
};

struct word{
  char *Ch;
  word *next;
};

int const maxlayouts=10;
word *layoutlist[maxlayouts];
int mln=0;
 
int const maxmenus=10;
Menu menu[maxmenus];  //stores the data of each individual menu
int menuList[maxmenus]; //stores the area group name of each menu
int menuplace=0;
int activeMenu=0;
int mbox=0;

pixel addMenu(char *);
int menuNumber(pixel);
void menuSetup();
int mNumber(int, pixel);
int mplace(int);
int newLayout();
int newMenu(int, int, int, int, int, int);
int setMenu();

//=========================================================================

pixel addMenu(char *label){
  pixel thispix=menu[activeMenu].place;
  menu[activeMenu].place.x++;
  if (menu[activeMenu].place.x >= menu[activeMenu].cols){
    menu[activeMenu].place.x=0;
    menu[activeMenu].place.y++;
  };

  word *addlabel=new word, *thisword=layoutlist[mln-1];
  addlabel->Ch=label;
  while (thisword->next != NULL) thisword=thisword->next;
  thisword->next=addlabel;
  //exit(1);
  return thispix;
}


  //usable function, employing the active menu
int menuNumber(pixel thispix){
  return menu[activeMenu].matrix[thispix.x][thispix.y];
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

int newLayout(){
  menu[activeMenu].place.x=0;
  menu[activeMenu].place.y=0;
  layoutlist[mln]=new word;
  mln++;
  return mln-1;
}

int newMenu(int mcorx, int mcory, int mwide, int mtall, int mrows, int mcols){
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

void setMenu(int gplace){
  activeMenu=mplace(gplace);
}
