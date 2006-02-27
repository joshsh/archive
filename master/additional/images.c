#include <stdio.h>
#include <iostream.h>
#include "/home/chaos/files/char.h"
#include <math.h>

//type declarations=========================================================

struct int3{
  int a, b, c;
};

//intfield-----------------------------------------------------------------

struct intnode{
  int val;
  intnode *next;
};

struct intline{
  intnode *first;
  intline *next;
};

struct intfield{
  int length, height;
  int wraparound;
  intline *first;
  int i, j;
  intline *ati;
  intnode *atj;
};

//image256-----------------------------------------------------------------

struct pixel256{
  int red, green, blue;  //range from 0 to 255
  pixel256 *next;
};

struct line256{
  pixel256 *first;
  line256 *next;
};

struct image256{      //image has 256^3 possible colors
  int length, height;
  int index;         //to keep track of the individual images
  int colors;
  int wraparound;    //(0 or 1)
  int updated;       //every edit which does not keep track of how many
  line256 *first;    //  colors the image has should set 'updated' to 0.
  int i, j;          //store the integer position of the current pointer.
  line256 *ati;      //the current pointer
  pixel256 *atj;     //  itself.
};

//blake (grayscale, stored as double)--------------------------------------

struct pixelBlake{
  double shade;   //shade ranges from 0 to 1
  pixelBlake *next;
};

struct lineBlake{
  pixelBlake *first;
  lineBlake *next;
};

struct blake{
  int length, height;
  lineBlake *first;
  int i, j;
  int wraparound;   //(0 or 1)
  lineBlake *ati;
  pixelBlake *atj;
};

  //must be run exactly once before XPM's and/or image256's are to be used.  
void init_images_h(){
  color256List->next=NULL;
}

//intfield functions########################################################

intfield *newIntField(int length, int height, int val, int wraparound){
  if (height < 1) height=1;
  if (length < 1) length=1;
  if (wraparound != 0) wraparound=1;
  intfield *wrigley=new intfield;
  wrigley->length=length;
  wrigley->height=height;
  wrigley->wraparound=wraparound;

  int i, j;
  intline *linerunner2=new intline, *linerunner;
  intnode *pixrunner2=new intnode, *pixrunner;
  toreturn->first=linerunner2;
  toreturn->first->first=pixrunner2;
  linerunner=toreturn->first;
  pixrunner=linerunner->first;
  for (i=0; i<height; i++){
    for (j=0; j<length; j++){
      pixrunner->val=val;
      if (j == length-1){
        if (!wraparound) pixrunner->next=NULL;
        else pixrunner->next=linerunner->first;
      }else{
        pixrunner2=new intnode;
        pixrunner->next=pixrunner2;
        pixrunner=pixrunner2;
      };
    };
    if (i == height-1){
      if (!wraparound) linerunner->next=NULL;
      else linerunner->next=toreturn->first;
    }else{
      linerunner2=new intline;
      linerunner->next = linerunner2;
      linerunner=linerunner2;
      pixrunner2=new intnode;
      linerunner->first=pixrunner2;
      pixrunner=linerunner->first;
    };
  };

  wrigley->i=0; wrigley->j=0;
  wrigley->ati=wrigley->first;
  wrigley->atj=wrigley->first->first;
  return wrigley;
}

  //caution: ALWAYS chooses a pixel, which is modulated EVEN IF this is
  //not a wraparound image
