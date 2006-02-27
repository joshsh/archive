//string and character functions
//charSetup must be run once before using charStr()

#include <ctype.h>

char *Ch1, *Ch2;  //for general low-level use
char *ChA, *ChB;  //reserved strings for charStr() and digitStr() !

//preliminary declarations-------------------------------------------------

  char *addStrings(char *, char *);
  void charSetup();
  char *charStr(char);
  int countCharS(char *, char);
  char *cutLastCharS(char *);
  char *digitStr(int);
  int endsWith(char *, char *);
  int findStrS(char *, char *);
  char intChar(int);
  char *intStr(int);
  char *invertS(char *);
  int isAlph(char);
  int isDigit(char);
  int isEndPunct(char);
  int isNumber(char);
  int isNumStr(char *);
  int isSymmetricS(char *);
  char lastCharS(char *);
  char *lowercaseS(char *);
  char *numStr(double, int);
  char *remCharS(char *);
  char *remNonAlphS(char *);
  //char *remStrS(char *, char *);
  //char *remWord(char *, char *);
  char *stem(char *);
  int strInt(char *);
  int strnCmp(char *, char *);
  double strNum(char *);
  char *swapCharS(char *);
  char *switchCasesS(char *);
  char *takeAfterCharExc(char *, char);
  char *takeAfterCharInc(char *, char);
  char *takeToCharExc(char *, char);
  char *takeToCharInc(char *, char);
  char *upperCaseS(char *);
  
//-------------------------------------------------------------------------

  //appends stringB to stringA
char *addStrings( char *stringA, char *stringB){
  char *Ch2=new char, *Ch=Ch2;
  while (*stringA != '\0'){ *Ch=*stringA; Ch++; stringA++; };
  while (*stringB != '\0'){ *Ch=*stringB; Ch++; stringB++; };
  *Ch='\0';
  return Ch2;
}

void charSetup(){
  ChA=new char;
  ChB=ChA;
  ChB++;
  *ChB='\0';
}

  //converts a character to a one-byte string
  //charSetup must be run once first
char *charStr(char ch){
  *ChA=ch;
  return ChA;
}

int countCharS( char *thisStr, char ch1 ){
  Ch1=thisStr;
  int count=0;
  while (*Ch1 != '\0'){
    if (*Ch1 == ch1) count++;
    Ch1++;
  };
  return count;
}

  //removes the last character from a string
char *cutLastCharS( char *cutme ){
  char *imcut=new char, *imcut2=imcut;
  char ch=*cutme, ch2=0, ch3=0;
  int cli=0;
  while (ch != '\0'){
    cli++;
    ch3=ch2;
    ch2=ch;
    cutme++;
    ch=*cutme;
    if (cli>1){ *imcut2=ch3; imcut2++; };
  };
  *imcut2='\0';
  return imcut;
}

  //converts a natural number into a one-digit string
  //requires charSetup()
char *digitStr(int dig){
  if ((dig<0)||(dig>9)) return "";
  *ChA=dig+48;
  return ChA;
}

  //tells whether a string has a certain ending (e.g. "this.doc", ".doc" -> 1)
int endsWith(char *char1, char *charEnd){
  char *ChX=new char, *ChY=new char; ChX=char1; ChY=charEnd;
  int ew1=strlen(char1), ew2=strlen(charEnd);
  if (ew2 > ew1) return 0;
  for (int ew3=0; ew3 < (ew1-ew2); ew3++) ChX++;
  for (int ew3=0; ew3 < ew2; ew3++){
    if (*ChX != *ChY) return 0;
    ChX++; ChY++;
  };
  return 1;
}

  //returns the position of the start of the first occurence of the string
  //stringB in stringA, with the first character as 0.
  //returns -1 if stringB does not appear in stringA
int findStrS(char *stringA, char *stringB){
  char *stringA2, *stringB2;
  int ii, jj=0, found, thislen=strlen(stringB);
  while (*stringA != '\0'){
    if (*stringA == *stringB){
      stringA2=stringA, stringB2=stringB;
      found=1;
      for (ii=0; ii<thislen; ii++){
        if (*stringA2 == '\0') return -1;
        if (*stringA2 != *stringB2){ ii=strlen(stringB); found=0; };
        stringA2++;
        stringB2++;
      };
      if (found) return jj;  
    };
    stringA++;
    jj++;
  };
  return -1;  
}

  //returns the character representing a natural number < 10
char intChar(int digit){
  return digit+48;
}

  //returns the string representation of an integer
char *intStr(int number){
  if (!number) return "0";
  char *ChA=new char;
  char *Ch=ChA;
  int power=0, tens=10;
  int boredom;
  if (number < 0){ *Ch='-'; Ch++; number=-number; };
  while (number >= tens){ tens=tens*10; power++; };
  tens=tens/10;
  for (int ii=0; ii<=power; ii++){
    boredom=number/tens;
    *Ch=boredom+48;
    Ch++;
    number=number-(boredom*tens);
    tens=tens/10;
  };
  *Ch='\0';
  return ChA; 
}

