//note: fgets() gets a string of ONE LESS byte in length than specified.

#include <fstream.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "char.h"

//preliminary declarations-------------------------------------------------

  int flip(int);
  void show(char *);
  void Y(int);

  int countAllWords(char *);
  int countCharF(char *);
  int countLines(char *);
  //int countString(char *, char *);
  int countWord(char *, char *);
  int countWords(char *);
  int fileLength(char *);
  int findStrF(char *, char *);
  char *getLine(char *, int);
  int longestLine(char *);
  void newEnding(char *, char *);
  //int findLowestLine(char *);
  char *returnLowestLine(char *);
  //int findNthLine(char *);
  char *returnNthLine(char *);
  void strFile(char *, char *);
  //char *randomLine(char *);

  void alphabetize(char *, char *); 
  void appendLine1(char *, char *);   
  void appendLine(char *, char *, char *);
  void clearFile(char *);
  void deleteLines(char *, char *, char);
  void extractCore(char *, char *, char);
  void fileCopy(char *, char *);
  void killCurlyBrackets(char *, char *);
  void killLinesWith(char *, char *, char);
  void lowercase(char *, char *);
  void remCharF(char *, char *, char);
  void remSpaceF(char *, char *);
  void removeLine(char *, char *, int);
  void removeRepetitions(char *, char *);  
  void stripJunk(char *, char *);
  void swapChar(char *, char *, char, char);
  void switchCases(char *, char *);
  void trimFront(char *, char *, int);
  void uppercase(char *, char *); 
  
  void addFiles(char *, char *, char *);
  void addFiles3(char *, char *, char *, char *);
  void alphabetizeAndMerge(char *, char *, char *);
  void insertFile(char *, char *, int, char *);
  void mergeAlphabetically(char *, char *, char *);

//---------------------------------------------------------------------

  //meant to take a 0 or 1 and flip it to a 1 or 0, respectively
int flip(int toflip){
  if (!toflip) return 1;
  return 0;
}

void show(char *toshow){
  cout << endl << "<" << toshow << ">" << endl;
}

void Y( double yell ){
  cout << "<label " << yell  << ">" << endl;
}

//functions without file output===========================================

  //counts the total number of words, including repeats
int countAllWordsF( char *name1 ){
  fstream file1;
  file1.open(name1, ios::in);
  int word=0, wordcount=0;
  char ch=file1.get();
  while (!file1.eof()){
    if (isalnum(ch)){ word=1; }else{ if (word){ wordcount++; word=0; }; };
    ch=file1.get();
  };
  if (word) wordcount++;
  return wordcount;  
}

  //counts the number of appearances of the character cch
int countCharF( char *CCName, int cch ){
  fstream fileCC;
  fileCC.open( CCName, ios::in );
  if (!fileCC.good()) return 0;
  char cch2=fileCC.get();
  int cci=0;
  while (!fileCC.eof()){
    if (cch2==cch) cci++;
    cch2=fileCC.get(); 
  };
  return cci;
}

 //counts the number of line feeds, plus one
int countLines( char *CLName ){
  fstream fileCL;
  fileCL.open( CLName, ios::in );
  if (!fileCL.good()) return 0;
  char ch=fileCL.get();
  //if (ch < 32) return 0;
  int cli=0;
  while (!fileCL.eof()){
    if (ch == 10) cli++;
    ch=fileCL.get();
  };
  return cli+1;
}

  //counts the appearances of a string in a file
/*
int countString( char *name1, char *string1 ){
  fstream fileA;
  fileA.open(name1, ios::in);
  int csi=0;
  char ch=fileA.get();
  while(!fileA.eof()){ csi++; ch=fileA.get(); };
  
  char *Ch;
  FILE *file1=fopen(name1, "r");
  int thislen=strlen(string1);
  int count=0;
  for (int csi2 =0; csi2 <= csi-thislen; csi2++){
    fseek(file1, csi2, 0);
    fgets(Ch, thislen, file1);
    if (strnCmp(Ch, string1)) count++; 
  };
  return count;    
}*/

  //counts the appearances of one particular word
