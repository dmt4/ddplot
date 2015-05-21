#include <math.h>
#include <qpainter.h>
#include <stdlib.h>

#include "pltwin.h"


/*
  Plotting the EDGE COMPONENT of the displacements. The arrows correspond to the relative
  displacements of the two connected atoms in the plane of the figure. Their direction
  is the actual direction of the relative displacement on going from the perfect lattice
  into the distorted structure.
*/

void PltWin::plotEdgeComponent(QPainter *p)
{
  QString txt;
  DVector rdisp(3);
  DVector xy_from(2), xy_to(2);
  qreal ang;
  QFontMetrics fm(p->fontMetrics());
  QRect rect;
  double xfrom, yfrom, xto, yto, x, y;
  double cen, lproj, pvsize, ndisp, cdisp, len;
  int i, d, ineigh, icen, idx, xf, yf;
  bool plotted;


  p->setFont( QFont( "helvetica", 8 ) );

  for (icen=1; icen<=NInit; icen++) {
    // plot only those atoms which belong to the active layers
    if (!zLayerSel(zLayer(icen)))
      continue;

    idx = 1;    

    while (NeighListInit(icen,idx)!=END_OF_LIST) {
      ineigh = NeighListInit(icen,idx);

      // plot only those atoms which belong to the active layers
      if (!zLayerSel(zLayer(ineigh))) {
	idx++;
	continue;
      }

      switch(DispComponent) {
        case EDGE:
	  rdisp(1) = scaleArr(icen,idx,1)*DScaleFact;
	  rdisp(2) = scaleArr(icen,idx,2)*DScaleFact;
          break;

        case MIXED:
          for (d=1; d<=3; d++)
            rdisp(d) = aDisp(ineigh,d) - aDisp(icen,d);

	  // dot product of rdisp with ProjVector (normalized)
	  lproj = 0;
          pvsize = sqrt( pow(ProjVector(1),2)+pow(ProjVector(2),2)+pow(ProjVector(3),2) );
          for (d=1; d<=3; d++)
            lproj += rdisp(d)*ProjVector(d)/pvsize; 
 
          for (d=1; d<=3; d++)
            rdisp(d) = lproj*ProjVector(d);

	  rdisp(1) = rdisp(1)*DScaleFact;
	  rdisp(2) = rdisp(2)*DScaleFact;
      }

      // calculation of the length of an arrow between icen and ineigh
      for (d=1; d<=2; d++) {
	if (AtomPos==UNRELAXED)
          cen = (xyzInit(icen,d)+xyzInit(ineigh,d))/2.0;         // center of the arrow
        else
          cen = (xyzRel(icen,d)+xyzRel(ineigh,d))/2.0;           // center of the arrow

        xy_from(d) = cen - rdisp(d)/2.0;
        xy_to(d) = cen + rdisp(d)/2.0;
      }
      //--------------------------------------------------------------------

      // plotting of the arrows
      xfrom = ZFact*xy_from(1) - xPan - (ZFact-1)*xyzCen(1);
      yfrom = ZFact*xy_from(2) - yPan - (ZFact-1)*xyzCen(2);
      xto = ZFact*xy_to(1) - xPan - (ZFact-1)*xyzCen(1);
      yto = ZFact*xy_to(2) - yPan - (ZFact-1)*xyzCen(2);

      if (xfrom==xto && yfrom==yto) 
	plotted = false;
      else
	plotted = DrawArrow(p, xfrom, yfrom, xto, yto);

      // annotations
      if (PrintMagDisp && plotted) {
	len = sqrt( pow(rdisp(1)/DScaleFact,2) + pow(rdisp(2)/DScaleFact,2) );
        txt.sprintf("a/%0.2lf", 1.0/fabs(len));
	rect = fm.boundingRect(txt);
	rect.moveRight(rect.width()/2.0);
	
	x = (xfrom+xto)/2.0;
	y = (yfrom+yto)/2.0;
	xf = xBorder + xOffset + (int) floor(factor*(x-xyzMin(1)));
	yf = height() - (yBorder + yOffset + (int) floor(factor*(y-xyzMin(2))));
	ang = atan2(yto-yfrom, xto-xfrom);

	if (fabs(ang)>M_PI/2.0) ang += M_PI;

	p->save();
	p->translate(xf, yf);
	p->rotate(-ang*180.0/M_PI);
        p->drawText(rect, Qt::AlignJustify, txt);
	p->restore();
      }

      idx++;
    }
  }
}


