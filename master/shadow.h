int lcol=140, scol=lcol-30;
pixel posA, posB, posC;
int size=15;
int blen=xpix/size;
int lowcolor, notfound;
char dir;

void shadowBox(pixel posA){
  setColor(setmatrix[posA.x][posA.y]);
  fillRectangle(win,posA.x*blen,posA.y*blen,blen,blen);
}

pixel newpos(pixel posA){
  posB=posA;
  lowcolor=lcol;
  if (setmatrix[posA.x][posA.y-1]<lowcolor){dir='u';
    lowcolor=setmatrix[posA.x][posA.y-1];};
  if (setmatrix[posA.x-1][posA.y]<lowcolor){dir='l';
    lowcolor=setmatrix[posA.x-1][posA.y];};
  if (setmatrix[posA.x][posA.y+1]<lowcolor){dir='d'; 
    lowcolor=setmatrix[posA.x][posA.y+1];};
  if (setmatrix[posA.x+1][posA.y]<lowcolor){dir='r';
    lowcolor=setmatrix[posA.x+1][posA.y];};
  if (lowcolor == lcol) dir = 'n';
  switch (dir){
    case 'u': {posB.y=posA.y-1; break;};
    case 'd': {posB.y=posA.y+1; break;};
    case 'l': {posB.x=posA.x-1; break;};
    case 'r': {posB.x=posA.x+1; break;};
    case 'n': break;
  };
  setmatrix[posB.x][posB.y]=lcol;
  shadowBox(posB);
  return posB;
}

void newsquare(){
  for (i=0;i<size;i++){
    for (j=0;j<size;j++){
      posB.x=i;
      posB.y=j;
      shadowBox(posB);
    };
  };
}

pixel startpos(){
  notfound=1;
  while (notfound==1){
    i=int(drand48()*size);
    j=int(drand48()*size);
    if (setmatrix[i][j] == scol) notfound=0;
  };
  posC.x=i;
  posC.y=j;
  setmatrix[i][j]=lcol;
  shadowBox(posC);  
  return posC;
}

void lighten(){
  for (i = 1; i < size-1; i++){
    for (j = 1; j < size-1; j++){
      if (setmatrix[i][j]>scol) setmatrix[i][j]=setmatrix[i][j]-1;
    };
  };  
  newsquare();
}


int shadowMain(){
  int steps, thisbox;
  pixel pos1,pos2,pos3,pos4,pos5; 

  resetMatrix();

  for (i=0;i<size;i++){
    for (j=0;j<size;j++) setmatrix[i][j]=lcol;
  };
  for (i=1;i<size-1;i++){
    for (j=1;j<size-1;j++) setmatrix[i][j]=scol;
  };

  //set screen
  setColor(lcol);
  fillRectangle(win,0, 0, xpix, ypix);
  for (i=0;i<size;i++){
    for (j=0;j<size;j++){
      posB.x=i;
      posB.y=j;
      shadowBox(posB);
    };
  };

  pos1=startpos();
  pos2=startpos();
  pos3=startpos();
  pos4=startpos();
  pos5=startpos();

  while (1==1){
    lighten();
    pos1=newpos(pos1);
    pos2=newpos(pos2);
    pos3=newpos(pos3);
    pos4=newpos(pos4);
    pos5=newpos(pos5);
    thisbox = gotChoiceM(general);
    if (thisbox >= 0) return thisbox;
  };
}