int countWord( char *name1, char *word ){
  fstream file1;
  file1.open(name1, ios::in);
  char *Ch=new char, *ChA=Ch;
  int onword=0, wordcount=0;
  char ch=file1.get();
  while (!file1.eof()){
    if (isalnum(ch)){
      onword=1;
      *Ch=ch;
      Ch++; 
    }else{
      if (onword){
       if (strnCmp(word, ChA)) wordcount++;
       onword=0;
       Ch=new char;
       //Ch++;
       ChA=Ch;
      };
    };
    ch=file1.get();
  };
  if ((onword)&&(strnCmp(ChA, word))) wordcount++;
  return wordcount;  
}

  //counts each word only once
int countWords( char *name1 ){
  int ii;
  char *wordList[1000];
  //exit(1);
  //for (ii=0; ii<1000; ii++) *wordList[ii]='\0';
  
  //exit(1);
  //word *first=new word;
  //word *current, *nextword;
  char *Ch = new char;
  char *ChA=Ch;
  fstream file1;
  file1.open(name1, ios::in);
  int onword=0, wordcount=0, newword;
  char ch=file1.get();
  //ii=0;
  //exit(1);
  while (!file1.eof()){
    if (isalnum(ch)){ onword=1; *Ch=ch; Ch++;
    }else{
      if (onword){
        newword=1;
        //current=first;
        //while (current->next != NULL){
        ii=0;
        //exit(1);
        while( *wordList[ii] != '\0'){ 
          exit(1);
          //if (strnCmp(Ch, current->Ch)) newword=0;
          if (strnCmp(Ch, wordList[ii])) newword=0;
          //cout << newword;
          //current=current->next;
          ii++;
          //if (ii>limit) exit(1);
        };
        //cout << "got here";
        //exit(1);
        if (newword){
          wordcount++;
          //current->Ch=Ch;
          wordList[ii]=ChA;
          //nextword=new word;
          //current->next=nextword;
        };
        //exit(1);
        Ch=new char;
        ChA=Ch;
        //exit(1);
      };  
      onword=0;
    };
    ch=file1.get();
    //cout<<ch;
  };
  return wordcount;
}

  //returns the byte length of a file
int fileLength( char *CCName ){
  fstream fileCC;
  fileCC.open( CCName, ios::in );
  if (!fileCC.good()) return 0;
  char cch=fileCC.get();
  int cci=0;
  while (!fileCC.eof()){
    cci++;
    cch=fileCC.get();
  };
  return cci;
}

/*
int findLowestLine( char *name1 ){
  char *Ch = new char, *Ch2 = new char;
  int fli, flj;
  int llen=countLines(name1);
  Ch=getLine(1, name1);
  flj=1;
  for(fli=2; fli < llen; fli++){
    Ch2=getLine(fli, name1);
    if (strcmp(Ch2, Ch) <= 0){ flj=fli; Ch=Ch2; };
  };
  return flj;
}
*/

  //finds the line which stands n'th in alphabetical order
/*
int findNthLine(int fnl, char *name1 ){
  char *Ch = new char, *Ch2 = new char;
  int fli, flj, flk;
  int llen=countLines(name1);
  
  Ch=getLine(1, name1);
  flj=1;
  for(fli=2; fli < llen; fli++){
    Ch2=getLine(fli, name1);
    if (strcmp(Ch2, Ch) <= 0) Ch=Ch2; 
  };
  
  char *Clowest=new char;
  Clowest=Ch;

  for (flk=1; flk<fnl; flk++){
    Ch=getLine(1, name1);
    flj=1;
    for(fli=2; fli < llen; fli++){
      Ch2=getLine(fli, name1);
      if (strcmp(Ch2, Ch) <= 0){
        if (strcmp(Clowest, Ch2) <= 0 ){ flj=fli; Ch=Ch2; };
      };
    };
    Clowest=Ch;
  };
  
  return flj;

}
*/

  //returns the position of the start of the first occurence of the string
  //stringB in the file, with the first character as 0.
  //returns -1 if stringB does not appear in the file
