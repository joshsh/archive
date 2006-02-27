//3D maze editor (for now 6x6x6)

#include <fstream.h>
#include "graphics.h"

struct corner{    //x, y, and z are binary, expressing whether there is a link
  int x, y, z;    //in the positive x, y, or z direction, respectively.
  int type;       //type is for color coding the corners, to keep track of
  char *label;    //their properties.
};

int const mlen=6;
corner maze[mlen][mlen][mlen];  //x, y, z

int x, y, z;
int passlen;
int rad=10, cross=rad/1.4142135 + 1;
int tx, ty, ttx, tty;
int north=1, east=2, south=3, west=4, up=5, down=6;
int const xpix=500, ypix=500;
int const menuheight=20;
int lwidth=6, lheight=10;

int backC=40, lineC=80, cornerC=lineC+1, selectC=1;
int neutral=lineC, lit=2;

Window win;
Pixmap pix;

void redraw(){
  copyArea(pix, win, 0, 0, xpix, ypix+menuheight, 0, 0);
}

//strings===================================================================

void centerString(Drawable cdsD, int cdsxn, int cdsxx, int cdsyn, int cdsyx, char *ChX){
  drawString(cdsD, ((cdsxn+cdsxx)/2)-(lwidth*strlen(ChX)/2), ((cdsyn+cdsyx)/2)-(lheight/2), ChX);
}

char *inputStr(int isx, int isy){
  char *Ch1=new char, *Ch=Ch1, *ChA=new char, *ChB=ChA;
  ChB++;
  *ChB='\0';
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
      *ChA=char(inev.value);
      drawString(win, isx, isy, ChA );
      drawString(pix, isx, isy, ChA );
      isx=isx+lwidth;
      inev=nextEvent();
    }else stop=1;
  };
  Ch++;
  *Ch='\0';
  return Ch1;
}

//maze functions============================================================

void southLine(){
  drawLine(pix, (x+1)*passlen, (y+1)*passlen, (x+1)*passlen, (y+2)*passlen);
}

void eastLine(){
  drawLine(pix, (x+1)*passlen, (y+1)*passlen, (x+2)*passlen, (y+1)*passlen); 
}

void tunnelUp(int cornerC){
  setColor(backC);
  fillCircle(pix, (x+1)*passlen, (y+1)*passlen, rad);
  setColor(cornerC);
  drawCircle(pix, (x+1)*passlen, (y+1)*passlen, rad);
  drawPoint(pix, (x+1)*passlen, (y+1)*passlen);
}

void tunnelDown(int cornerC){
  setColor(backC);
  fillCircle(pix, (x+1)*passlen, (y+1)*passlen, rad);
  setColor(cornerC);
  drawCircle(pix, (x+1)*passlen, (y+1)*passlen, rad);
  int p=(x+1)*passlen;
  int q=(y+1)*passlen;
  drawLine(pix, p-cross, q+cross, p+cross, q-cross);
  drawLine(pix, p-cross, q-cross, p+cross, q+cross);
}

void tunnelBoth(int cornerC){
  setColor(backC);
  fillCircle(pix, (x+1)*passlen, (y+1)*passlen, rad);
  setColor(cornerC);
  drawCircle(pix, (x+1)*passlen, (y+1)*passlen, rad);
}

void drawLevel(int level){
  setColor(backC);
  fillRectangle(pix, 0, 0, xpix, ypix);
  passlen=xpix/(mlen+1);
  z=level; 
  corner thiscor, upcor, downcor;
  int istunnel; 
  for (x=0; x<mlen; x++){
    for (y=0; y<mlen; y++){
      thiscor=maze[x][y][z];
      if (z>0) upcor=maze[x][y][z-1];
      if (z<mlen-1) downcor=maze[x][y][z+1];
      setColor(lineC);
      if (thiscor.x) eastLine();
      if (thiscor.y) southLine();
      istunnel=0;
      setColor(thiscor.type);
      if ((z==0)&&(thiscor.z)){ istunnel=1; tunnelDown(thiscor.type);};
      if ((z==mlen-1)&&(upcor.z)){ istunnel=1; tunnelUp(thiscor.type);};
      if ((z != 0)&&(z<mlen-1)){
        if (thiscor.z){
          istunnel=1;
          if (upcor.z) tunnelBoth(thiscor.type); else tunnelDown(thiscor.type);
        }else{
          if (upcor.z){
            istunnel=1;
            tunnelUp(thiscor.type);
          };
        };
      };
      if ((!istunnel)&&(thiscor.type != neutral)) fillCircle(pix, (x+1)*passlen, (y+1)*passlen, rad/2);
      if (strlen(thiscor.label) > 0) drawString(pix, (x+1)*passlen+rad, (y+1)*passlen-rad, thiscor.label);
    };
  };
  redraw();
}

