//note: the notation for a matrix element is reversed in this program,
//that is, the column is written first, then the row,  matrix[col][row]

int matdim=20;
pixel matcor; //corner for showMatrix(), ...

struct matrix{
  double m[20][20]; //[matdim][matdim]!
  int rows, cols;
};

matrix A, B, D;

matrix emptyMatrix(){
  matrix cm1;
  for (i=0; i < matdim; i++){
    for (j=0; j < matdim; j++) cm1.m[i][j] = 0;
  };
  cm1.cols=1;
  cm1.rows=1;
  return cm1;
}

matrix unityMatrix( int urows, int ucols ){
  matrix um1=emptyMatrix();
  um1.rows=urows;
  um1.cols=ucols;
  intA=min( urows, ucols );
  for (i=0; i < intA; i++) um1.m[i][i]=1;
  return um1;
}

matrix randomMatrix( int rrows, int rcols ){
  matrix rm1;
  rm1.rows=rrows;
  rm1.cols=rcols;
  resetPage();
  realA=inputNumber( "Lower bound = " );
  realB=inputNumber( "Upper bound = " );
  realC = realB-realA;
  for (i=0; i < rcols; i++){
    for (j=0; j < rrows; j++) rm1.m[i][j] = realA + (drand48()*realC);
  };
  return rm1;
}

matrix randIntMatrix( int rrows, int rcols ){
  matrix rm1;
  rm1.rows=rrows;
  rm1.cols=rcols;
  resetPage();
  intA=inputNumber( "Lower bound = " );
  intB=inputNumber( "Upper bound = " );
  intC = intB-intA;
  for (i=0; i < rcols; i++){
    for (j=0; j < rrows; j++) rm1.m[i][j] = intA + randInt(intC);
  };
  return rm1;
}

matrix lowerTri( matrix ut1 ){
  for (i=0; i < ut1.rows; i++){
    for (j=0; j < i; j++) ut1.m[i][j]=0;
  };
  return ut1;
}

matrix upperTri( matrix lt1 ){
  for (i=0; i < lt1.rows; i++){
    for (j=i+1; j < lt1.cols; j++) lt1.m[i][j]=0;
  };
  return lt1;
}

matrix transUpper( matrix tu1 ){
  int k;
  double thismult;
  for (i=0; i < tu1.rows; i++){
    for (j=0; j < i; j++){
      thismult=tu1.m[j][i]/tu1.m[j][j];
      for (k=0; k < tu1.cols; k++){
        tu1.m[k][i]=tu1.m[k][i]-(thismult*tu1.m[k][j]);
      };
    };
  };
  return tu1;
}

/*matrix cholesky( matrix cf1 ){  //Cholesky-factorization
  int k;
  for (i=0; i < cf1.rows; i++){
    realA=0;
    for (k=0; k <= i-1; k++) realA=realA+(sq(cf1[i][k]));
    cf1[i][i]=sqrt(cf1[i][i]-realA);
    for (j=i+1; j < cf1.cols; j++){
      for (k=0; k <= i-1; k++) cf1[j][i]=cf1[j][i]-(cf1[i][k]*cf1[j][k]);
      cf1[j][i]=cf1[j][i]/cf1[i][i];
    };
  }; 
  return cf1;
}*/

matrix transLower( matrix tl1 ){
  int k;
  double thismult;
  for (i=tl1.rows; i >= 0; i=i-1){
    for (j=tl1.rows; j > i; j=j-1){
      thismult=tl1.m[j][i]/tl1.m[j][j];
      for (k=0; k < tl1.cols; k++){
        tl1.m[k][i]=tl1.m[k][i]-(thismult*tl1.m[k][j]);
      };
    };
  };
  return tl1;
}

matrix multScalar( matrix ms1, double mscalar ){
  for (i=0; i < ms1.cols; i++ ){
    for (j=0; j < ms1.rows; j++ ) ms1.m[i][j]=ms1.m[i][j]*mscalar;
  };
 return ms1;
}

