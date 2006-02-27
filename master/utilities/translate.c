#include "graphics.h"
#include "files.h"

int i, j;
int backC, whiteC;

int const xpix = 550;
int const ypix = 800;

int const maxlettersize = 14;
int placeX, placeY;
int borderX=5, borderY=5;

Window win;
Pixmap pix;

void redraw(){
  copyArea(pix, win, 0, 0, xpix, ypix, 0, 0);
}

void updateEvents(){
  Event ue1;
  while (checkEvent()){
    ue1=nextEvent();
    if (ue1.type == Expose) redraw();
  };
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

int main(){
  fstream lowfile;
  char *letters[63];
  fstream files[63];
  int openlist[63];
  Pixmap pixes[63], sketchboard;
  int lengthlist[63], heightlist[63];
  char filename[20];
  char ch;

  win = createWindow(0, 0, xpix, ypix, "Alphabet transcription program");
  pix = createPixmap(win, xpix, ypix);
  sketchboard = createPixmap(win, 50, 50);

  int found = 0;
  backC=200;
  while (!found){
    backC++;
    setColor(backC);
    fillRectangle(win, 0, 0, xpix, ypix);
    j=nextButtonPress();
    if (j==1) found=1;
  };
  
  //backC=204;
  whiteC=backC-1;
  
  setColor(whiteC);
  fillRectangle(pix, 0, 0, xpix, ypix);
  redraw();
    
  cout << endl << "Which document is to be transcribed? ";
  cin >> filename; cout << endl;
  
  //lowercase( filename, "midlet.doc" );
  
  lowfile.open( filename, ios::in );
  if (!lowfile.good()){ cout << "<bad file, aborting>" << endl; exit(1); };

  letters[0] = "null.map"; 
  letters[1] = "a.map";
  letters[2] = "b.map";      
  letters[3] = "c.map";
  letters[4] = "d.map";          
  letters[5] = "e.map";
  letters[6] = "f.map";      
  letters[7] = "g.map";
  letters[8] = "h.map";         
  letters[9] = "i.map";
  letters[10] = "j.map";      
  letters[11] = "k.map";
  letters[12] = "l.map";          
  letters[13] = "m.map";
  letters[14] = "n.map";      
  letters[15] = "o.map";
  letters[16] = "p.map";                 
  letters[17] = "q.map";
  letters[18] = "r.map";      
  letters[19] = "s.map";
  letters[20] = "t.map";          
  letters[21] = "u.map";
  letters[22] = "v.map";      
  letters[23] = "w.map";
  letters[24] = "x.map";         
  letters[25] = "y.map";
  letters[26] = "z.map";
  letters[27] = "0.map";      
  letters[28] = "1.map";
  letters[29] = "2.map";          
  letters[30] = "3.map";
  letters[31] = "4.map";      
  letters[32] = "5.map";
  letters[33] = "6.map";                 
  letters[34] = "7.map";
  letters[35] = "8.map";      
  letters[36] = "9.map";
  letters[37] = "A.map";
  letters[38] = "B.map";      
  letters[39] = "C.map";
  letters[40] = "D.map";          
  letters[41] = "E.map";
  letters[42] = "F.map";      
  letters[43] = "G.map";
  letters[44] = "H.map";         
  letters[45] = "I.map";
  letters[46] = "J.map";      
  letters[47] = "K.map";
  letters[48] = "L.map";          
  letters[49] = "M.map";
  letters[50] = "N.map";      
  letters[51] = "O.map";
  letters[52] = "P.map";                 
  letters[53] = "Q.map";
  letters[54] = "R.map";      
  letters[55] = "S.map";
  letters[56] = "T.map";          
  letters[57] = "U.map";
  letters[58] = "V.map";      
  letters[59] = "W.map";
  letters[60] = "X.map";         
  letters[61] = "Y.map";
  letters[62] = "Z.map";
  for (i=0; i<63; i++){ files[i].open(letters[i], ios::in); openlist[i]=1; };
  for (i=37; i<63; i++){
    if (!files[i].good()){
      files[i].close();
      files[i].open(letters[i-36], ios::in);
    };
  };
  for (i=0; i<63; i++){
   if (!files[i].good()){ files[i].close(); openlist[i]=0; };
  };
  //cout << "here I am" << endl; exit(1);
  //int howfar;
  //cout << "how far ? "; cin >> howfar; cout << endl; 
  char ch1;
  int letterlen;
  for (int ii=0; ii<63; ii++){
    if (openlist[ii]){
      //cout << ii << "|";
      //if (ii >= howfar) exit(1);
      setColor(whiteC);
      fillRectangle(sketchboard, 0, 0, 50, 50);
      i=0; j=0;
      setColor(backC);
      ch1=files[ii].get();
      while(!files[ii].eof()){
        if (ch1=='1') drawPoint( sketchboard, placeX+i, placeY+j );
        i++;
        if (ch1==10){ letterlen=i; i=0; j++;};
        ch1=files[ii].get();
      };
      pixes[ii]=createPixmap(win, letterlen+1, j+1);
      lengthlist[ii]=letterlen+1; heightlist[ii]=j+1;
      copyArea(sketchboard, pixes[ii], 0, 0, letterlen+1, j+1, 0, 0);
    };
  };
  
  //cout << "here" << endl; exit(1);
  placeX=borderX; placeY=borderY;
  ch=lowfile.get();
  int drawchar, whichletter;
  while (!lowfile.eof()){  
    drawchar=1;
    whichletter=0;
    if((ch>47)&&(ch<58)) whichletter=ch-21;
    if((ch>96)&&(ch<123)) whichletter=ch-96;
    if((ch>64)&&(ch<91)) whichletter=ch-28;
    if (!openlist[whichletter]) whichletter=0;
    if (ch == 10) { drawchar=0; placeX=borderX; placeY=placeY+maxlettersize+2; };

    if (drawchar){
      copyArea(pixes[whichletter], pix, 0, 0, lengthlist[whichletter], heightlist[whichletter], placeX, placeY); 
      placeX=placeX+lengthlist[whichletter];
    };
    ch=lowfile.get();
  };
  redraw();
  i=nextButtonPress();
}







