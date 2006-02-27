/note: disdigits must be declared and set before the use of controlBar()

int cbthick=40;
int theight=20;

  //disappears after use, restoring what it covers
double controlBar(char *title, double lowval, double highval, int cbx, int cby, int length){
  Pixmap temppix=createPixmap(win, length, cbthick);
  copyArea(win, temppix, cbx, cby, length, cbthick, 0, 0);

  //int height=theight*4;
  int lastC = getColor(); 
  //int cbthick=theight*2;
  int numlen=lwidth*(disdigits+topten(highval));
  int numplace=cbx+((length+numlen)/2)+5;
  int lineY=(theight*3)+cby;
  int barplacelast;

  frameRectangle(pix, cbx, cby, length, cbthick, backC, partC);
  drawLine(win, cbx, cby+theight, cbx+length, theight);
  drawRectangle(pix, cbx+length-theight, cby, theight, theight);
  setColor( textC );
  drawString(pix, cbx+5, cby+letterheight, title);
  drawString(pix, cbx+length-theight+5, cby+letterheight, "Done" );
  drawNumber(pix, cbx+5, cby+theight+letterheight, lowval);  //!
  drawNumber(pix, cbx+length-(2*numlen)-15, cby+theight+letterheight, highval);

  double presentval;
  int barplace=10+x;

  setColor(backC);
  fillRectangle(pix, cbx+numplace-5, cby+theight+2, numlen+10, theight-3);
  fillRectangle(pix, cbx+1, cby+theight+2, length-3, cbthick-3);
  setColor(foreC);
  presentval=lowval+((barplace-cbx-10)*(highval-lowval)/(length-20));
  drawNumber(pix, numplace, cby+theight+letterheight, presentval);
  setLineWidth(2);
  setColor(partC);
  drawLine(pix, cbx+5, lineY, cbx+length-5, lineY);
  setLineWidth(3);
  setColor(foreC);
  drawLine(pix, barplace-1, lineY-10, barplace-1, lineY+10);

  Event e;
  //while (checkEvent()) e=nextEvent();
  //e=nextEvent();
  updateEvents();
  e=nextEvent();
  if (e.type == Expose) redraw();
  int done=0, stop;
  while (!done){
    if ((e.type == ButtonPress)&&(e.value == 1)){
       stop=0;
      if((e.y > cby)&&(e.y < cby+theight)&&(e.x > cbx+length-theight)&&(e.x < cbx+length)){ stop=1; done=1;};
       while (!stop){
         if (fabs(e.y-lineY) <= 10){
           barplacelast=barplace;
           barplace=e.x;
           setColor(backC);
           fillRectangle(win, numplace-5, cby+theight+2, numlen+10, theight-3);
           fillRectangle(win, 1, theight+2, length-3, cbthick-3);
           setColor(foreC);
           presentval=lowval+((barplace-cbx-10)*(highval-lowval)/(length-20));
           drawNumber(win, numplace, cby+theight+letterheight, presentval);
           setLineWidth(3);
           setColor(backC);
           drawLine(win, barplacelast-1, lineY-10, barplacelast-1, lineY+10);
           setColor(foreC);
           drawLine(win, barplace-1, lineY-10, barplace-1, lineY+10);
           setLineWidth(2);
           setColor(partC);
           drawLine(win, cbx+5, lineY, cbx+length-5, lineY);
         };
         if (checkEvent()) e=nextEvent();
         if (e.type == Expose) redraw(); 
         if (e.type == ButtonRelease) stop=1;
       };
    };
    e=nextEvent();
    if (e.type == Expose) redraw();
  };
    
  copyArea(temppix, win, 0, 0, length, cbthick, cbx, cby);
  destroyPixmap(temppix);
  setLineWidth(1);
  setColor( lastC );
  return presentval;
}}