char *invertS(char *stringX){
  int length=strlen(stringX)-1;
  char *stringY;
  char *target=new char, *target2=target;
  int ii, jj;
  for (ii=0; ii<= length; ii++){
    stringY=stringX;
    for (jj=1; jj <= length-ii; jj++) stringY++;
    *target2=*stringY;
    target2++;
  };
  *target2='\0';
  return target;
}

int isAlphabetic( char ch ){
  return (((ch>64)&&(ch<91))||((ch>96)&&(ch<123)));
}

  //tells whether a character is a numerical digit or not
int isDigit( char isdig ){
  return ((isdig>47)&&(isdig<58));
}

  //tells whether a character is end punctuation (. ! ?) or not
int isEndPunct( char ispun ){
  if ((ispun == 33)||(ispun == 46)||(ispun == 63)) return 1;
  return 0;
}

  //considers '.' and '-' as numbers as well as the numerical digits.
int isNumber( char isnum ){
  if (isnum == 46) return 1;
  if (isnum == 45) return 1;
  return ((isnum > 47)&&(isnum<58));
}

  //tells whether a string represents a proper number or not
int isNumStr( char *isnum ){
  char *string1=isnum;
  int madness=0, periods=0;
  while (*string1 != '\0'){
    if (!isNumber(*string1)) return 0;
    if ((*string1 == '-')&&(madness)) return 0;
    if (*string1 == '.'){
      periods++;
      if (periods > 1) return 0;
    };
    madness=1;
    string1++;
  };
  return 1;
}

int isSymmetricS(char *stringS){
  return strnCmp(stringS, invertS(stringS));
}

  //returns the last character of a string
char lastCharS( char *string1 ){
  char ch=*string1, ch2=0;
  while (ch != '\0'){ ch2=ch; string1++; ch=*string1; };
  return ch2;
}

  //converts a string to lowercase
char *lowercaseS( char *stringA ){
  char *stringB=new char;
  char *stringC=stringB;
  char ch=*stringA;
  while (ch != '\0'){
    *stringC=tolower(ch);
    stringC++;
    stringA++;
    ch=*stringA;
  };
  *stringC='\0';
  return stringB;
}


  //returns the string representation of a number to #digits# decimal places
char *numStr(double number, int digits){
  if (!number) return "0";
  int intnum=int(number);
  char *ChB=intStr(intnum);
  if(intnum == number) return ChB;
  int tens=1;
  for (int ii=0; ii<digits; ii++) tens=tens*10;
  char *ChC=addStrings(ChB, ".");
  return addStrings(ChB, intStr(int((number-intnum)*tens)));
}

  //cut out char ch1 wherever it appears in stringA
char *remCharS( char *stringA, char ch1 ){
  char *stringB=new char;
  char *stringC=stringB;
  char ch=*stringA;
  while(ch != '\0'){
    if (ch != ch1){
      *stringC=ch;
      stringC++;
    };
    stringA++;
    ch=*stringA;
  };
  *stringC='\0';
  return stringB;
}

  //remove all non-alphabet characters from stringA
char *remNonAlphS( char *stringA ){
  char *stringB=new char;
  char *stringC=stringB;
  char ch=*stringA;
  while(ch != '\0'){
    if (((ch>64)&&(ch<91))||((ch>96)&&(ch<123))){
      *stringC=ch;
      stringC++;
    };
    stringA++;
    ch=*stringA;
  };
  *stringC='\0';
  return stringB;
}

/*
  //removes a string from another wherever it appears
char *remStr( char *string, char *unwanted){

}*/

/*
  //removes a word from a string wherever it appears
char *remWordS( char *string1, char *word){
  int rwi, rwj;
  int stop=1, caught;
  int wordlen=strlen(word);
  char *string2 = new char;
  rwi=0;
  int stop2=0;
  while (string1[rwi] != '\0'){ 
    rwj=0;
    caught=0;
    while (!stop){
      if (string1[rwi+rwj] != word[rwj]) stop=1;
      if ((jrw==(wordlen-1))&&(!stop)){
        caught=1;
        stop=1;
      }; 
      rwj++;
      if (string1[rwi+rwj] == '\0'){ stop=1; stop2=1;};
    }; 
    if (stop2) return string2;   
    if (caught){
      reallycaught=0;
      if ((!rwi)&&
      rwi=rwi+rwj;
    };
    rwi++
  };
  return string2;
}
*/

  //returns all of a string before the first period (e.g. "this.doc" -> "this")