matrix addMatrices(){
  matrix am1 = emptyMatrix();
  int maxcols=A.cols;
  if (B.cols > A.cols) maxcols=B.cols;
  int maxrows=A.rows;
  if (B.rows > A.rows) maxrows=B.rows;
  for (i=0; i < maxcols; i++){
    for (j=0; j < maxrows; j++) am1.m[i][j]=A.m[i][j]+B.m[i][j];
  };
  am1.cols=maxcols;
  am1.rows=maxrows;
  return am1;
}

matrix subtractMatrices(){
  matrix am1 = emptyMatrix();
  int maxcols=A.cols;
  if (B.cols > A.cols) maxcols=B.cols;
  int maxrows=A.rows;
  if (B.rows > A.rows) maxrows=B.rows;
  for (i=0; i < maxcols; i++){
    for (j=0; j < maxrows; j++) am1.m[i][j]=A.m[i][j]-B.m[i][j];
  };
  am1.cols=maxcols;
  am1.rows=maxrows;
  return am1;
}

matrix multMatrices( matrix mm1, matrix mm2 ){
  int k;
  double sum;
  matrix mm3 = emptyMatrix();
  mm3.cols=mm2.cols;
  mm3.rows=mm1.rows;
  for (i=0; i < mm3.cols; i++){
    for (j=0; j < mm3.rows; j++){
      sum=0;
      for (k=0; k < mm1.cols; k++) sum=sum+(mm1.m[k][j] * mm2.m[i][k]);
      mm3.m[i][j]=sum;
    };
  };
  return mm3;
}

matrix expMatrix( matrix em1, int power ){
  matrix em2=em1;
  for (i=1; i < power; i++) em2=multMatrices( em2, em1 );
  return em2;
}

void showMatrix( matrix sm1, char* mName ){
                              //extractMatrix and changeMatrix must be the same
  int blocklen=disdigits+3;  //eventually adjust this correctly (for all values)
  //if (2+(sm1.cols*blocklen
  saveLimits();
  setLimits(0, xpix, 0, ypix);
  setCorner(matcor.x, matcor.y);
  resetPage();
  setColor( textC );
  printString( "Matrix " ); printString( mName ); newLine(); newLine();
  for (i=0; i < (sm1.rows*2)-1; i++){ printString( "|" ); newLine(); };
  int nn=(blocklen*sm1.cols)+sm1.cols+2;
  for (i=0; i < (sm1.rows*2)-1; i++){ locate(nn,i+2); printString( "|" ); };
  for (i=0; i < sm1.cols; i++ ){
    for (j=0; j < sm1.rows; j++ ){
      locate ((i*(blocklen+1))+1, (2*j)+2);
      blank( 1 );
      locate ((i*(blocklen+1))+2, (2*j)+2);
      //printNumber( cut( sm1.m[i][j], disdigits ) ); fix this.
      printNumber( sm1.m[i][j] );
    };
  };
  resetLimits();
}

matrix changeMatrix( matrix cm1, char* cmName ){
  resetPage();

  int blocklen=disdigits+3;  //don't change this without changing showMatrix()
  //if (2+(sm1.cols*blocklen
  pixel mplace;
  int mtop=lheight*2;
  int mbottom=lheight*((cm1.rows*2)+2);
  int mleft=lwidth*2;
  int mright=lwidth*(2+(cm1.cols*(blocklen+1)));

  Event e;
  updateEvents();
  int stop=0, stop2, stop3;
  comment( "Use the mouse to edit, double enter to procceed to next column." );
  while (!stop){
    e=nextEvent();
    if (e.type == Expose) redraw();
    if ((e.type == ButtonPress)&&(areaName(general, e.x, e.y) == backM())) stop=1;
    if ((e.type == ButtonPress)&&(areaName(general, e.x, e.y) == quitM())) exit(1);
    if ((e.type == ButtonPress)&&(e.x > mleft)&&(e.x < mright)&&(e.y > mtop)&&(e.y < mbottom)){
      mplace.x=(e.x-mleft)/(lwidth*(blocklen+1));
      mplace.y=(e.y-mtop)/(lheight*2);
      stop2=0;
      while (!stop2){
        resetPage();
        cm1.m[mplace.x][mplace.y]=inputNumber("new value =  ");
        showMatrix( cm1, cmName );
        mplace.x=mplace.x+1;
        if (mplace.x > cm1.cols){ mplace.x=0; mplace.y++; };
        if (mplace.y > cm1.rows){ mplace.x=0; mplace.y=0; };
        stop3=0;
        while (!stop3){
          e=nextEvent();
          if (e.type == Expose) redraw();
          if ((e.type == ButtonPress)&&(areaName(general, e.x, e.y) == backM())){ stop=1; stop2=1; stop3=1; };
          if (e.type == KeyPress) stop3=1;
          if ((e.type == ButtonPress)&&(e.x > mleft)&&(e.x < mright)&&(e.y > mtop)&&(e.y < mbottom)){
            mplace.x=(e.x-mleft)/(lwidth*(blocklen+1));
            mplace.y=(e.y-mtop)/(lheight*2);
            stop3=1;
          };        
        };
      };
    };
  };
  return cm1;
}