int findStrF(char *name1, char *stringB){
  FILE *file1=fopen(name1, "r");
  char *stringA;
  int jj, thislen=strlen(stringB), thatlen=fileLength(name1)-thislen;
  for(jj=0; jj<thatlen; jj++){
    fseek(file1, jj, 0);
    fgets(stringA, thislen+1, file1);
    if (strnCmp(stringA, stringB)) return jj;
  };
  return -1;  
}

  //returns line linenum without the line feed
  //first line is numbered 1, not 0
char *getLine(int linenum, char *GLName){
  if (linenum > countLines(GLName)) return "";
  int gli=1;
  fstream fileGL;
  fileGL.open( GLName, ios::in );
  if (!fileGL.good()) return "";
  char ch=fileGL.get();
  while(gli<linenum){
    if (ch == 10) gli++;
    ch=fileGL.get();
  };
  char *string1=new char, *string1t=string1;
  while(( ch != 10 )&&(!fileGL.eof())){
    *string1t=ch;
    string1t++;
    ch=fileGL.get();
  };
  *string1t='\0';
  return string1;
}

  //returns the byte length of the longest line in a file
int longestLine( char *name1 ){
  fstream file1;
  file1.open(name1, ios::in);
  char ch;
  int thislen=0;
  int maxlen=0;
  
  ch=file1.get();
  while (ch == 10) ch=file1.get();
  while (!file1.eof()){
    thislen++;
    if (thislen > maxlen) maxlen=thislen;
    ch=file1.get();
    while (ch == 10){
      thislen=0;
      ch=file1.get();
    };
  };
  return maxlen;
}

  //changes the ending of a file's name
void newEnding(char *name1, char *ending){
  char *ChY=new char, *ChX=ChY, *ChZ=name1;
  while ((*ChZ != '\0')&&(*ChZ != '.')){ *ChX=*ChZ; ChX++; ChZ++; };
  *ChX='.'; ChX++;
  while (*ending != '\0'){ *ChX=*ending; ChX++; ending++; };
  *ChX='\0';
  rename(name1, ChY);  
}

  //returns a randomly chosen line of a file
  //the "slow way", not meant for rapid procedures
/*char *randomLine( char *name1 ){
  return getLine( name1, randInt( 1, countLines(name1) ) );
}*/

char *returnLowestLine( char *name1 ){
  char *Ch = new char, *Ch2 = new char;
  int fli;
  int llen=countLines(name1);
  Ch=getLine(1, name1);
  for(fli=2; fli < llen; fli++){
    Ch2=getLine(fli, name1);
    if (strcmp(Ch2, Ch) <= 0) Ch=Ch2; 
  };
  return Ch;
}

  //returns the line which stands n'th in alphabetical order
  //assumes no two lines of the file are the same
char *returnNthLine(int fnl, char *name1 ){
  char *Ch = new char, *Ch2 = new char;
  int fli, flj, flk;
  int llen=countLines(name1);
  if (fnl > llen){ cout << "Error in returnLine()" << endl; return ""; };
  
  Ch=getLine(1, name1);
  flj=1;
  for(fli=2; fli < llen; fli++){
    Ch2=getLine(fli, name1);
    if (strcmp(Ch2, Ch) <= 0){ flj=fli; Ch=Ch2; };
  };
  
  char *Clowest=new char;
  Clowest=Ch;
  
  Ch=getLine(1, name1);
  if (flj==1) Ch=getLine(2, name1);

  for (flk=1; flk < fnl; flk++){
    //*Ch=255;
    Ch="~";
    for(fli=1; fli <= llen; fli++){
      Ch2=getLine(fli, name1);
      if (strcmp(Clowest, Ch2) < 0){
        if (strcmp(Ch2, Ch) < 0) Ch=Ch2;
      };
    };
    Clowest=Ch;
  };
  
  return Clowest;
}

  //takes a string and makes it into a file