intnode *at(intfield *ofdreams, int i, int j){
  while (i<0) i=i+ofdreams->height;
  while (j<0) j=j+ofdreams->length;
  while (i > ofdreams->height) i= i - ofdreams->height;
  while (j > ofdreams->length) j= j - ofdreams->length; 
  intline *linerunner;
  intnode *pixrunner;
  int k;
  if (i == ofdreams->i){
    if (j == ofdreams->j) return ofdreams->atj;
    else{
      linerunner=ofdreams->ati;
      if (j >= ofdreams->j){
        pixrunner=ofdreams->atj;
        for (k=0; k < j-ofdreams->j; k++) pixrunner=pixrunner->next;
        ofdreams->j=j;
        ofdreams->atj=pixrunner;
        return pixrunner;
      }else{
        pixrunner=linerunner->first;
        for (k=0; k<j; k++) pixrunner=pixrunner->next;
        ofdreams->j=j;
        ofdreams->atj=pixrunner;
        return pixrunner;
      };
    };
  }else{
    if (i >= ofdreams->i){
      linerunner=ofdreams->ati;
      for (k=0; k < i-ofdreams->i; i++) linerunner=linerunner->next;
      pixrunner=linerunner->first;
      for (k=0; k<j; k++) pixrunner=pixrunner->next;
      ofdreams->i=i;
      ofdreams->j=j;
      ofdreams->ati=linerunner;
      ofdreams->atj=pixrunner;
      return pixrunner;      
    }else{
      linerunner=ofdreams->first;
      for (k=0; k<i; k++) linerunner=linerunner->next;
      pixrunner=linerunner->first;
      for (k=0; k<j; k++) pixrunner=pixrunner->next;
      ofdreams->i=i;
      ofdreams->j=j;
      ofdreams->ati=linerunner;
      ofdreams->atj=pixrunner;
      return pixrunner;
    };
  };
}

void binary_to_XPM(intfield *cadden, char *namestub){
  FILE *file1=fopen(addStrings(namestub, ".xpm"), "w");
  fputs("/* XPM */", file1);
  fputc(10, file1);
  fputs("static char * ", file1);
  fputs(namestub, file1);
  fputs("_xpm[] = {", file1);
  fputc(10, file1);
  fputc(34, file1);
  fputs(intStr(cadden->length), file1);
  fputc(32, file1);
  fputs(intStr(cadden->height), file1);
  fputc(32, file1);
  fputc('2', file1);
  fputc(32, file1);
  fputc('1', file1);
  fputc(34, file1); fputc(',', file1); fputc(10, file1);
  fputc(34, file1); fputs("  c #000000000000", file1);
  fputc(34, file1); fputc(',', file1); fputc(10, file1);
  fputc(34, file1); fputc(',', file1); fputc(10, file1);
  fputc(34, file1); fputs(". c #FFFFFFFFFFFF", file1);
  fputc(34, file1); fputc(',', file1); fputc(10, file1);	
  int i, j;
  int length=cadden->length;
  int height=cadden->height;
  intline *linerunner=cadden->first;
  intnode *pixrunner=linerunner->first;
  int m;
  for (i=0; i<height; i++){
    fputc(34, file1);
    for (j=0; j<length; j++){
      if (!pixrunner->val) fputc(32, file1);
      else fputc('.', file1);
    };
    fputc(34, file1);
    if (i < height-1){
      fputc(',', file1); fputc(10, file1);
      linerunner=linerunner->next;
      pixrunner=linerunner->first;
    };
  };
  fputs("};", file1);
  fputc(10, file1);
}

//image256 functions########################################################

char XPMList[93]=" .XoO+@#$%&*=-;:>,<1234567890qwertyuipasdfghjklzxcvbnmMNBVCZASDFGHJKLPIUYTREWQ!~^/()_`'][{}|";
//int XPMList_made=0;
pixel256 *color256List=new pixel256;
int color256List_started=0;
int index256=0;
int current256=0;

  //creates a new image256 and gives it a uniform color
image256 *newImage256(int length, int height, int red, int green, int blue, int wraparound){
  if (red > 255) red=255;
  if (green > 255) green=255;
  if (blue > 255) blue=255;
  if (red < 0) red=0;
  if (green < 0) green=0;
  if (blue < 0) blue=0;
  if (wraparound != 0) wraparound=1;
  image256 *toreturn = new image256;
  if (length < 1) length=1;
  if (height < 1) height=1;
  int i, j;
  line256 *linerunner2=new line256, *linerunner;
  pixel256 *pixrunner2=new pixel256, *pixrunner;
  toreturn->first=linerunner2;
  toreturn->first->first=pixrunner2;
  linerunner=toreturn->first;
  pixrunner=linerunner->first;
  for (i=0; i<height; i++){
    for (j=0; j<length; j++){
      pixrunner->red=red;
      pixrunner->green=green;
      pixrunner->blue=blue;
      if (j == length-1){
        if (!wraparound) pixrunner->next=NULL;
        else pixrunner->next=linerunner->first;
      }else{
        pixrunner2=new pixel256;
        pixrunner->next=pixrunner2;
        pixrunner=pixrunner2;
      };
    };
    if (i == height-1){
      if (!wraparound) linerunner->next=NULL;
      else linerunner->next=toreturn->first;
    }else{
      linerunner2=new line256;
      linerunner->next = linerunner2;
      linerunner=linerunner2;
      pixrunner2=new pixel256;
      linerunner->first=pixrunner2;
      pixrunner=linerunner->first;
    };
  };
  toreturn->height=height;
  toreturn->length=length;
  toreturn->i=0;
  toreturn->j=0;
  toreturn->ati=toreturn->first;
  toreturn->atj=toreturn->ati->first;
  toreturn->colors=1;
  toreturn->updated=1;
  toreturn->wraparound=wraparound;
  toreturn->index=index256;
  index256++;
  return toreturn;
}

  //keeps the first node and caps it off, destroys all the rest