matrix transpose( matrix t1 ){
  comment( "Transposition function not programmed yet. " );
  matrix t2=t1;
  return t2;
}

matrix invert( matrix in1 ){
  comment( "Inversion function not programmed yet. " );
  matrix in2 = in1;
  return in2;
}

determinant( matrix det1, char* detname ){
  comment( "Determinant function not programmed yet. " );
  double deter;
  //find determinant
  resetPage();
  printString( "Determinant of matrix " ); printString( detname );
  printString( " = " ); printNumber( deter );
  newLine();
}

matrix extractMatrix( matrix em1, char* emname ){

  int blocklen=disdigits+3;  //don't change this without changing showMatrix()
  //if (2+(sm1.cols*blocklen
  pixel mplace;
  int mtop=lheight*2;
  int mbottom=lheight*((em1.rows*2)+2);
  int mleft=lwidth*2;
  int mright=lwidth*(2+(em1.cols*(blocklen+1)));

  int mtop2, mbottom2, mleft2, mright2;
  int ptop, pleft, plength, pheight;
  pixel corner;
  showMatrix( em1, emname );
  comment( "Use the mouse to choose a partial matrix" );
  Event e;
  updateEvents();
  matrix em2=em1;
  int stop=0;
  setColor( partC );
  setDrawMode( GXxor );
  while (!stop){
    e=nextEvent();
    if (e.type == Expose) redraw();
    if ((e.type == ButtonPress)&&(areaName( general, e.x, e.y ) == backM())){
      setDrawMode( GXcopy ); redraw(); return em2; };
    if (e.type == ButtonPress){  
      mplace.x=(e.x-mleft)/(lwidth*(blocklen+1));
      mplace.y=(e.y-mtop)/(lheight*2);
      corner=mplace;
      while (e.type != ButtonRelease){
        e=nextEvent();
        if (e.type == Expose) redraw();
        mplace.x=(e.x-mleft)/(lwidth*(blocklen+1));
        mplace.y=(e.y-mtop)/(lheight*2);
        mtop2=corner.y; mleft2=corner.x;
        if (mplace.y < mtop2) mtop2 = mplace.y;
        if (mplace.x < mleft2) mleft2 = mplace.x;
        mbottom2=mtop2+fabs(mplace.y-corner.y);
        mright2=mleft2+fabs(mplace.x-corner.x);
        plength=(blocklen+1)*fabs(corner.x-mplace.x)*lwidth;
        pheight=2*fabs(corner.y-mplace.y)*lheight;
        ptop=matcor.y+(lheight*((mtop2*2)+2));
        pleft=matcor.x+(lwidth*((mleft2*(blocklen+1))+2));
        drawRectangle(w, pleft, ptop, plength, pheight);
      };
      if ((mtop2>=0)&&(mbottom2<=em1.rows)&&(mleft2>=0)&&(mright2<=em1.cols)){
        em2.rows=fabs(mtop2-mbottom2)+1;
        em2.cols=fabs(mright2-mleft2)+1;
        for (i=0; i < em2.cols; i++){
          for (j=0; j < em2.rows; j++){
            em2.m[i][j]=em1.m[i+mleft2][j+mtop2];
          };
        };
      };
    };
  };
}