void strFile(char *Chline, char *name1){
  fstream file1;
  file1.open(name1, ios::out);
  char ch=Chline[0];
  int ctfi=0;
  while(ch != '\0'){
    file1.put(ch);
    ctfi++;
    ch=Chline[ctfi];
  };
}

//output functions of one file==============================================
/*
void alphabetize( char *name1, char *name2 ){
  stripJunk(name1);

  char *Ch=new char;
  int ai;
  
  clearFile(name2);
  clearFile("temp.doc");
  
  int llen=countLines(name1);
  int whichfile=0;
  charToFile(returnLowestLine(name1), name2);
  
  for (ai=2; ai <= llen; ai++){
    whichfile=flip(whichfile);
    Ch=returnNthLine(ai, name1);
       
    if (!whichfile) appendLine(Ch, "temp.doc", name2);
    else appendLine(Ch, name2, "temp.doc"); 
  };
  
  if (whichfile == 1) fileCopy("temp.doc", name2);
  remove("temp.doc");
}*/

void alphabetize( char *name1, char *name2 ){
  //stripJunk(name1);

  struct node{
    char *Ch;
    node *next;
  };
  
  fstream file1, file2;
  file1.open(name1, ios::in );
  
  node *first = new node;
  
  int llen=countLines(name1);
  
  node *nodeA=first;
  node *nodeB, *nodeC, *nodeD;
  char ch=file1.get();
  while (ch == 10) ch = file1.get();
  nodeA->Ch = new char;
  nodeC=nodeA;
  char *Ch2=nodeC->Ch;
  nodeB=new node;
  while (!file1.eof()){
                   //right here is where it crashes on the next word when the
                   //one before it was >9 bytes long (Segmentation fault)
    if (ch != 10){ *nodeA->Ch=ch; nodeA->Ch++;  
    }else{
      *nodeA->Ch='\0';
      nodeA->Ch=Ch2;
      nodeA->next=nodeB; 
      nodeA->Ch=nodeC->Ch;
      nodeA=nodeB;
      nodeC=nodeA;
      Ch2=nodeC->Ch;
      nodeB=new node;
    };
    ch=file1.get();
    if (file1.eof()){ *nodeA->Ch='\0'; nodeA->Ch=Ch2; };
  };

  file1.close();

  nodeB=first;
  node *first2=new node;
  first2->Ch=first->Ch;
  int stop;
  while (nodeB->next != NULL){
    nodeA=first2;
    nodeB=nodeB->next;
    if (strcmp(nodeB->Ch, first2->Ch) < 0){
      nodeC=new node;
      nodeC->Ch=nodeB->Ch;
      nodeC->next=first2;
      first2=nodeC;
    } else {
      stop=0;
      if (nodeA->next == NULL){
        nodeC=new node;
        nodeC->Ch=nodeB->Ch;
        nodeA->next=nodeC;
        stop=1;
      };
      while(!stop){
        nodeC=nodeA->next;
        if (strcmp(nodeB->Ch, nodeC->Ch) < 0){
          nodeD=new node;
          nodeA->next=nodeD;
          nodeD->next=nodeC;
          nodeD->Ch=nodeB->Ch;
          nodeA=nodeD;
          stop=1;
        } else {
          if( nodeC->next == NULL){
            nodeD = new node;
            nodeD->Ch=nodeB->Ch;
            nodeC->next=nodeD;
            stop=1;
          };
        };
        nodeA=nodeC;
      };
    };
  };
  
  nodeA=first2;
  file2.open(name2, ios::out);
  
  while(nodeA->next != NULL){
    ch=*nodeA->Ch;
    while (ch != '\0'){ file2.put(ch); nodeA->Ch++; ch=*nodeA->Ch; };
    nodeA=nodeA->next;
    file2.put(10);
  };
  ch=*nodeA->Ch;
  while (ch != '\0'){ file2.put(ch); nodeA->Ch++; ch=*nodeA->Ch; };
  nodeA=nodeA->next;
  
  file2.close();
}

