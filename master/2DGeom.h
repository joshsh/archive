void makeTriangle(point2 p1, point2 p2, point2 p3){
  line2D(p1, p2);
  line2D(p2, p3);
  line2D(p3, p1);
}

void makeQuader(point2 p1, point2 p2, point2 p3, point2 p4){
  line2D(p1, p2);
  line2D(p2, p3);
  line2D(p3, p4);
  line2D(p4, p1);
}

  //angle is the angle which a vertex makes relative to the center.
void makeRegPolygon( point2 pcen, double prad, int psides, double angle ){ 
  double pangle=2*pi/(psides*1.0);
  point2 p, pl;
  p.x=pcen.x+(prad*cos(angle));
  p.y=pcen.y+(prad*sin(angle));
  for (i=1; i<=psides; i++){
    pl=p;
    p.x=pcen.x+(prad*cos(angle+(i*pangle)));
    p.y=pcen.y+(prad*sin(angle+(i*pangle))); 
    line2D(pl, p);   
  };
}

  //ccor is the upper left hand corner
void makeRectangle( point2 ccor, double length, double height){
  pixel acor=pointPixel(ccor);
  int alength=length*xpix/xspan;
  int aheight=height*ypix/yspan;
  drawRectangle( pix, acor.x, acor.y, alength, aheight);
}

  //ccor is the upper left hand corner
void makeFilledRectangle( point2 ccor, double length, double height){
  pixel acor=pointPixel(ccor);
  int alength=length*xpix/xspan;
  int aheight=height*ypix/yspan;
  fillRectangle( pix, acor.x, acor.y, alength, aheight);
}

void makeCircle( point2 ccen, double prad){
  double xrad=prad*xpix/xspan;
  double yrad=prad*ypix/yspan;
  pixel apparentcen=pointPixel(ccen);
  drawEllipse( pix, apparentcen.x, apparentcen.y, 2*xrad, 2*yrad);
};

void makeFilledCircle( point2 ccen, double prad){
  double xrad=prad*xpix/xspan;
  double yrad=prad*ypix/yspan;
  pixel apparentcen=pointPixel(ccen);
  fillEllipse( pix, apparentcen.x, apparentcen.y, 2*xrad, 2*yrad);
};

void makeEllipse( point2 ccen, double pxrad, pyrad){
  double xrad=pxrad*xpix/xspan;
  double yrad=pyrad*ypix/yspan;
  pixel apparentcen=pointPixel(ccen);
  drawEllipse( pix, apparentcen.x, apparentcen.y, 2*xrad, 2*yrad);
};

void makeFilledEllipse( point2 ccen, double pxrad, pyrad){
  double xrad=pxrad*xpix/xspan;
  double yrad=pyrad*ypix/yspan;
  pixel apparentcen=pointPixel(ccen);
  fillEllipse( pix, apparentcen.x, apparentcen.y, 2*xrad, 2*yrad);
};