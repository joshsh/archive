int griddotC=140, graphC=40;

//area constants
pixel draw2DB, inter2DB, change2DB, color2DB, windo2DB, integral2DB, length2DB;
pixel derivative2DB, values2DB, trace2DB, zoomi2DB, zoomo2DB, range2DB;

pixel cartes2DB, polar2DB, param2DB;

void colors2D(){
  graphC=inputNumber( "Color for function graph = " );
  axesC=inputNumber( "Color for axes = " );
  griddotC=inputNumber( "Grid color = " );
  newLine();
}

#include "F2D.h"
#include "polar.h"
#include "param2D.h"

void 2DMain(){

  //assign menu positions
  setMenu(general);
  resetMenu();
  cartes2DB=addMenu();
  polar2DB=addMenu();
  param2DB=addMenu();
  
  resetAll();
  while (1==1){
    setMenu(general);
    clearMenu();
    setColor( textC );
    putMenu( cartes2DB, "cartesian 1D");
    putMenu( polar2DB, "polar" );
    putMenu(param2DB, "parameter");
    
    getChoiceM( general );

    if (chosen( cartes2DB )) F2DMain();
    if (chosen( polar2DB )) polarMain();
    if (chosen( param2DB )) param2DMain();
    
    if (backM()) return;
    if (quitM()) exit(1);
  };
}

