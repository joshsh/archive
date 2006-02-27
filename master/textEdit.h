//menu positions
pixel capB, lowB, caplowB, removeB, replaceB, remspaB, remcharB, remwordB;
pixel newfB;

void capitalize(){

}

void lowerCase(){

}

void switchCases(){

}

void remChar(){

}

void remSpace(){

}

void remString(){

}

void replace(){

}

void tEMain(){
  int box, box2, stop;
  char* filename;
  
  //assign menu positions (all coordinates not written are 0)
  lowB.x=1; caplowB.x=2; removeB.x=3;
  replaceB.y=1; newfB.y=1;
  newfB.x=1;
  remcharB.x=1; remwordB.x=2;
  
  resetAll();
  filename = inputString( "Which file will you edit?" ); //this function doesn't
  //make name into variable                             //yet exist!
  //create backup file

  resetWindow();
  
  while(1==1){
    comment(" ");
    clearMenu();
    setColor(textC);
    putMenu(capB, "capitalize");
    putMenu2(lowB, "lower case" );
    putMenu2(caplowB, "switch cases" );
    putmenu2(removeB, "remove..." );
    putMenu2(replaceB, "replace..." );
    putMenu2(newfB, "new file" ); 
    
    box=getChoice(general);
    if (menuNumber(capB) == box) capitalize();
    if (menuNumber(lowB) == box) lowerCase();
    if (menuNumber(caplowB) == box) switchCases();
    if (menuNumber(removeB) == box){
      stop=0;
      while(!stop){
         comment("Removal utility");
         clearMenu();
         setColor(textC);
         putMenu2(remspaB, "remove spaces" );
         putMenu2(remcharB, "remove character" );
         putMenu2(remwordB, "remove string" );

         box2=getChoice(general);
         if (menuNumber(remspaB) == box2){ remSpace(); stop=1; };
         if (menuNumber(remcharB) == box2){ remChar(); stop=1; };
         if (menuNumber(remwordB) == box2){ remString(); stop=1; };
         if (menuNumber(backB) == box2) stop=1;
         if (menuNumber(quitB) == box2) exit(1);
      };
    };
    if (menuNumber(replaceB) == box) replace();
    if (menuNumber(newfB) == box){
      filename=inputString( "Which file will you edit? " );
      //make name into variable
      //create backup file
    };
    if (menuNumber(backB) == box) return;
    if (menuNumner(quitB) == box) exit(1);
  };
}