void matrixMain(){
  matcor.x=10; matcor.y=10;
  int box, showC, stop;

  pixel matAB=boringB, matBB=boringB, matCB=boringB;
  pixel plusB=boringB, minusB=boringB, timesB=boringB;

  pixel AtBB=boringB, transposeB=boringB, invertB=boringB, detB=boringB;
  pixel dimB=boringB, clrB=boringB, rowB=boringB, colB=boringB, editB=boringB;
  pixel mulscB=boringB, powmaB=boringB, specB=boringB, extractB=boringB;
  
  pixel unitmaB=boringB, uptriB=boringB, lotriB=boringB, randmaB=boringB;
  pixel randimaB=boringB, transupB=boringB, transloB=boringB, choleskyB=boringB;

  pixel CtAB=boringB, CtBB=boringB;

  //assign places for menu postions (all coordinates not written are 0)
  matBB.x=1; matCB.x=2; plusB.x=3;
  minusB.y=1; timesB.y=1;
  timesB.x=1;
  transposeB.x=1; invertB.x=2; detB.x=3;
  dimB.y=1; editB.y=1; rowB.y=1; colB.y=1;
  editB.x=1; rowB.x=2; colB.x=3;
  clrB.y=2; mulscB.y=2; powmaB.y=2; specB.y=2;
  mulscB.x=1; powmaB.x=2; specB.x=3;
  extractB.y=3;
  uptriB.x=1; lotriB.x=2; randmaB.x=3;
  randimaB.y=1; transupB.y=1; transloB.y=1; choleskyB.y=1;
  transupB.x=1; transloB.x=2; choleskyB.x=3;
  CtBB.x=1;

  matrix E;
  A = emptyMatrix();
  B = A;
  D = A;
  resetAll();
  while (1==1){
    comment( "Matrices: main menu" );
    setMenu(general);
    clearMenu();
    setColor( textC );
    putMenu( matAB, "A" );
    putMenu( matBB, "B" );
    putMenu( matCB, "C" );
    putMenu( plusB, "+" );
    putMenu( minusB, "-" );
    putMenu( timesB, "*" );
    box=getChoice( general );
    showC=0;
    if (menuNumber( matAB ) == box){
      stop=0;
      while (!stop){
        showMatrix( A, "A" );
        clearMenu();
        setColor( textC );
        putMenu( AtBB, "A<->B" );
        putMenu( transposeB, "transpose" );
        putMenu( invertB, "invert" );
        putMenu( detB, "determinant" );
        putMenu( dimB, "dimension" );
        putMenu( editB, "edit matrix" );
        putMenu( clrB, "clear matrix" );
        putMenu2( specB, "special", "forms..." );
        putMenu2( mulscB, "multiply", "with scalar" );
        putMenu2( powmaB, "raise to", "power" );
        putMenu2( extractB, "partial", "matrix" );
        putMenu( rowB, "row" );
        putMenu( colB, "column" );
        getChoiceM( general );
        if (chosen( AtBB )2){ E=A; A=B; B=E; showMatrix(A, "A" ); };
        if (chosen( transposeB )2){ 
          D = transpose( A ); showC=1; };
        if (chosen( invertB )2){
          D = invert( A ); showC=1; };
        if (chosen( detB )2) determinant(A, "A" );
        if (chosen( editB )2) A = changeMatrix( A, "A" );
        if (chosen( dimB )2){
          resetPage();
          printLine( "New dimensions for matrix A: " ); newLine();
          A=emptyMatrix();
          A.rows = inputNumber( "Rows = " );
          if (A.rows > matdim){
            printLine( "Warning: this program handles matrices of dimension" );
            printLine( "of at most 20x20.  Assuming rows=20. " ); //matdim!
            A.rows = matdim;
          };
          A.cols = inputNumber( "Columns = " );
          if (A.cols > matdim){
            printLine( "Warning: this program handles matrices of dimension" );
            printLine( "of at most 20x20.  Assuming columns=20. " ); //matdim!
            A.cols = matdim;
          };
          showMatrix( A, "A" );
        };
        if (chosen( clrB )2){
          intA=A.rows;
          intB=A.cols;
          A=emptyMatrix();
          A.rows=intA;
          A.cols=intB;
          showMatrix( A, "A" );
        };
        if (chosen( mulscB )2){
          resetPage();
          realA = inputNumber( "Multiply the matrix by... " );
          D=multScalar( A, realA );
          showC=1;
        };
        if (chosen( powmaB )2){
          intA=-1;
          resetPage();
          while (intA <= 0){
            intA=inputNumber( "To which power (>0) ? " );
            if (intA <=0) printLine( "Wrong answer. Try again>" );
            newLine();
          };
          D=expMatrix( A, intA );
          showC = 1;
        };
        if (chosen( extractB )2){
          D=extractMatrix( A, "A" ); showC = 1; };
        if (chosen( specB )2){
          clearMenu();
          setColor( textC );
          putMenu2( unitmaB, "unity", "matrix" );
          putMenu2( uptriB, "upper tri.", "matrix" );
          putMenu2( lotriB, "lower tri.", "matrix" );
          putMenu2( transupB, "transform", "upper tri." );
          putMenu2( transloB, "transform", "lower tri. " );
          putMenu2( randmaB, "random", "matrix" );
          putMenu2( randimaB, "random integer", "matrix" );
          putMenu2( choleskyB, "Cholesky", "factorization" );
          box3 = getChoice( general );
          if (chosen( unitmaB )3){
            A=unityMatrix(A.rows, A.cols); showMatrix( A, "A" ); };
          if (chosen( uptriB )3){A=upperTri(A); showMatrix(A, "A");};
          if (chosen( lotriB )3){A=lowerTri(A); showMatrix(A, "A");};
          if (chosen( transupB )3){
            A=transUpper(A); showMatrix(A, "A"); };
          if (chosen( transloB )3){
            A=transLower(A); showMatrix(A, "A" ); };
          //if (chosen( choleskyB )3){
            //D=cholesky(A); showC=1; };
          if (chosen( randmaB )3){
            A=randomMatrix( A.rows, A.cols ); showMatrix( A, "A" ); };
          if (chosen( randimaB )3){
            A=randIntMatrix( A.rows, A.cols ); showMatrix( A, "A" ); };
          if (chosen( quitB )3 ) exit(1);
          //(it backs up anyway)
        };
        //if (chosen( rowB )2)...
        //if (chosen( colB )2)...
        if (chosen( backB )2) stop=1;
        if (chosen( quitB )2) exit(1);
        boxM=-2;
      };
    };
    if (menuNumber( matBB ) == box){
      stop=0;
      while (!stop){
        showMatrix( B, "B" );
        clearMenu();
        setColor( textC );
        putMenu( AtBB, "A<->B" );
        putMenu( transposeB, "transpose" );
        putMenu( invertB, "invert" );
        putMenu( detB, "determinant" );
        putMenu( dimB, "dimension" );
        putMenu( editB, "edit matrix" );
        putMenu( clrB, "clear matrix" );
        putMenu2( specB, "special", "forms..." );
        putMenu2( mulscB, "multiply", "with scalar" );
        putMenu2( powmaB, "raise to", "power" );
        putMenu2( extractB, "partial", "matrix" );
        putMenu( rowB, "row" );
        putMenu( colB, "column" );
        getChoiceM( general );
        if (chosen( AtBB )2){ E=A; A=B; B=E; showMatrix(B, "B" ); };
        if (chosen( transposeB )2){ 
          B = transpose( B ); showMatrix(B, "B"); };
        if (chosen( invertB )2){
          D = invert( B ); showC=1; };
        if (chosen( detB )2) determinant(B, "B" );
        if (chosen( editB )2) B = changeMatrix(B, "B");
        if (chosen( dimB )2){
          resetPage();
          printLine( "New dimensions for matrix B: " ); newLine();
          B=emptyMatrix();
          B.rows = inputNumber( "Rows = " );
          B.cols = inputNumber( "Cols = " );
          showMatrix( B, "B" );
        };
        if (chosen( clrB )2){
          intA=B.rows;
          intB=B.cols;
          B=emptyMatrix();
          B.rows=intA;
          B.cols=intB;
          showMatrix( B, "B" );
        };
        if (chosen( mulscB )2){
          resetPage();
          realA = inputNumber( "Multiply the matrix by... " );
          D=multScalar( B, realA );
          showC=1;
        };
        if (chosen( powmaB )2){
          intA=-1;
          resetPage();
          while (intA <= 0){
            intA=inputNumber( "To which power (>0) ? " );
            if (intA <=0) printLine( "Wrong answer. Try again." );
            newLine();
          };
          D=expMatrix( B, intA );
          showC = 1;
        };
        if (chosen( extractB )2){
          D=extractMatrix( B, "B" ); showC = 1; };
        if (chosen( specB )2){
          clearMenu();
          setColor( textC );
          putMenu2( unitmaB, "unity", "matrix" );
          putMenu2( uptriB, "upper tri.", "matrix" );
          putMenu2( lotriB, "lower tri.", "matrix" );
          putMenu2( transupB, "transform", "upper tri." );
          putMenu2( transloB, "transform", "lower tri. " );
          putMenu2( randmaB, "random", "matrix" );
          putMenu2( randimaB, "random integer", "matrix" );
          putMenu2( choleskyB, "Cholesky", "factorization" );
          box3 = getChoice( general );
          if (chosen( unitmaB )3){
            B=unityMatrix(B.rows, B.cols); showMatrix( B, "B" ); };
          if (chosen( uptriB )3){B=upperTri(B); showMatrix(B, "B");};
          if (chosen( lotriB )3){B=lowerTri(B); showMatrix(B, "B");};
          if (chosen( transupB )3){
            B=transUpper(B); showMatrix(B, "B"); };
          if (chosen( transloB )3){
            B=transLower(B); showMatrix(B, "B" ); };
          //if (chosen( choleskyB )3){
            //D=cholesky(B); showC = 1; };
          if (chosen( randmaB )3){
            B=randomMatrix( B.rows, B.cols ); showMatrix( B, "B" ); };
          if (chosen( randimaB )3){
            B=randIntMatrix( B.rows, B.cols ); showMatrix( B, "B" ); };
          if (chosen( quitB )3 ) exit(1);
          //(it backs up anyway)
        };
        //if (chosen( rowB )2)...
        //if (chosen( colB )2)...
        if (chosen( backB )2) stop=1;
        if (chosen( quitB )2) exit(1);
      };
      boxM=-2;
    };
    if (menuNumber( matCB ) == box) showC=1;
    if (menuNumber( plusB ) == box){ D = addMatrices(); showC=1; };
    if (menuNumber( minusB ) == box){ D = subtractMatrices(); showC=1; };
    if (menuNumber( timesB ) == box){
      if (A.cols == B.rows){ 
        D = multMatrices(A, B); showC=1;
      } else printLine( "These matrices can't be multiplied with each other." );
    };
    while (showC){
      showMatrix( D, "C" );
      clearMenu();
      setColor( textC );
      putMenu( CtAB, "C<->A" );
      putMenu( CtBB, "C<->B" );
      box2=getChoice(general);
      if (chosen( CtAB )2){ E=A; A=D; D=E; showC=0; };
      if (chosen( CtBB )2){ E=B; B=D; D=E; showC=0; };
      if (chosen( backB )2) showC=0;
      if (chosen( quitB )2) exit(1);
      boxM=-2;
    };
    if (menuNumber( backB ) == box) return;
    if (menuNumber( quitB ) == box) exit(1);
  };
}