void kill_pixel256(pixel256 *thisone){
  if (thisone->next == NULL) return;
  if (thisone->next->next == NULL){
    delete thisone->next;
    thisone->next = NULL;
    return;
  };
  pixel256 *third=thisone->next, *second=third->next;
  while (second->next != NULL){
    delete third;
    third=second;
    second=second->next;
  };
  delete third;
  delete second;
  thisone->next=NULL;
}

  //counts the total number of distinct colors in an image256
  //and lists them (for encoding as an XPM file).
int countColors256(image256 *roan){
  kill_pixel256(color256List);
  pixel256 *sitter = new pixel256;
  pixel256 *prunner, *prunner2;
  int length=roan->length;
  int height=roan->height;
  int numcolors=0;
  int red, green, blue;
  line256 *linerunner=roan->first;
  pixel256 *pixrunner=linerunner->first;
  int i, j, k, found;
  for (i=0; i<height; i++){
    for (j=0; j<length; j++){
      red=pixrunner->red;
      green=pixrunner->green;
      blue=pixrunner->blue;
      prunner=sitter;
      found=0;
      for (k=0; k<numcolors; k++){
        if ((red == prunner->red)&&(green == prunner->green)&&(blue == prunner->green)){ found = 1; k=numcolors; };
        if (k < numcolors-1) prunner=prunner->next;
      };
      if (!found){
        while (prunner->next != NULL) prunner=prunner->next;
        prunner2=new pixel256;
        prunner2->red=red;
        prunner2->green=green;
        prunner2->blue=blue;
        prunner2->next=NULL;
        if (!numcolors) sitter=prunner2;
        else prunner->next=prunner2;
        numcolors++;
      };
      if (j < length-1) pixrunner=pixrunner->next;
    };
    if (i < height-1){
      linerunner=linerunner->next;
      pixrunner=linerunner->first;
    };
  };
  color256List=sitter;
  roan->updated=1;
  current256=roan->index;
  return numcolors;
}

int findColor256(int red, int green, int blue){
  pixel256 *pixrunner=color256List;
  if ((pixrunner->red == red)&&(pixrunner->green == green)&&(pixrunner->blue == blue)) return 0;
  int i=0;
  while (pixrunner->next != NULL){
    pixrunner=pixrunner->next;
    i++;
    if ((pixrunner->red == red)&&(pixrunner->green == green)&&(pixrunner->blue == blue)) return i;    
  };
  return -1;
}

  //returns black as default color
  //you might want to give this a 'bookmark' pointer to speed things up.
int3 getColor256(int indexer){
  int3 nuller;
  nuller.a=0; nuller.b=0; nuller.c=0;
  if (indexer < 0) return nuller;
  pixel256 *pixrunner=color256List;
  for (int i=0; i<indexer; i++){
    if (pixrunner->next == NULL) return nuller;
    pixrunner=pixrunner->next;
  };
  nuller.a=pixrunner->red;
  nuller.b=pixrunner->green;
  nuller.c=pixrunner->blue;
  return nuller;
}

int findIndex256(char ch1){
  for (int i=0; i<92; i++){
    if (XPMList[i] == ch1) return i;
  };
  return -1;
}

  //very little error tolerance