int selectCorner(){
  Event e;
  while (checkEvent()){
    e=nextEvent();
    if(e.type == Expose) redraw();
  };
  e=nextEvent();
  while (e.type != ButtonPress){
    e=nextEvent();
    if (e.type == Expose) redraw();
  };
  if (e.y > ypix){
    if (e.x < xpix/2) return 4; else return 5;  //4="edit label", 5="end"
  };
  tx=(e.x-(passlen/2))/passlen;
  ty=(e.y-(passlen/2))/passlen;
  if (tx<0) tx=0;
  if (ty<0) ty=0;
  if (tx >= mlen) tx=mlen-1;
  if (ty >= mlen) ty=mlen-1;
  //cout << "<" << e.type << ">";
  return e.value;
}

int whichDirect(){
  int diff1=tx-ttx, diff2=ty-tty;
  if ((diff1 != 0) && (diff2 != 0)) return 0;
  if ((diff1 > 1)||(diff1 < -1)) return 0;
  if ((diff2 > 1)||(diff2 < -1)) return 0;
  if (diff1 == 1) return east;
  if (diff1 == -1) return west;
  if (diff2 == 1) return south;
  if (diff2 == -1) return north;
  return 0;
}

int inbounds(int ix, int iy, int iz){
  if((ix < 0)||(ix >= mlen)) return 0;
  if((iy < 0)||(iy >= mlen)) return 0;
  if((iz < 0)||(iz >= mlen)) return 0;
  return 1;
}

  //flips a binary integer directly
void flip(int *toflip){
  if (!*toflip) *toflip=1; else *toflip=0;
}

  //trades the value of writeto from val1 to val2 or vice versa
  //assumes that writeto is originally either one or the other
void trade(int *writeto, int val1, int val2){
  if (*writeto == val1) *writeto=val2; else *writeto=val1;
}

void changeMaze(int thisx, int thisy, int thisz, int direction){
  if (!inbounds(thisx, thisy, thisz)) return;
  switch (direction){
    case 5: 
      if (!thisz) return;
      flip(&maze[thisx][thisy][thisz-1].z);
      break;
    case 6:
      if (thisz >= mlen) return;
      flip(&maze[thisx][thisy][thisz].z);
      break; 
    case 1:
      if(!thisy) return;
      flip(&maze[thisx][thisy-1][thisz].y);
      break;
    case 2:
      if(thisx == mlen-1) return;
      flip(&maze[thisx][thisy][thisz].x);
      break;
    case 3:
      if(thisy == mlen-1) return;
      flip(&maze[thisx][thisy][thisz].y);
      break;
    case 4:
      if(!thisx) return;
      flip(&maze[thisx-1][thisy][thisz].x);
      break;
    default: return;
  };
}

void highlight(int hx, int hy){
  int p, q;
  setColor(selectC);
  p=(hx+1)*passlen;
  q=(hy+1)*passlen;
  drawRectangle(pix, p-rad, q-rad, 2*rad, 2*rad);
  drawRectangle(win, p-rad, q-rad, 2*rad, 2*rad);  
}

