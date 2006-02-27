struct pString{
  double x, y;
  pString *next;
};

  //centered=0 -- the function graphs the value y as AFTER x and before x->next.
  //  the last point is only for telling where the graph should end.
  //centered=1 -- the function graphs y as a direct funciton of x.
void barGraph1D(pstring *pfirst, int centered){
  clearScreen();
  if (!centered){
    
  } else {
  
  };
}

void statisticsMain(){
  int box;
  
  //assign menu positions
  newMainMenu();
  
  while (1==1){
    resetAll();
    setColor(textC);
    
    box=getChoice(general);
    
    if (menuNumber(box) == backB) return;
    if (menuNumber(box) == quitB) exit(1);
  };
}