image256 *loadXPM(char *filestump, int wraparound){
  kill_pixel256(color256List);
  if (!color256List_started) color256List->next=NULL;
  FILE *file1=fopen(addStrings(filestump, ".xpm"), "r");
  char ch=fgetc(file1);
  while (ch != '{') ch=fgetc(file1);
  while (ch != 34) ch=fgetc(file1);
  while ((ch == 32)||(ch == 34)) ch=fgetc(file1);
  char *stringer=new char[9], *stringer2=stringer;
  while (ch != 32){ *stringer2=ch; stringer2++; ch=fgetc(file1); };
  *stringer2='\0';
  int length=strInt(stringer);
  while (ch == 32) ch=fgetc(file1);
  stringer2=stringer;
  while (ch != 32){ *stringer2=ch; stringer2++; ch=fgetc(file1); };
  *stringer2='\0';
  int height=strInt(stringer);
  while (ch == 32) ch=fgetc(file1);
  stringer2=stringer;
  while (ch != 32){ *stringer2=ch; stringer2++; ch=fgetc(file1); };
  *stringer2='\0';
  int numcolors=strInt(stringer);
  while (ch == 32) ch=fgetc(file1);
  stringer2=stringer;
  while ((ch != 32)&&(ch != 34)){ 
    *stringer2=ch; 
    stringer2++;
    ch=fgetc(file1);
  };
  while (ch != 10) ch=fgetc(file1);
  *stringer2='\0';
  int leng=strInt(stringer);
  image256 *thisone = newImage256(length, height, 0, 0, 0, wraparound);
  thisone->colors=numcolors;
  thisone->updated=1;
  //while (ch != 34) ch=fgetc(file1);
  //ch=fgetc(file1);
  //while (ch != 34) ch=fgetc(file1);
  //ch=fgetc(file1);
  int i, j, k;
  pixel256 *pixrunner=color256List, *pixrunner2;
  for (i=0; i<numcolors; i++){
    while (ch != '	') ch=fgetc(file1);
    while (ch != '#') ch=fgetc(file1);
    ch=fgetc(file1);
    stringer2=stringer;
    *stringer2=ch;
    stringer2++;
    ch=fgetc(file1);
    *stringer2=ch;
    stringer2++;
    *stringer2='\0';
    ch=fgetc(file1); ch=fgetc(file1);
    pixrunner->red=strInt(stringer, 16);
    stringer2=stringer;
    ch=fgetc(file1);
    *stringer2=ch;
    stringer2++;
    ch=fgetc(file1);
    *stringer2=ch;
    stringer2++;
    *stringer2='\0';
    ch=fgetc(file1); ch=fgetc(file1);
    pixrunner->green=strInt(stringer, 16);
    stringer2=stringer;
    ch=fgetc(file1);
    *stringer2=ch;
    stringer2++;
    ch=fgetc(file1);
    *stringer2=ch;
    stringer2++;
    *stringer2='\0';
    ch=fgetc(file1); ch=fgetc(file1);
    pixrunner->blue=strInt(stringer, 16);

    if (i < numcolors-1){
      if (pixrunner->next != NULL) pixrunner=pixrunner->next;
      else{
        pixrunner2=new pixel256;
        pixrunner2->next=NULL;
        pixrunner->next=pixrunner2;
        pixrunner=pixrunner2;
      };
    };
  };
  pixrunner->next = NULL;
  while (ch != 10) ch=fgetc(file1);

  int3 coltrio;
  int number, tens;
  line256 *lrunner=thisone->first;
  pixrunner=lrunner->first;
  for (i=0; i<height; i++){
    while (ch != 34) ch=fgetc(file1);
    ch=fgetc(file1);
    for (j=0; j<length; j++){
      stringer2=stringer;
      for (k=0; k<leng; k++){
        *stringer2=ch;
        stringer2++;
        ch=fgetc(file1);
      };
      *stringer2='\0';
      stringer2=stringer;
      number=0; tens=1;
      while (*stringer2 != '\0'){
        number=number+(tens*findIndex256(*stringer2));
        tens=tens*92;
        stringer2++;
      };

      coltrio=getColor256(number);
      pixrunner->red=coltrio.a;
      pixrunner->green=coltrio.b;
      pixrunner->blue=coltrio.c;
      //cout << pixrunner->red << " " << pixrunner->green << " ";
      //cout << pixrunner->blue << "|";
      if (j < length-1) pixrunner=pixrunner->next;
    };
    if (i < height-1){
      lrunner=lrunner->next;
      pixrunner=lrunner->first;
      while (ch != 10) ch=fgetc(file1);
    };
  };
  thisone->updated=1;
  current256=thisone->index;
  return thisone;
}