char *stem(char *name1){
  char *name2=name1;
  char *string1 = new char, *string2=string1;
  while((*name2 != '\0')&&(*name2 != '.')){ *string2=*name2; name2++; string2++;};
  *string2='\0';
  return string1; 
}

  //assumes that the string is a proper integer, consisting of nothing but
  //digits and possibly a minus sign as the first character.
int strInt( char *stringX ){
  char *numstring=stringX;
  int negative=0;
  if (*numstring == '-'){ negative=1; numstring++; };
  //numstring = reverseS(numstring);
  int sum=0, tens=1;
  for(int ii=1; ii<strlen(numstring); ii++) tens=tens*10; 
  while (*numstring != '\0'){
    sum=sum+(tens*(*numstring-48));
    tens=tens/10;
    numstring++;
  };
  if (negative) return -sum;
  return sum;   
}

  //returns 1 if they are the same string, 0 if they aren't
int strnCmp( char *string1, char *string2){
  int sci=strlen(string1);
  int scj=strlen(string2);
  if (sci != scj) return 0;
  for (scj=0; scj<sci; scj++){
    if (string1[scj] != string2[scj]) return 0;
  };
  return 1;
}

  //assumes that the string is a proper number
  //is still "weird"; doesn't give the exact value, but rounds it somehow
double strNum(char *stringX){
  char *numstring=stringX;
  int negative=0;
  if (*numstring == '-'){ negative=1; numstring++; };
  double sum=strInt(takeToCharExc(numstring, '.'));
  cout << "sum = " << sum << endl;
  char *therest=takeAfterCharExc(numstring, '.');
  if (strlen(therest) < 2){
    if (negative) return -sum;
    else return sum;
  };
  cout << "therest = " << therest << endl;
  double tens=1;
  for (int ii=1; ii <=  strlen(therest); ii++) tens=tens*10;
  cout << "tens = " << tens << endl;
  sum=sum+(strInt(therest)/(tens*1.0));
  if (negative) return -sum;
  return sum;
}

  //swap ch2 for ch1 wherever ch1 appears in stringA
char *swapCharS( char *stringA, char ch1, char ch2 ){
  char *stringB=stringA;
  char ch=*stringB;
  while(ch != '\0'){
    if (ch == ch1) *stringB=ch2;
    stringB++;
    ch=*stringB;
  }; 
  return stringA;
}

  //switches the cases of a string
char *switchCasesS( char *stringA ){
  char *stringB=new char;
  char *stringC=stringB;
  char ch=*stringA;
  while (ch != '\0'){
    *stringC=ch;
    if (islower(ch)) *stringC=toupper(ch);
    if (isupper(ch)) *stringC=tolower(ch);
    stringC++;
    stringA++;
    ch=*stringA;
  };
  *stringC='\0';
  return stringB;
}

  //returns the string occuring after the first appearance of char chA 
char *takeAfterCharExc( char *stringA, char chA ){
  char *returnstr=new char, *Ch1=returnstr;
  while ((*stringA != '\0')&&(*stringA != chA)) stringA++;
  if (*stringA != '\0') stringA++;
  while (*stringA != '\0'){
    *Ch1=*stringA;
    Ch1++;
    stringA++;
  };
  *Ch1='\0';
  return returnstr;
}

  //returns the string occuring after the first appearance of char chA, 
  //including chA
char *takeAfterCharInc( char *stringA, char chA ){
  char *returnstr=new char, *Ch1=returnstr;
  while ((*stringA != '\0')&&(*stringA != chA)) stringA++;
  while (*stringA != '\0'){
    *Ch1=*stringA;
    Ch1++;
    stringA++;
  };
  *Ch1='\0';
  return returnstr;
}

  //returns the string occuring before the first appearance of char chA
char *takeToCharExc( char *stringA, char chA){
  char *returnstr=new char, *Ch1=returnstr;
  while (*stringA != '\0'){
    if (*stringA == chA){ *Ch1='\0'; return returnstr; };
    *Ch1=*stringA;
    Ch1++;
    stringA++;
  };
  return "";
}

  //returns the string occuring before the first appearance of char chA, 
  //including chA
char *takeToCharInc( char *stringA, char chA){
  char *returnstr=new char, *Ch1=returnstr;
  while (*stringA != '\0'){
    if (*stringA == chA){ *Ch1=chA; Ch1++; *Ch1='\0'; return returnstr; };
    *Ch1=*stringA;
    Ch1++;
    stringA++;
  };
  return "";
}

  //converts a string to uppercase
char *uppercaseS( char *stringA ){
  char *stringB=new char;
  char *stringC=stringB;
  char ch=*stringA;
  while (ch != '\0'){
    *stringC=toupper(ch);
    stringC++;
    stringA++;
    ch=*stringA;
  };
  *stringC='\0';
  return stringB;
}