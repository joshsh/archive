struct intNode{
  int n;
  intNode *next;
};

struct inlist{
  intNode *in;
  inlist *next;
}

struct area{
  Window win;
  Pixmap pix;
  int label;
  int x, y;  //upper left hand corner of the area's rectangular convex hull
  int length, height; //length and height of the area's "..."
  area *son;  //NULL if there are no sub-areas
  area *next; 
};



  //assumes taht you are applying it to the appropriate window
void address(int x, y){

}

  //assumes that you are applying it to the appropriate window
inlist *allIns(int x, y){
  
}
