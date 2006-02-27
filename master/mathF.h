//#include "functions.h"

void mathFMain(){

  resetAll();
  setMenu(leftmenu);
  setPage(mpage);
  resetMenu();
  pixel boxcarB=addMenu(), cosB=addMenu(), heavB=addMenu(), ramB=addMenu();
  pixel rectB=addMenu(), sinB=addMenu();
  pixel triB=addMenu(), tri3B=addMenu();
  clearMenu();
  putMenu(boxcarB, "boxcar");
  putMenu(cosB, "cosine");
  putMenu(heavB, "Heaviside step");
  putMenu(ramB, "Ramanujan");
  putMenu(rectB, "rectangle" );
  putMenu(sinB, "sine");
  putMenu(triB, "triangle");
  putMenu(tri3B, "triangle 3D");
  writeBack(); writeQuit();
  resetPage();
  while(1==1){
    getChoiceM(leftmenu);
    if(chosen(cosB)){ printNumber(cos(inputNumber("x = "))); newLine(); };
    if(chosen(sinB)){ printNumber(sin(inputNumber("x = "))); newLine(); };
    /*
    if(chosen(boxcarB)){
      
    };
    if(chosen(heavB)){
    
    };
    if(chosen(ramB)){
    
    };
    if(chosen(rectB)){
    
    };
    if(chosen(triB)){
    
    };
    if(chosen(tri3B0){
    
    };*/
    if(backM()) return;
    if(quitM()) exit(1);
  };  
}