/*
  Plotting the arrows for the SCREW COMPONENT of relative displacements between atoms.
  Each arrow corresponds to the relative displacements of the two connected atoms in 
  the direction perpendicular to the plane of the screen, relative to the distance of
  the two atoms in the perfect lattice. If an arrow is longer than the Burgers vector,
  only its modulo is taken. If this is larger than 1/2 of the Burgers vector, the
  sense of the arrow is reversed and the length is proportional to the difference
  between the original size of the arrow and b/2.
*/

void PltWin::plotScrewComponent(QPainter *p)
{
  QString txt;
  DVector rdisp(3), xyz0(3), xyz1(3), rvect(3);
  DVector xyz_from(3), xyz_to(3), rscreen(3);
  qreal ang;
  QFontMetrics fm(p->fontMetrics());
  QRect rect;
  QPoint offs;
  double xfrom, yfrom, xto, yto, cen, x, y;
  int i, d, ineigh, icen, idx, xf, yf;
  bool plotted;

  p->setFont( QFont( "helvetica", 8 ) );

  for (icen=1; icen<=NInit; icen++) {
    // plot only those atoms which belong to the active layers
    if (!zLayerSel(zLayer(icen)))
      continue;

    idx = 1;    

    while (NeighListInit(icen,idx)!=END_OF_LIST) {
      ineigh = NeighListInit(icen,idx);

      // plot only those atoms which belong to the active layers
      if (!zLayerSel(zLayer(ineigh))) {
	idx++;
	continue;
      }

      // calculation of the length of an arrow between icen and ineigh
      if (AtomPos==UNRELAXED) {
        for (d=1; d<=3; d++) {
          xyz0(d) = xyzInit(icen,d);
          xyz1(d) = xyzInit(ineigh,d); 
        }
      } else {
        for (d=1; d<=3; d++) {
          xyz0(d) = xyzRel(icen,d) + AtomDispScale*aDisp(icen,d);
          xyz1(d) = xyzRel(ineigh,d) + AtomDispScale*aDisp(ineigh,d); 
        }
      }

      for (d=1; d<=3; d++) {
        rvect(d) = xyz1(d) - xyz0(d);
	rscreen(d) = scaleArr(icen,idx,3)*DScaleFact*rvect(d);   
      }

      // adjustment of arrows - they must lie between two atoms
      for (d=1; d<=3; d++) {
        cen = (xyz0(d)+xyz1(d))/2.0;
        xyz_from(d) = cen - rscreen(d)/2.0;
        xyz_to(d) = cen + rscreen(d)/2.0;
      }
      //----------------------------------------------

      // plotting of the arrows
      xfrom = ZFact*xyz_from(1) - xPan - (ZFact-1)*xyzCen(1);
      yfrom = ZFact*xyz_from(2) - yPan - (ZFact-1)*xyzCen(2);
      xto = ZFact*xyz_to(1) - xPan - (ZFact-1)*xyzCen(1);
      yto = ZFact*xyz_to(2) - yPan - (ZFact-1)*xyzCen(2);

      if (xfrom==xto && yfrom==yto)
	plotted = false;
      else
      	plotted = DrawArrow(p, xfrom, yfrom, xto, yto);

      // annotations
      if (PrintMagDisp && plotted) {
        txt.sprintf("a/%0.2lf", 4.0/(fabs(scaleArr(icen,idx,3))));
	rect = fm.boundingRect(txt);
	rect.moveRight(rect.width()/2.0);
	
	x = (xfrom+xto)/2.0;
	y = (yfrom+yto)/2.0;
	xf = xBorder + xOffset + (int) floor(factor*(x-xyzMin(1)));
	yf = height() - (yBorder + yOffset + (int) floor(factor*(y-xyzMin(2))));
	ang = atan2(yto-yfrom, xto-xfrom);

	if (fabs(ang)>M_PI/2.0) ang += M_PI;

	p->save();
	p->translate(xf, yf);
	p->rotate(-ang*180.0/M_PI);
        p->drawText(rect, Qt::AlignJustify, txt);
	p->restore();
      }
 
      idx++;
    }
  }
}


/*
  Plotting the DIFFERENCE between the displacements in two plots. Prior to calling this function,
  it is necessary to call PltWin::compareDisp to calculate the difference between the displacement
  of an atom in the two selected plots. This is then stored for each individual atom in aDisp.
  Note, that in this case aDisp does not keep the displacements of atoms due to applied stress but
  instead the difference between these displacements in two different plots.
*/

