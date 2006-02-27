// note: graphics.h must be included before areas.h
// a function "void redraw()" must be declared before inclusion

struct area{
  int x, y, length, height;
};

int place=0, groupplace=0, block=0;
int const listlen=200;
area areaList[listlen];
int groupList[50][2];  // grouplist[i][0] is the first area in group i
                    // groupList[i][1] is the number of areas in group i.
//the name of the first area on the list is 0.

//adds an area to the list
int addList(int adx, int ady, int adlength, int adheight){
  if (!block){ 
    if ((!adlength) || (!adheight)) return -1;
    areaList[place].x = adx;
    areaList[place].y = ady;
    areaList[place].length = adlength;
    areaList[place].height = adheight;
    place++;
    if (place >= listlen) block = 1;  
    groupList[groupplace][1]++;
  }
  else{
    //setColor( 10 );
    //printLine( "areas.h: Out of list space; last area was not added." );
  };
  return place-1;
}

void removeList(){  //deletes an area from the list

}

int areaName(int gplace,int px, int py){ //returns the name of the area at pixel
  int ii, iii, jj;                      //(px,py), -2 if area there isn't named.
  for (ii=0; ii<groupList[gplace][1]; ii++){
    iii=groupList[gplace][0]+ii;
    jj=-2;
    if ((px >= areaList[iii].x) && (px < areaList[iii].x + areaList[iii].length)){
      if((py >=areaList[iii].y) && (py < areaList[iii].y + areaList[iii].height)){
        jj=iii;
        ii=listlen;
      };
    };
  };
  return jj;
}

int getChoice( int gplace ){
  Event gc1;
  int choice=-1;
  while (choice < gplace){
    gc1=getNextEvent();
    while (( gc1.type != ButtonPress)||(gc1.value != 1 )){
      gc1=nextEvent();
      if (gc1.type == Expose) redraw();
    };
    choice=areaName( gplace, gc1.x, gc1.y );
  };
  return choice;
}

int gotChoice( int gplace ){
  Event gc1;
  int choice=-1;
  while ( checkEvent() ){
    gc1 = nextEvent();
    if (gc1.type == Expose) redraw();
    if (( gc1.type == ButtonPress)&&(gc1.value == 1 )){
      choice = areaName( gplace, gc1.x, gc1.y );
      if (choice >= gplace) return choice;
    };
  };
  return choice;
}

int startGroup(){
  int ii;
  groupplace++;
  for (ii=0; ii < groupplace; ii++)
    groupList[groupplace][0]=groupList[groupplace][0] + groupList[ii][1];
  return groupplace;
}

void drawAreas(Drawable dda, int gplace, int xoffset, int yoffset){
  int ii, iii;
  area da;
  for (ii=0; ii < groupList[gplace][1]; ii++){
    iii=groupList[gplace][0]+ii;
    da=areaList[iii];
    drawRectangle(dda, da.x+xoffset, da.y+yoffset, da.length, da.height);
  };
}

