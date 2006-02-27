struct *tree2{
  point3 p;
  tree *right;
  tree *left;
};

struct *tree3{
  point3 p;
  tree *right;
  tree *left;
  tree *middle;
}

int maxbranches=5;
struct treeN{  
  point3 p;
  tree *list[5];  //[maxbranches]!
}

//area constants
pixel tree2DB, tree3DB;

void dtree2D(){

}

void dtree3D(){

}

void writeTrees(0{
  setColor( textC );
  putMenu ( tree2DB, "2-dimensional" );
  putMenu ( tree3DB, "3-dimensional" );
}

void treeMain(){
  //assgn area positions (all coordinates not written are 0)
  tree3DB.x=1;

  int box;
  while (1==1){
    resetAll();
    writeTrees();
    getChoice( general );
    if (menuNumber(tree2DB) == box)
    if (menuNumber(tree3DB) == box)
    if (menuNumber(backB) == box) return;
    if (menuNumber(quitB) == box) exit(1);
  };
}