void PltWin::plotDifference( QPainter *p )
{
  QString txt;
  DVector xycen(2), xy_from(2), xy_to(2);
  double diff, ratio, rdist, xfrom, yfrom, xto, yto;
  int icen, ineigh, idx, i, d, symsize;
  bool exist;


  p->setFont( QFont( "helvetica", 8 ) );

  for (icen=1; icen<=NInit; icen++) {
    // plot only those atoms which belong to the active layers
    if (!zLayerSel(zLayer(icen)))
      continue;

    idx = 1;    

    while (NeighListInit(icen,idx)!=END_OF_LIST) {
      ineigh = NeighListInit(icen,idx);

      // plot only those atoms which belong to the active layers
      if (!zLayerSel(zLayer(ineigh))) {
	idx++;
	continue;
      }

      // we are plotting in the (xy) plane - don't take the z-coordinate for scaling
      rdist = sqrt( pow(xyzInit(icen,1)-xyzInit(ineigh,1),2) +
		    pow(xyzInit(icen,2)-xyzInit(ineigh,2),2) );

      // for comparison of screw components
      switch(DispComponent) {
        case DIFF_SCREW:
	  symsize = trunc( scaleArr(icen,idx,3)*rdist * factor*DScaleFact*ZFact );  
	  if (symsize==0) 
	    break;

	  p->setFont( QFont("helvetica", abs(symsize)) );
	    
	  if (symsize>0) {
	    txt = "+";
	    p->setPen( QPen(Qt::red) );
	  } else {
	    txt = '-';
	    p->setPen( QPen(Qt::blue) );
	  }
	  
	  // center the sign
	  for (d=1; d<=2; d++)
	    xycen(d) = (xyzInit(icen,d) + xyzInit(ineigh,d))/2.0 - abs(symsize)/(3*ZFact*factor);
	    
	  // plotting either the plus or minus sign according to the magnitude in diff
	  DrawText( p, ZFact*xycen(1)-xPan-(ZFact-1)*xyzCen(1), 
		    ZFact*xycen(2)-yPan-(ZFact-1)*xyzCen(2), txt );
	  break;

        case DIFF_EDGE:
	  // center of the arrow
	  for (d=1; d<=2; d++) {
	    xycen(d) = (xyzInit(icen,d)+xyzInit(ineigh,d))/2.0;         
	    xy_from(d) = xycen(d) - DScaleFact*scaleArr(icen,idx,d)/2.0;
	    xy_to(d) = xycen(d) + DScaleFact*scaleArr(icen,idx,d)/2.0;
	  }

	  // plotting of the arrows
	  xfrom = ZFact*xy_from(1) - xPan - (ZFact-1)*xyzCen(1);
	  yfrom = ZFact*xy_from(2) - yPan - (ZFact-1)*xyzCen(2);
	  xto = ZFact*xy_to(1) - xPan - (ZFact-1)*xyzCen(1);
	  yto = ZFact*xy_to(2) - yPan - (ZFact-1)*xyzCen(2);
	  
	  DrawArrow(p, xfrom, yfrom, xto, yto);
      }

      idx++;
    }
  }
}


/*
  Plotting a contour plot that corresponds to the spatial variation of a given component of the
  dislocation density (Nye) tensor.
*/

void PltWin::plotNyeTensor( QPainter *p )
{

}

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------


bool PltWin::DrawArrow( QPainter *p, double x0, double y0, double x1, double y1 )
{
  QPolygon pts;
  const double h = 0.2*tan(arrAngle*M_PI/360.0);  
  double arrow[4][2] = { {0.0, 0.0}, {1.0, 0.0}, {0.8, h}, {0.8, -h} };
  //  const double h = 0.3*tan(arrAngle*M_PI/360.0);  // bigger arrowhead
  //  double arrow[4][2] = { {0.0, 0.0}, {1.0, 0.0}, {0.7, h}, {0.7, -h} };
  DMatrix xy(4,2);
  int i;
  double ang, xrot, yrot, size;

  size = sqrt(pow(x1-x0,2)+pow(y1-y0,2));
  if (factor*size<shortestArrow) 
    return false;

  // scaling and rotation
  ang = atan2(y1-y0,x1-x0);
  for (i=0; i<4; i++) {
    // scaling
    arrow[i][0] *= size;
    arrow[i][1] *= size;

    // rotation
    xrot = arrow[i][0]*cos(ang) - arrow[i][1]*sin(ang);
    yrot = arrow[i][0]*sin(ang) + arrow[i][1]*cos(ang);
    arrow[i][0] = xrot;
    arrow[i][1] = yrot;
  }

  for (i=1; i<=4; i++) {
    xy(i,1) = xBorder + xOffset + factor*(x0+arrow[i-1][0]-xyzMin(1));
    xy(i,2) = height() - (yBorder + yOffset + factor*(y0+arrow[i-1][1]-xyzMin(2)));
  }

  if (paintEPS) {
    fprintf(feps, "%0.2lf %0.2lf moveto %0.2lf %0.2lf lineto stroke\n", 
  	    xy(1,1), height()-xy(1,2), xy(2,1), height()-xy(2,2));
    fprintf(feps, "%0.2lf %0.2lf newpath moveto %0.2lf %0.2lf lineto  %0.2lf %0.2lf lineto closepath fill stroke\n", 
	    xy(2,1), height()-xy(2,2), xy(3,1), height()-xy(3,2), xy(4,1), height()-xy(4,2));
  } else {
    QLineF line(xy(1,1), xy(1,2), xy(2,1), xy(2,2));
    p->drawLine(line);
    QPointF poly[3] = {
      QPointF(xy(2,1), xy(2,2)), 
      QPointF(xy(3,1), xy(3,2)), 
      QPointF(xy(4,1), xy(4,2)) };
    p->drawPolygon(poly, 3);
  }
  
  return true;
}


