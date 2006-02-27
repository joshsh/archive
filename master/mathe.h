//area constants
pixel primeB, primefB, fibonacciB, tablesB;

void printPrimes(){
  resetWindow();
  printLine( "Nothing here." );
}

void primeFact(){
  resetWindow();
  printLine( "There isn't anything in this function yet!" );
}

void fibonacci(){
  resetWindow();
  printLine( "Better luck next time." );
}

void addper(){
  resetWindow();
  int startnum=inputNumber("Additive persistence on what integer?");
  
}

void multper(){

}

void mathMain(){
  int box, box2, dontstop;
  
  //assign menu positions (all coordinates not written are 0)
  tablesB.x=1;
  fibonacciB.x=1;
  
  resetAll();

  while( 1==1 ){
    comment("Minor mathematical functions" );
    clearMenu();
    setColor(textC);
    putMenu2(primefB, "prime number", "factorization");
    putMenu(tablesB, "tables" );
    
    box=getChoice(general);
    if (menuNumber(primeB) == box) primeFact();
    if (menuNumber(tablesB) == box){
      dontstop=1;
      while (dontstop){
        comment("Number tables");     
        clearMenu();
        setColor(textC);
        putMenu2(primeB, "prime", "numbers" );
        putMenu2(fibonacciB, "fibonacci", "numbers" );
      
        box2=getChoice(general);
        if (menuNumber(primeB) == box2) printPrimes();
        if (menuNumber(fibonacciB) == box2) fibonacci();
        if (menuNumber(backB) == box2) dontstop=0;
        if (menuNumber(quitB) == box2) exit(1);
      };
    };
    if (menuNumber(backB) == box) return;
    if (menuNumber(quitB) == box) exit(1);
  };
}