void saveXPM(image256 *cadden, char *namestub){
  //if (!XPMList_made) makeXPMList();
  FILE *file1=fopen(addStrings(namestub, ".xpm"), "w");
  fputs("/* XPM */", file1);
  fputc(10, file1);
  fputs("static char * ", file1);
  fputs(namestub, file1);
  fputs("_xpm[] = {", file1);
  fputc(10, file1);
  fputc(34, file1);
  fputs(intStr(cadden->length), file1);
  fputc(32, file1);
  fputs(intStr(cadden->height), file1);
  fputc(32, file1);
  if ((!cadden->updated)||(current256 != cadden->index)){
    cadden->colors=countColors256(cadden);
    cadden->updated=1;
  };
  int numcolors=cadden->colors;
  fputs(intStr(numcolors), file1);
  fputc(32, file1);
  int topcolors=92, leng=1;
  while (numcolors > topcolors){
    leng++;
    topcolors=topcolors*92;
  };
  fputs(intStr(leng), file1);
  fputc(34, file1); fputc(',', file1); fputc(10, file1);
  int i, j, k, l, tens, number;
  pixel256 *pixrunner=color256List;
  char *thinger=new char[leng+1], *thinger2;
  for (i=0; i<numcolors; i++){
    fputc(34, file1);
    tens=1;
    number=i;
    thinger2=thinger;
    for (j=1; j < leng; j++) tens=tens*92;
    //while (tens < number) tens=tens*92; };
    for (k=0; k<leng; k++){
      l=number/tens;
      number=number-(l*tens);
      //fputc(XPMList[l], file1);
      *thinger2=XPMList[l];
      thinger2++;
      tens=tens/92;
    };
    *thinger2='\0';
    fputs(invertS(thinger), file1);
    //fputs(thinger, file1);
    //for (k=0; k<=leng-j; k++) fputc(32, file1);
    fputc(32, file1);
    fputs("	c #", file1);
    thinger=intStr(pixrunner->red, 16);
    if (strlen(thinger) < 2) thinger=addStrings("0", thinger);
    fputs(thinger, file1); fputs(thinger, file1);
    thinger=intStr(pixrunner->green, 16);
    if (strlen(thinger) < 2) thinger=addStrings("0", thinger);
    fputs(thinger, file1); fputs(thinger, file1);
    thinger=intStr(pixrunner->blue, 16);
    if (strlen(thinger) < 2) thinger=addStrings("0", thinger);
    fputs(thinger, file1); fputs(thinger, file1);
    fputc(34, file1); fputc(',', file1); fputc(10, file1);
    if (i < numcolors-1) pixrunner=pixrunner->next;
  };
  int length=cadden->length;
  int height=cadden->height;
  line256 *linerunner=cadden->first;
  pixrunner=linerunner->first;
  int m;
  for (i=0; i<height; i++){
    fputc(34, file1);
    for (j=0; j<length; j++){
      tens=1; //m=1;
      //cout << pixrunner->red << " " << pixrunner->green << " ";
      //cout << pixrunner->blue << "|";
      number=findColor256(pixrunner->red, pixrunner->green, pixrunner->blue);
      //cout << number << " | ";
      //while (92*tens < number){ tens=tens*92; m++; };
      thinger2=thinger;
      for (m=1; m<leng; m++) tens=tens*92;
      for (k=0; k<leng; k++){
        l=number/tens;
        number=number-(l*tens);
        //fputc(XPMList[l], file1);
        *thinger2=XPMList[l];
        thinger2++;
        tens=tens/92;
      };
      *thinger2='\0';
      fputs(invertS(thinger), file1);
      if (j < length-1) pixrunner=pixrunner->next;
    };
    fputc(34, file1);
    if (i < height-1){
      fputc(',', file1); fputc(10, file1);
      linerunner=linerunner->next;
      pixrunner=linerunner->first;
    };
  };
  fputs("};", file1);
  fputc(10, file1);
}