void appendLine1(char *name1, char *appChar ){
  FILE *file1=fopen(name1, "a");
  fputc(10, file1);
  fputs(appChar, file1);
}

void appendLine(char *name1, char *name2, char *appChar ){
  int ali=0;
  char ch;
  
  fstream file1, file2, fileA;
  file1.open(name1, ios::in);
  fileA.open(name2, ios::out);
  
  ch=file1.get();
  while (!file1.eof()){
    fileA.put(ch);
    ch=file1.get();
  };
  fileA.put(10);
  ch=appChar[ali];
  while(ch != '\0'){
    fileA.put(ch);
    ali++;
    ch=appChar[ali];
  };  
}


void clearFile(char *name1){
  fstream file1;
  file1.open(name1, ios::out);
  file1.close();
}

  //gets rid of all lines starting with the character chb
void deleteLines( char *name1, char *name2, char chb ){
  fstream file1, file2;
  file1.open(name1, ios::in);
  file2.open(name2, ios::out);
  
  char ch=file1.get();
  while (!file1.eof()){
    if (ch == chb){
      while ((ch != 10)&&(!file1.eof)) ch=file1.get();
      ch=file1.get();
    }else{
      file2.put(ch); 
      while ((ch != 10)&&(!file1.eof)){
        ch=file1.get();
        if (!file1.eof()) file2.put(ch);
      };
      ch=file1.get();
    };
  } 
}

  //to remove the chX "border" from a file, as well as leading and trailing
  //line feeds
  //the line feed 10 is not allowed as chX
void extractCore(char *name1, char *name2, char chX){
  char ch;
  fstream file1, file2;
  file1.open(name1, ios::in);
  file2.open(name2, ios::out);
  int bottomb=0, topb=countLines(name1)-1, leftb=longestLine(name1)-1, rightb=0;
  ch=file1.get();
  while (ch == 10) ch=file1.get();
  int xx=0, yy=0;
  int foundone=0;
  while (!file1.eof()){
    if ((ch != chX)&&(ch != 10)){
      //foundone=1;
      if (yy > bottomb) bottomb=yy;
      if (yy < topb) topb=yy;
      if (xx < leftb) leftb=xx;
      if (xx > rightb) rightb=xx;
    };
    ch=file1.get();
    xx++;
    while (ch == 10){
      yy++;
      xx=0;
      ch=file1.get();
    };
  };
  
  file1.close();
  file1.open(name1, ios::in);
  ch=file1.get();
  for (yy=0; yy < topb; yy++){
    while (ch != 10) ch=file1.get();
  };
  for (yy=topb; yy <= bottomb; yy++){
    for (xx=0; xx < leftb; xx++){
      ch=file1.get();
      if ((file1.eof())||(ch == 10)) xx=leftb;
    };
    for (xx=leftb; xx <= rightb; xx++){
      file2.put(ch);
      ch=file1.get();
      if ((file1.eof())||(ch == 10)) xx=rightb+1;
    };
    while ((ch != 10)&&(!file1.eof())) ch = file1.get();
    while (ch == 10){ file2.put(10); ch=file1.get(); };  
  };
}

void fileCopy(char *name1, char *name2){
  char ch;
  fstream file1, file2;
  file1.open(name1, ios::in);
  file2.open(name2, ios::out);
  ch=file1.get();
  while(!file1.eof()){
    file2.put(ch);
    ch=file1.get();
  };
}

  //gets rid of {all text in curly brackets, including the brackets}.