void editByLevel(){
  int level=0, thisdirect;
  drawLevel(level);
  int selected=0, mouseclick;
  ttx=0; tty=0;
  int stop=0;
  while (!stop){
    mouseclick=selectCorner();
    if (selected){
      if (mouseclick == 1){
        thisdirect=whichDirect();
        if (!thisdirect){
          if ((tx == ttx)&&(ty == tty)) trade(&maze[tx][ty][level].type, neutral, lit);
          selected=0;
          drawLevel(level);
        }else{
          changeMaze(ttx, tty, level, thisdirect);
          drawLevel(level);
          highlight(ttx, tty); 
        };
      };
      if ((mouseclick == 2)&&(level < mlen-1)){
        changeMaze(ttx, tty, level, down);
        drawLevel(level);
        highlight(ttx, tty);
      };
      if ((mouseclick == 3)&&(level > 0)){
        changeMaze(ttx, tty, level, up);
        drawLevel(level);
        highlight(ttx, tty);
      };
      if (mouseclick == 4){
        maze[tx][ty][level].label="";
        drawLevel(level);
        maze[tx][ty][level].label=inputStr((tx+1)*passlen+rad, (ty+1)*passlen-rad);
      };
      if (mouseclick == 5) stop=1;  
    }else{
      if (mouseclick == 1){
        selected=1;
        ttx=tx; tty=ty;
        highlight(tx, ty);
      };
      if ((mouseclick == 2)&&(level < mlen-1)){ level=level+1; drawLevel(level);};
      if ((mouseclick == 3)&&(level > 0)){ level=level-1; drawLevel(level);};
      if (mouseclick == 5) stop=1;
    };
  };
}

void emptyMaze(){
  for(x=0; x<mlen; x++){
    for(y=0; y<mlen; y++){
      for(z=0; z<mlen; z++){
        maze[x][y][z].x=0; maze[x][y][z].y=0; maze[x][y][z].z=0;
        maze[x][y][z].type=neutral;
        maze[x][y][z].label="";
      };
    };
  };
}

void saveMaze(char *destname){
  char *ChY;
  fstream file1;
  file1.open(destname, ios::out);
    //first three characters code for the dimensions of the maze
  file1.put(mlen); file1.put(mlen); file1.put(mlen);
  for(z=0; z<mlen; z++){
    for(y=0; y<mlen; y++){
      for(x=0; x<mlen; x++){
        file1.put(maze[x][y][z].x + 48);
        file1.put(maze[x][y][z].y + 48);
        file1.put(maze[x][y][z].z + 48);
        file1.put(maze[x][y][z].type);
        ChY=maze[x][y][z].label;
        while (*ChY != '\0'){ file1.put(*ChY); ChY++;};
        file1.put('_');  //thus, '_' is not allowed in the labels!
      };
      file1.put(10);  //just to keep the file "viewable"
    };
    if (z < mlen-1) file1.put(10);
  };  
}

void loadMaze(char *mazefile){
  char *ChX=new char, *ChY;
  fstream file1;
  file1.open(mazefile, ios::in);
  //emptyMaze();
    //(since the dimensions are not variable yet)
  char ch=file1.get(); ch=file1.get(); ch=file1.get();
  for(z=0; z<mlen; z++){
    for(y=0; y<mlen; y++){
      for(x=0; x<mlen; x++){
        maze[x][y][z].x=file1.get()-48;
        maze[x][y][z].y=file1.get()-48;
        maze[x][y][z].z=file1.get()-48;
        maze[x][y][z].type=file1.get();
        ch=file1.get();
        ChY=ChX;
        while (ch != '_'){ *ChY=ch; ch=file1.get(); ChY++;};
        *ChY='\0';
        maze[x][y][z].label=ChX;
      };
      ch=file1.get();
    };
    if (z < mlen-1) ch=file1.get();
  };  
}

  //make this into a real function!
int isMazeFile(char *mazename){
  fstream file1;
  file1.open(mazename, ios::in);
  char ch=file1.get(); ch=file1.get(); ch=file1.get(); ch=file1.get();

  if (!file1.eof()) return 1;
  return 0;
}

int main(){
  win=createWindow(20, 20, xpix, ypix+menuheight, "maze editor");
  pix=createPixmap(win, xpix, ypix+menuheight);

  setColor(backC);  
  fillRectangle(pix, 0, 0, xpix, ypix+menuheight);
  setColor(selectC);
  centerString(pix, xpix, xpix+menuheight, 0, ypix, "Edit label");
  
  //emptyMaze();
  
  if(isMazeFile("tre.maze")) loadMaze("tre.maze"); else emptyMaze();
  
  editByLevel();
  
  saveMaze("tre.maze");
  
}