void PltWin::DrawCircle( QPainter *p, int atom, double x, double y, int dia)
{
  QColor col;
  int xpos, ypos, lw;
  int rad = int(dia/2.0);

  // (0,0) is at the bottom left corner of the screen
  xpos = xBorder + xOffset + (int) floor(factor*(x-xyzMin(1))) - rad; 
  ypos = height() - (yBorder + yOffset + (int) floor(factor*(y-xyzMin(2)))) - rad;

  if (paintEPS) {
    if (atom>0) 
      col = zColorLayer(zLayer(atom),2);
    else
      col = Qt::white;
    fprintf(feps, "%0.1f %0.1f %0.1f setrgbcolor %d %d %d 0 360 arc closepath fill\n", 
	    col.red()/255.0, col.green()/255.0, col.blue()/255.0, xpos+rad, height()-ypos-rad, rad);
    if (atom>0) {
      col = zColorLayer(zLayer(atom),1);
      lw = zLineThickLayer(zLayer(atom));
    } else {
      col = Qt::black;
      lw = 1;
    }
    fprintf(feps, "%0.1f %0.1f %0.1f setrgbcolor %d setlinewidth %d %d %d 0 360 arc closepath stroke\n", 
	    col.red()/255.0, col.green()/255.0, col.blue()/255.0, lw, xpos+rad, height()-ypos-rad, rad);
  } else
    p->drawEllipse( xpos, ypos, dia, dia );    

  // if the real atomic number is given, keep its coordinates on the screen
  if (atom>0) {
    atomScreen(atom,1) = xpos + rad;
    atomScreen(atom,2) = ypos + rad;
  }
}


void PltWin::DrawLine( QPainter *p, double x0, double y0, double x1, double y1, int addx, int addy)
{
  int xfrom, xto, yfrom, yto;

  xfrom = xBorder + xOffset + (int) floor(factor*(x0-xyzMin(1))) + addx;
  xto   = xBorder + xOffset + (int) floor(factor*(x1-xyzMin(1))) + addx;
  yfrom = height() - (yBorder + yOffset + (int) floor(factor*(y0-xyzMin(2))) + addy);
  yto   = height() - (yBorder + yOffset + (int) floor(factor*(y1-xyzMin(2))) + addy);

  if (paintEPS)
    fprintf(feps, "%d %d moveto %d %d lineto stroke\n", xfrom, height()-yfrom, xto, height()-yto);
  else
    p->drawLine( xfrom, yfrom, xto, yto );
}


void PltWin::DrawPlaneTraces( QPainter *p, double x0, double y0, double x1, double y1 )
{
  int ang, circDia;
  int x0int, y0int, x1int, y1int;
  double xEnd, yEnd, xC, yC, rang;

  QPen *apen = new QPen();
  apen->setColor( Qt::darkGray );
  p->setPen(*apen);

  x0int = xBorder + xOffset + (int) floor(factor*(x0-xyzMin(1))); 
  y0int = height() - (yBorder + yOffset + (int) floor(factor*(y0-xyzMin(2))));  
  x1int = xBorder + xOffset + (int) floor(factor*(x1-xyzMin(1))); 
  y1int = height() - (yBorder + yOffset + (int) floor(factor*(y1-xyzMin(2))));

  circDia = 10;
  p->setClipRect(x0int-2*circDia, y1int-2*circDia,
		 x1int-x0int+4*circDia, y0int-y1int+4*circDia);

  xC = ZFact*xCore - xPan - (ZFact-1)*xyzCen(1);
  yC = ZFact*yCore - yPan - (ZFact-1)*xyzCen(2);

  for (ang=0; ang<360; ang+=PTAngle) {
    rang = ang*M_PI/180.0;
    xEnd = xCore + 100.0*cos(rang);
    yEnd = yCore + 100.0*sin(rang);

    DrawLine(p, xC, yC, xEnd, yEnd, 0, 0);
  }

  p->setClipping(false);
}