void killCurlyBrackets(char *name1, char *name2){
  fstream file1, file2;
  file1.open(name1, ios::in);
  file2.open(name2, ios::out);
  char ch=file1.get();
  while (!file1.eof()){
    while ((!file1.eof())&&(ch != '{')){ file2.put(ch); ch=file1.get(); };
    while ((!file1.eof())&&(ch != '}')) ch=file1.get(); 
    ch=file1.get();
  };
}

  //gets rid of all lines beginning with the character ch
void killLinesWith(char *name1, char *name2, char ch){
  fstream file1, file2;
  file1.open(name1, ios::in);
  file2.open(name2, ios::out);
  char chk=file1.get();
  while (!file1.eof()){
    if (chk != ch){
      while ((!file1.eof())&&(chk != 10)){ file2.put(chk); chk=file1.get(); };
      file2.put(10);
    }else{
      while ((!file1.eof())&&(chk != 10)) chk=file1.get();
    };
    chk=file1.get();
    while (chk == 10){ file2.put(10); chk=file1.get(); };
  };
}

void lowercase( char *name1, char *name2 ){
  char ch, ch2;

  fstream file1, file2;
  file1.open( name1, ios::in );
  file2.open( name2, ios::out );
  
  ch = file1.get();
  while( !file1.eof() ){
    ch2=tolower(ch);
    file2.put(ch2);
    ch = file1.get();
  };
}

  //remove character chrem from the file wherever it appears
void remCharF( char *name1, char *name2, char chrem ){
  char ch;
  
  fstream file1, file2;
  file1.open( name1, ios::in );
  file2.open( name2, ios::out );  
 
  ch = file1.get();
  while( !file1.eof() ){
    if (ch != chrem) file2.put(ch);
    ch = file1.get();
  };
}

void removeLine(char *name1, char *name2, int linenum ){
  fstream file1, file2;
  file1.open(name1, ios::in);
  file2.open(name2, ios::out);
  
  char ch=file1.get();
  int ali=1;
  while(ali < linenum){
    file2.put(ch);
    ch=file1.get();
    if (ch == 10) ali++; 
  };
  file2.put(10);
  ch=file1.get();
  while ((ch != 10)&&(!file1.eof())) ch=file1.get();
  ch=file1.get();
  while(!file1.eof()){
    file2.put(ch);
    ch = file1.get();
  };
}

  //removes adjacent repetitious lines (in other words, in an alphabetized file)
  //also removes bogus line feeds
  //! works only for lines <= 15 bytes; it ignores rep.'s longer than that
void removeRepetitions(char *name1, char *name2){
  fstream file1, file2;
  file1.open(name1, ios::in);
  file2.open(name2, ios::out);
  
  char *lineA=new char, *lineB=lineA, *lineC=new char, *lineD;
  char ch;
  
  ch=file1.get();
  while (ch == 10) ch=file1.get();
  while ((ch != 10)&&(!file1.eof())){
    *lineB=ch;
    lineB++;
    ch=file1.get(); 
  };
  *lineB='\0';
  
  lineB=lineA;
  ch=*lineB;
  while (ch != '\0'){
    file2.put(ch);
    lineB++;
    ch=*lineB;
  };
  
  ch=file1.get();
  while(ch == 10) ch=file1.get();
  
  int stop=0;
  while(!stop){
    lineB=lineC;
    while((ch != 10)&&(!file1.eof())){
      *lineB=ch;
      lineB++;
      ch=file1.get();
    };
    *lineB='\0';
    if (strcmp(lineA, lineC) != 0){
      file2.put(10);
      lineB=lineC;
      ch=*lineB;
      while (ch != '\0'){
        file2.put(ch);
        lineB++;
        ch=*lineB;
      };
      lineD=lineA;      
      lineA=lineC;
      lineC=lineD;
    };
    ch=file1.get();
    while (ch == 10) ch=file1.get();
    if (file1.eof()) stop=1;
  };
}

