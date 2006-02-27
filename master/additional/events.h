//a few simple event handling functions.
//graphics.h must be included before this file.
//a function redraw() must be defined before inclusion of this file.

void updateEvents(){
  Event ue1;
  while (checkEvent()){
    ue1=nextEvent();
    if (ue1.type == Expose) redraw();
  };
}

Event getNextEvent(){
  Event gne1;
  while (checkEvent()){
    gne1=nextEvent();
    if (gne1.type == Expose) redraw();
  };
  gne1=nextEvent();
  if (gne1.type == Expose) redraw();
  return gne1;
}

void wait(){
  Event w1;
  updateEvents();
  do{
    w1=nextEvent();
    if (w1.type == Expose) redraw();
  } while (w1.type != ButtonPress );
}

int nextButtonPress(){
  Event nbp;
  updateEvents();
  do{ 
    nbp=nextEvent();
    if (nbp.type == Expose) redraw();
  } while (nbp.type != ButtonPress );
  return nbp.value;
}