void clearImage256(image256 *leander, int red, int green, int blue){
  if (red > 255) red=255;
  if (green > 255) green=255;
  if (blue > 255) blue=255;
  if (red < 0) red=0;
  if (green < 0) green=0;
  if (blue < 0) blue=0;
  int length=leander->length;
  int height=leander->height;
  line256 *linerunner=leander->first;
  pixel256 *pixrunner=linerunner->first;
  int i, j;
  for (i=0; i<height; i++){
    for (j=0; j<length; j++){
      pixrunner->red=red;
      pixrunner->green=green;
      pixrunner->blue=blue;
      if (j<99) pixrunner=pixrunner->next;
    };
    if (i<99){ linerunner=linerunner->next; pixrunner=linerunner->first; };
  };
}

  //thoroughly checks out an image256 to see if it has the proper form
  //returns 0 for a good image
int check_image256(image256 *midos){
  if (midos->length < 1) return 1;
  if (midos->height < 1) return 2;
  if (midos->i < 0) return 3;
  if (midos->i > midos->height - 1) return 4;
  if (midos->j < 0) return 5;
  if (midos->j > midos->length - 1) return 6;
  if (midos->first == NULL) return 7;
  if (midos->first->first == NULL) return 8;
  if (midos->ati == NULL) return 9;
  if (midos->atj == NULL) return 10;

  int found=0, stop=0;
  line256 *linerunner=midos->ati;
  pixel256 *pixrunner=linerunner->first;
  /* tests #11 and #12 aren't working properly
  while (!stop){
    if (pixrunner->next == NULL) stop=1;
    if (&pixrunner == &midos->atj){ found=1; stop=1; };
    if (!stop) pixrunner=pixrunner->next;
  };
  //if (!found) return 11;  //reference x isn't on line of reference y

  linerunner=midos->first;
  stop=0; found=0;
  while (!stop){
    if (linerunner->next == NULL) stop=1;
    if (&linerunner == &midos->ati){ found=1; stop=1; };
    if (!stop) linerunner=linerunner->next;
  };
  if (!found) return 12; //reference y does not belong to the image
  */

  linerunner=midos->first;
  int i, j;
  for (i=0; i < midos->height; i++){
    if (i < midos->height - 1){
      if (linerunner->next == NULL) return 13; //image too short
      else linerunner=linerunner->next;
    };
  };
  if ((linerunner->next != NULL)&&(!midos->wraparound)) return 14; //too tall

  linerunner=midos->first;
  pixrunner=linerunner->first;
  for (i=0; i < midos->height; i++){
    if (linerunner->first == NULL) return 15; //missing row of pixels
    for (j=0; j< midos->length; j++){
      if ((pixrunner->red < 0)||(pixrunner->red > 255)) return 16;
      if ((pixrunner->green < 0)||(pixrunner->green > 255)) return 17;
      if ((pixrunner->blue < 0)||(pixrunner->blue > 255)) return 18;
      if (j < midos->length - 1){
        if (pixrunner->next == NULL) return 19; //pixel row too short
        else pixrunner=pixrunner->next;
      };
    };
    if ((pixrunner->next != NULL)&&(!midos->wraparound)) return 20; //too long
    if (i < midos->height - 1){
      linerunner=linerunner->next;
      pixrunner=linerunner->first;
    };
  };
  return 0;  //if you've gotten this far, then everything's o.k.
}

//blake functions##########################################################

  //creates a new blake and gives it a uniform color