void PltWin::DrawText( QPainter *p, double x, double y, QString txt )
{
  int xf, yf;

  xf = xBorder + xOffset + (int) floor(factor*(x-xyzMin(1)));
  yf = height() - (yBorder + yOffset + (int) floor(factor*(y-xyzMin(2))));

  p->drawText( xf, yf, txt );
}


void PltWin::Pan( double xsteps, double ysteps )
{
  xPan += PAN_RATIO*xsteps*blSize(1);   
  yPan += PAN_RATIO*ysteps*blSize(2);   
  repaint();
}


void PltWin::SetGeometry()
{
  double xfact, yfact;

  xfact = (width()-2*xBorder)/blSize(1);
  yfact = (height()-2*yBorder)/blSize(2);
  factor = (xfact<yfact) ? xfact : yfact;

  xOffset = (int) floor( (width()-2*xBorder-factor*blSize(1))/2.0 );
  yOffset = (int) floor( (height()-2*yBorder-factor*blSize(2))/2.0 );
}


void PltWin::ShowActiveZLayers( QPainter *p )
{
  QString txt;
  double step, pos;
  int i, yheight;

  p->setPen( Qt::darkGray ); 
  p->setBrush( Qt::white );

  yheight = 12*(NumZLayers-1);
  if (yheight>height()-80)
    yheight = height()-80;

  p->drawRect( 10, height()-yheight-60, 130, yheight+40 );

  step = double(yheight)/(NumZLayers-1);
  for (i=1; i<=NumZLayers; i++ ) {
    pos = height()-40 - (i-1)*step;

    // active layers in green, inactive in black
    if (zLayerSel(i))
      p->setPen( Qt::green );
    else
      p->setPen( Qt::black );

    p->drawLine( 75, pos, 120, pos );

    txt = txt.setNum(zCoordLayer(i), 'f', 4);
    p->drawText( 30, pos+step/2.0, txt );
  }
}


void PltWin::ShowCSys( QPainter *p )
{
  QString txt;
  int xwidth, yheight, xmin, ymin;

  xwidth = yheight = 150;
  xmin = width()-xwidth;
  ymin = height()-yheight-20;

  p->setPen( Qt::darkGray ); 
  p->setBrush( Qt::white );
  p->drawRect( xmin, ymin, xwidth, yheight );

  // outlining box
  p->setBrush( Qt::NoBrush );
  p->drawRect( xmin+60, ymin+30, xwidth-90, yheight-90 );
  p->drawRect( xmin+30, ymin+60, xwidth-90, yheight-90 );
  p->drawLine( xmin+30, ymin+60, xmin+60, ymin+30 );
  p->drawLine( xmin+30+xwidth-90, ymin+60, xmin+60+xwidth-90, ymin+30 );
  p->drawLine( xmin+30, ymin+60+yheight-90, xmin+60, ymin+30+yheight-90 );
  p->drawLine( xmin+30+xwidth-90, ymin+60+yheight-90, xmin+60+xwidth-90, ymin+30+yheight-90 );

  // red lines indicating axes
  p->setPen( QPen( Qt::red, 2 ) );
  p->drawLine( xmin+60, ymin+30, xmin+60, ymin+30+yheight-90 );
  p->drawLine( xmin+60, ymin+30+yheight-90, xmin+60+xwidth-90, ymin+30+yheight-90 );
  p->drawLine( xmin+60, ymin+30+yheight-90, xmin+30, ymin+60+yheight-90 );

  // x-label
  txt.sprintf("[%d %d %d]", int(csys(1,1)), int(csys(1,2)), int(csys(1,3)));
  p->drawText( xmin+30+xwidth-90, ymin+30+yheight-90+13, txt);

  // y-label
  txt.sprintf("[%d %d %d]", int(csys(2,1)), int(csys(2,2)), int(csys(2,3)));
  p->drawText( xmin+30, ymin+27, txt);

  // z-label
  txt.sprintf("[%d %d %d]", int(csys(3,1)), int(csys(3,2)), int(csys(3,3)));
  p->drawText( xmin+15, ymin+60+yheight-90+13, txt);
}


void PltWin::Zoom( double zfact )
{
  if (ZFact*zfact<=20 && ZFact*zfact>=0.1) {
    ZFact *= zfact;
    repaint();
  }
}
