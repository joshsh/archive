//note:  initComplex() must be run once in order to create cOne, cI and cZero

struct comp{
  double p;
  double q;
};

comp cZero, cOne, cI;

comp cAdd( comp cAdd1, comp cAdd2 ){
  comp cAdd3;
  cAdd3.p=cAdd1.p+cAdd2.p;
  cAdd3.q=cAdd1.q+cAdd2.q;
  return cAdd3;
}

comp cSub( comp cSub1, comp cSub2 ){
  comp cSub3;
  cSub3.p=cSub1.p-cSub2.p;
  cSub3.q=cSub1.q-cSub2.q;
  return cSub3;
}

comp cMult( comp cMult1, comp cMult2 ){
  comp cMult3;
  cMult3.p=(cMult1.p*cMult2.p)-(cMult1.q*cMult2.q);
  cMult3.q=(cMult1.p*cMult2.q)+(cMult1.q*cMult2.p);
  return cMult3;
}

comp cExp( comp cExp1, int cPow ){
  comp cExp2=cExp1;
  for (int iexp=1; iexp < cPow; iexp++) cExp2=cMult(cExp1, cExp2);
  return cExp2;
}

//comp cCompExp( comp cExp1, comp cPow ){
//
//}

comp cConj( comp cConj1 ){
  cConj1.q=-cConj1.q;
  return cConj1;
}

comp cInv( comp cInv1 ){
  comp cInv2;
  cInv2.p=(cInv1.p)/((cInv1.p*cInv1.p)+(cInv1.q*cInv1.q));
  cInv2.q=(-cInv1.q)/((cInv1.p*cInv1.p)+(cInv1.q*cInv1.q));
  return cInv2;
} 

double cAbs( comp cAbs1 ){
  return sqrt((cAbs1.p*cAbs1.p)+(cAbs1.q*cAbs1.q));
}

comp cDiv( comp cDiv1, comp cDiv2 ){
  return cMult( cDiv1, cInv( cDiv2 ));
}

comp cDistR( comp cDistR1, double cdrfac ){
  comp cDistR2;
  cDistR2.p=cDistR1.p*cdrfac;
  cDistR2.q=cDistR1.q*cdrfac;
  return cDistR2;
}

comp cDistI( comp cDistI1, double cdifac ){
  comp cDistI2;
  cDistI2.p=-cDistI1.q*cdifac;
  cDistI2.q=cDistI1.p*cdifac;
  return cDistI2;
}

double cDiff( comp cDiff1, comp cDiff2 ){
  double cDiff3=(cDiff1.p-cDiff2.p)*(cDiff1.p-cDiff2.p);
  double cDiff4=(cDiff1.q-cDiff2.q)*(cDiff1.q-cDiff2.q);
  return sqrt( cDiff3 + cDiff4 );
}

void initComplex(){
  cOne.p=1;
  cOne.q=0;
  cZero.p=0;
  cZero.q=0;
  cI.p=0;
  cI.q=1;
}