blake *newBlake(int length, int height, double shade, int wraparound){
  if (shade < 0) shade=0;
  if (shade > 1) shade=1;
  if (wraparound != 0) wraparound=1;
  blake *toreturn = new blake;
  if (length < 1) length=1;
  if (height < 1) height=1;
  int i, j;
  lineBlake *linerunner2=new lineBlake, *linerunner;
  pixelBlake *pixrunner2=new pixelBlake, *pixrunner;
  toreturn->first=linerunner2;
  toreturn->first->first=pixrunner2;
  linerunner=toreturn->first;
  pixrunner=linerunner->first;
  for (i=0; i<height; i++){
    for (j=0; j<length; j++){
      pixrunner->shade=shade;
      if (j == length-1){
        if (!wraparound) pixrunner->next=NULL;
        else pixrunner->next=linerunner->first;
      }else{
        pixrunner2=new pixelBlake;
        pixrunner->next=pixrunner2;
        pixrunner=pixrunner2;
      };
    };
    if (i == height-1){
      if (!wraparound) linerunner->next=NULL;
      else linerunner->next=toreturn->first;
    }else{
      linerunner2=new lineBlake;
      linerunner->next = linerunner2;
      linerunner=linerunner2;
      pixrunner2=new pixelBlake;
      linerunner->first=pixrunner2;
      pixrunner=linerunner->first;
    };
  };
  toreturn->height=height;
  toreturn->length=length;
  toreturn->i=0;
  toreturn->j=0;
  toreturn->ati=toreturn->first;
  toreturn->atj=toreturn->ati->first;
  toreturn->wraparound=wraparound;
  return toreturn;
}

/*
blake *exaggerateEdges(blake *blakeslee, double intensity, int wraparound){
  int length=blakeslee->length;
  int height=blakeslee->height;
  blake *vaj=newBlake(length, height, 0);
  double north, south, east, west;
  lineBlake *top, *middle, *bottom;
  pixelBlake *pixN, *pixW, *pixS, *pixE, *pixM;
  int i, j, k;
  double diff;

    //doesn't work for non-wraparound images yet (more work...)
  if (blakeslee->wraparound){
    middle=blakeslee->first;
    bottom=middle->next;
    top=middle;
    for (i=1; i < height; i++) top=top->next;
    for (i=0; i < height; i++){
      pixM=middle->first;
      pixE=pixM->next;
      pixW=pixM;
      pixN=top->first;
      pixS=bottom->first;
      for (k=1; k < length; k++) pixW=pixW->next;
      for (j=0; j < length; j++){

        diff= pixN->shade - pixM->shade;
        //... 

        pixN=pixN->next;
        pixE=pixE->next;
        pixS=pixS->next;
        pixW=pixW->next;
        pixM=pixM->next;
      };
      top=top->next;
      middle=middle->next;
      bottom=bottom->next;
    };
  }; 
  }*/

//format conversion functions##############################################

image256 *blake_to_image256(blake *alding){
  image256 *delvage = newImage256(alding->length, alding->height, 0, 0, 0);
  line256 *linerunnerC = delvage->first;
  pixel256 *pixrunnerC = linerunnerC->first;
  lineBlake *linerunnerB = alding->first;
  pixelBlake *pixrunnerB = linerunnerB->first;
  int i, j, k;
  int length=alding->length, height=alding->height;
  for (i=0; i<height; i++){
    for (j=0; j<length; j++){
      k=int(255*pixrunnerB->shade);
      pixrunnerC->red=k;
      pixrunnerC->green=k;
      pixrunnerC->blue=k;
      if (j < length-1){
        pixrunnerC=pixrunnerC->next;
        pixrunnerB=pixrunnerB->next;
      };
    };
    if (i < height-1){
      linerunnerC=linerunnerC->next;
      linerunnerB=linerunnerB->next;
      pixrunnerC=linerunnerC->first;
      pixrunnerB=linerunnerB->first;
    };
  };

  delvage->updated=0;
  return delvage;
}

blake *image256_to_blake(image256 *alding){
  blake *delvage = newBlake(alding->length, alding->height, 0);
  lineBlake *linerunnerB = delvage->first;
  pixelBlake *pixrunnerB = linerunnerB->first;
  line256 *linerunnerC = alding->first;
  pixel256 *pixrunnerC = linerunnerC->first;
  int i, j, k;
  int length=alding->length, height=alding->height;
  for (i=0; i<height; i++){
    for (j=0; j<length; j++){
      pixrunnerB->shade=(pixrunnerC->red + pixrunnerC->green + pixrunnerC->blue)/765.0;
      if (j < length-1){
        pixrunnerC=pixrunnerC->next;
        pixrunnerB=pixrunnerB->next;
      };
    };
    if (i < height-1){
      linerunnerC=linerunnerC->next;
      linerunnerB=linerunnerB->next;
      pixrunnerC=linerunnerC->first;
      pixrunnerB=linerunnerB->first;
    };
  };

  return delvage;
}
