void remSpaceF( char *name1, char *name2 ){
  char ch, ch2;

  fstream file1, file2;
  file1.open( name1, ios::in );
  file2.open( name2, ios::out );  

  ch = file1.get();
  while( !file1.eof() ){
    if (ch != 32) file2.put(ch);
    ch = file1.get();
  };
}

/*
void remString(){

}

void replace(){

}
*/

  //removes the unwanted garbage like imbedded nulls, etc.
  //keeps only line feeds and characters between 32 and 126
void stripJunk(char *name1, char *name2){
  fstream fileA, fileB;
  fileA.open(name1, ios::in);
  fileB.open(name2, ios::out);
  char ch=fileA.get();
  while(!fileA.eof()){
    if (ch == 10) fileB.put(ch);
    if ((ch > 31)&&(ch < 127)) fileB.put(ch);
    ch=fileA.get();
  };
}

void swapChar( char *name1, char *name2, char unwanted, char wanted){
  fstream file1, file2;
  file1.open(name1, ios::in);
  file2.open(name2, ios::out);
  char ch=file1.get();
  while(!file1.eof()){
    if (ch == unwanted) file2.put(wanted);
    else file2.put(ch);
  };
}

void switchCases( char *name1, char *name2 ){
  char ch, ch2;

  fstream file1, file2;
  file1.open( name1, ios::in );
  file2.open( name2, ios::out );
  
  ch = file1.get();
  while( !file1.eof() ){
    if (isupper(ch)) ch2=tolower(ch);
    if (islower(ch)) ch2=toupper(ch);
    file2.put(ch2);
    ch = file1.get();
  }; 
}

  //takes the first n (dellines) characters off of the beginning of each line
void trimFront( char *name1, char *name2, int dellines){
  fstream file1, file2;
  file1.open(name1, ios::in);
  file2.open(name2, ios::out);
  
  char ch;
  int ii, tooshort=0;
  
  while (!file1.eof()){
    tooshort=0;
    for (ii=1; ii <= dellines; ii++){
      ch=file1.get();
      if (ch == 10){ tooshort=1; ii=dellines+1; };
    };
    if (!tooshort){
      ch=file1.get();
      if (!file1.eof()) file2.put(ch);
      while ((ch != 10)&&(!file1.eof())){
        ch=file1.get();
        if (!file1.eof()) file2.put(ch);
      };
    };
  };
}

void uppercase( char *name1, char *name2 ){
  char ch, ch2;

  fstream file1, file2;
  file1.open( name1, ios::in );
  file2.open( name2, ios::out );
  
  ch = file1.get();
  while( !file1.eof() ){
    ch2=toupper(ch);
    file2.put(ch2);
    ch = file1.get();
  }; 
}

//output functions of more than one file====================================

  //(adds in a line feed between the files)
void addFiles( char *name1, char *name2, char *name3 ){
  char ch;

  fstream file1, file2, file3;
  file1.open( name1, ios::in );
  file2.open( name2, ios::in );
  file3.open( name3, ios::out );  

  ch = file1.get();
  while( !file1.eof() ){
    file3.put(ch);
    ch = file1.get();
  };
  file3.put(10);
  ch = file2.get();
  while ( !file2.eof() ){
    file3.put(ch);
    ch = file2.get();
  };
}

  //(adds in line feeds between the three files)
void addFiles3( char *name1, char *name2, char *name3, char *name4 ){
  char ch;

  fstream file1, file2, file3, file4;
  file1.open( name1, ios::in );
  file2.open( name2, ios::in );
  file3.open( name3, ios::in );
  file4.open( name4, ios::out );  

  ch = file1.get();
  while( !file1.eof() ){
    file4.put(ch);
    ch = file1.get();
  };
  file4.put(10);
  ch = file2.get();
  while ( !file2.eof() ){
    file4.put(ch);
    ch = file2.get();
  };
  file4.put(10);
  ch = file3.get();
  while ( !file3.eof() ){
    file4.put(ch);
    ch = file3.get();
  };
}

  //takes any two files and makes one alphabetical file out of them
void alphabetizeAndMerge(char *name1, char *name2, char *name3){
  addFiles(name1, name2, "mAb.tmp");
  alphabetize("mAb.tmp", "mAbB.tmp");
  rename("mAn.tmp", name3);
}

  //file 1 is inserted into file 2 at the position ifp
void insertFile( char *name1, char *name2, char *name3, int ifp ){
  char ch;

  fstream file1, file2, file3;
  file1.open( name1, ios::in );
  file2.open( name2, ios::in );
  file3.open( name3, ios::out );  

  int ifi=0;
  ch = file2.get();
  while(( !file2.eof() )&&( ifi < ifp )){
    file3.put(ch);
    ch = file2.get();
  };
  ch = file1.get();
  while ( !file1.eof() ){
    file3.put(ch);
    ch = file2.get();
  };
  while ( !file2.eof() ){
    file3.put(ch);
    ch = file2.get();
  };
}

 //meant to take two already alphabetized files and merge them alphabetically
void mergeAlphabetically(char *name1, char *name2, char *name3){
  fstream file1, file2, fileA;
  file1.open(name1, ios::in);
  file2.open(name2, ios::in);
  fileA.open(name3, ios::out);

  int llen1=countLines(name1);
  int llen2=countLines(name2);
   
  char *line1 = new char, *line2 = new char;

  char *linet1=line1, *linet2=line2;
  
  char ch;
  
  ch=file1.get();  
  while((ch != 10)&&(!file1.eof())){
    *linet1=ch;
    ch=file1.get();
    linet1++;
  };
  *linet1='\0';
    
  ch=file2.get();
  while((ch != 10)&&(!file2.eof())){
    *linet2=ch;
    ch=file2.get();
    linet2++;
  };
  *linet2='\0';
  
  cout << endl << line1 << " " << line2 << endl;
  
  int whowon=0;
  int spam=strcmp(line1, line2);
  if (!spam) whowon=0;
  if (spam < 0) whowon=1;
  if (spam > 0) whowon=2;
  
  int firsttime=1;
  
  int lost1=0, lost2=0;
  if (file1.eof()) lost1=1;
  if (file2.eof()) lost2=1;

  //Y(1);
  int stop=0;
  while(!stop){
    if (whowon == 1){
      //Y(2);
      if (!firsttime) fileA.put(10);
      firsttime=0;
      linet1=line1;
      ch=*linet1;
      while(ch != '\0'){
        fileA.put(ch);
        linet1++;
        ch=*linet1;
      };
      
      if (file1.eof()) lost1=1;

      ch=file1.get();
      linet1=line1;
      while((ch != 10)&&(!file1.eof())){
        *linet1=ch;
        ch=file1.get();
        linet1++;
      };
      *linet1='\0';
      //Y(3);
    } else {
      //Y(4);
      if (!firsttime) fileA.put(10);
      firsttime=0;
      linet2=line2;
      ch=*linet2;
      while(ch != '\0'){
        fileA.put(ch);
        linet2++;
        ch=*linet2;
      };
      
      if (file2.eof()) lost2=1;

      ch=file2.get();
      linet2=line2;
      while((ch != 10)&&(!file2.eof())){
        *linet2=ch;
        ch=file2.get();
        linet2++;
      };
      *linet2='\0';
      //Y(5);
    };
    
    if (file1.eof()) lost1=1;
    if (file2.eof()) lost2=1;
    
    if ((!lost1)&&(!lost2)){
      spam=strcmp(line1, line2);
      if (!spam) whowon=2;       //default goes to 2
      if (spam < 0) whowon=1;
      if (spam > 0) whowon=2; 
    };
    //Y(6);
    if ((!lost1)&&(lost2)) whowon=1;
    if ((!lost2)&&(lost1)) whowon=2;
    if ((lost2)&&(lost1)) stop=1;      
  }; 
}