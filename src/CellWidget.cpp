// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-

#include  <QResizeEvent>
#include  <QPainter>
#include  <QPen>
#include  <QBrush>
#include  <QFont>
#include  <QApplication>
#include  "CellWidget.h"
#include  "Term.h"
#include  "Instance.h"
// #include  "Symbol.h"
#include  "Shape.h"
#include  "Cell.h"
#include  "Line.h"
#include  "Node.h"
#include  "Net.h"
#include  "BoxShape.h"
#include  "LineShape.h"
#include  "ArcShape.h"
#include  "TermShape.h"
#include  "EllipseShape.h"

#define SCREENSIZE 500

namespace Netlist {

  using namespace std;

  int CellWidget::InstancesShapes = 0;
  int CellWidget::InstanceShow = 0;
  int CellWidget::NodeShapes = 1;

  ostream& operator<< ( ostream& o, const QRect& rect )
  {
    o << "<QRect x:" << rect.x()
      <<       " y:" << rect.y()
      <<       " w:" << rect.width()
      <<       " h:" << rect.height() << ">";
    return o;
  }


  ostream& operator<< ( ostream& o, const QPoint& p )
  { o << "<QRect x:" << p.x() << " y:" << p.y() << ">"; return o; }


  CellWidget :: CellWidget ( QWidget * parent )
    : QWidget ( parent )
    , cell_ ( NULL )
    , viewport_ ( Box (0 ,0 ,500 ,500))
    {
    setAttribute ( Qt :: WA_OpaquePaintEvent );
    setAttribute ( Qt :: WA_NoSystemBackground );
    setSizePolicy ( QSizePolicy :: Expanding // X direction .
    , QSizePolicy :: Expanding ); // Y direction .
    setFocusPolicy ( Qt :: StrongFocus );
    setMouseTracking ( true );

    // CellWidget.cpp - À l'intérieur du constructeur, juste avant l'accolade fermante }

    // 1. On crée une cellule de test
    Cell* testCell = new Cell("TestVisuel");

    // 2. On crée un modèle (une porte ET par exemple) avec une forme carrée
    Cell* master = new Cell("MasterGate");
    new BoxShape(master->getSymbol(), Box(0, 0, 100, 100)); // Un carré de 100x100

    // 3. On ajoute des instances de ce modèle à positions différentes
    new Instance(testCell, master, "inst1"); // Position (0,0)
    Instance* inst2 = new Instance(testCell, master, "inst2");
    inst2->setPosition(Point(150, 150));     // Position (150, 150)

    // 4. IMPORTANT : On associe cette cellule au widget pour qu'il la dessine !
    setCell(testCell);
  }


  CellWidget::~CellWidget ()
  { }


  void  CellWidget::setCell ( Cell* cell )
  {
    cell_ = cell;
    repaint();
  }


  QSize CellWidget :: minimumSizeHint () const
  { return QSize (500 ,500); }


  void CellWidget :: resizeEvent ( QResizeEvent * event ) {
    const QSize & size = event -> size ();
    // Assume the resize is always done by drawing the bottom right corner .
    viewport_ . setX2 ( viewport_ . getX1 () + size . width () );
    viewport_ . setY1 ( viewport_ . getY2 () - size . height () );
    cerr << " CellWidget :: resizeEvent () ␣ viewport_ : " << viewport_ << endl;
  }

  /*Calcul des coordonnées*/
  int CellWidget::xToScreenX(int x) const {
    return x - viewport_.getX1(); 
  }

  int CellWidget::yToScreenY(int y) const{
    return viewport_.getY2() - y;
  }

  int CellWidget::screenXtoX(int x) const {
    return x + viewport_.getX1();
  }

  int CellWidget::screenYtoY(int y) const{
    return viewport_.getY2() - y;
  }

  void CellWidget :: paintEvent ( QPaintEvent * event ) {
    QPainter painter ( this );
    painter.setBackground ( QBrush ( Qt :: black ) );
    painter.eraseRect ( QRect ( QPoint (0 ,0) , size () ) );
    painter.setPen ( QPen ( Qt :: darkGreen , 3 ) );
    QRect rect = boxToScreenRect ( viewport_ );
    painter.drawRect ( rect );
    // painter.setPen ( QPen ( Qt :: red , 0 ) );
    // painter.setBrush ( QBrush ( Qt :: red ) );
    // ...
    painter . drawRect ( rect );
    if (InstancesShapes)
      query( InstancesShapes, painter );
    else
      query( InstanceShow, painter);
  }

  void CellWidget :: goRight () {
    viewport_.translate ( Point (20 ,0) );
    repaint ();
  }


  void CellWidget :: goUp () {
    viewport_ . translate ( Point (0 ,20) );
    repaint ();
  }

  void CellWidget :: goDown () {
    viewport_ . translate ( Point (0 , -20) );
    repaint ();
  }

  void CellWidget :: goLeft () {
    viewport_.translate ( Point (-20 ,0) );
    repaint ();
  }


  void CellWidget :: keyPressEvent ( QKeyEvent * event ) {
    event -> ignore ();
    if ( event -> modifiers () & ( Qt :: ControlModifier | Qt :: ShiftModifier ))
    return ;
    switch ( event -> key ()) {
    case Qt::Key_Up:goUp (); break ;
    case Qt::Key_Down:goDown (); break ;
    case Qt::Key_Left:goLeft (); break ;
    case Qt::Key_Right:goRight (); break ;
    default : return ;
    }
    event -> accept ();
  }

  void CellWidget ::query(unsigned int flags, QPainter &painter)
  {

    /*Si pas de cell et pas de flag*/
    if ((not cell_) or (not flags))
      return;

    if ((flags & NodeShapes) && (flags & InstancesShapes))
    {
      vector<Net *> n = cell_->getNets();
      painter.save();
      for (size_t j = 0; j < n.size(); ++j)
      {
        vector<Node *> node = n[j]->getNodes();
        for (size_t i = 0; i < node.size(); ++i)
        {
          NodePoint *nodePoint = dynamic_cast<NodePoint *>(node[i]);
          if (nodePoint && nodePoint->getLines().size() > 2)
          {
            Point p = nodePoint->getPosition();
            QPoint Qp = QPoint(xToScreenX(p.getX()), yToScreenY(p.getY()));
            QPen pen(Qt::cyan, 10, Qt::SolidLine, Qt::RoundCap);
            painter.setPen(pen);
            painter.drawPoint(Qp);
          }
        }

        vector<Line *> line = n[j]->getLines();
        for (size_t i = 0; i < line.size(); ++i)
        {
          Point p1 = line[i]->getSourcePosition();
          Point p2 = line[i]->getTargetPosition();

          int x1 = xToScreenX(p1.getX());
          int y1 = yToScreenY(p1.getY());
          int x2 = xToScreenX(p2.getX());
          int y2 = yToScreenY(p2.getY());

          painter.setPen(QPen(Qt ::cyan, 1));
          painter.drawLine(x1, y1, x2, y2);
        }
      }
      painter.restore();
    }

    /*Récupération des Instances*/
    const vector<Instance *> &instances = cell_->getInstances();
    /*Boucle sur les instances*/
    for (size_t i = 0; i < instances.size(); ++i)
    {
      Point instPos = instances[i]->getPosition();

      const Symbol *symbol = instances[i]->getMasterCell()->getSymbol();
      if (not symbol)
        continue;
      if (flags & InstancesShapes)
      {

        /*Affichage du nom de l'instance*/
        painter.drawText(xToScreenX(instPos.getX()) + 5, yToScreenY(instPos.getY()) + 10, 100, 20, Qt::AlignLeft, QString::fromStdString(instances[i]->getName()));

        drawInstance(symbol, instPos, painter);
        vector<Term *> terms = cell_->getTerms();
        // Symbol* symbol = cell_->getSymbol();
        painter.save();
        for (size_t j = 0; j < terms.size(); ++j)
        {
          Term *term = terms[j];
          Point p = term->getPosition();
          int x = xToScreenX(p.getX());
          int y = yToScreenY(p.getY());

          Term::Direction dir = term->getDirection();
          bool isOutput = (dir == Term::Direction::Out ||
                           dir == Term::Direction::Tristate ||
                           dir == Term::Direction::Transcv);

          int h = 20; // Hauteur
          int half_h = h / 2;
          int w_rect = 15;  // Largeur rectangle
          int w_arrow = 10; // Largeur pointe

          QPolygon poly;

          // Construction de la Forme
          if (isOutput)
          {
            poly << QPoint(x, y)                             
                 << QPoint(x + w_arrow, y - half_h)          
                 << QPoint(x + w_arrow + w_rect, y - half_h) 
                 << QPoint(x + w_arrow + w_rect, y + half_h) 
                 << QPoint(x + w_arrow, y + half_h);       
          }
          else
          {

            poly << QPoint(x, y)                             
                 << QPoint(x - w_arrow, y + half_h)         
                 << QPoint(x - w_arrow - w_rect, y + half_h) 
                 << QPoint(x - w_arrow - w_rect, y - half_h) 
                 << QPoint(x - w_arrow, y - half_h);       
          }

          //Dessin de la forme

          painter.setPen(Qt::NoPen);
          painter.setBrush(QBrush(Qt::darkRed));
          painter.drawPolygon(poly);

          //Dessin du Texte
          painter.setPen(QPen(Qt::darkRed));
          QString name = QString::fromStdString(term->getName());

          if (isOutput)
          {
            // Texte à DROITE
            int textX = x + w_arrow + w_rect + 5;
            painter.drawText(textX, y - half_h, 50, h, Qt::AlignLeft | Qt::AlignVCenter, name);
          }
          else
          {
            // Texte à GAUCHE
            int textX = x - w_arrow - w_rect - 55;
            painter.drawText(textX, y - half_h, 50, h, Qt::AlignRight | Qt::AlignVCenter, name);
          }
        }
        painter.restore();
      }
    }
    // CellWidget::InstancesShapes = 0;
    // Affichage de l'Instance/Cellule
    if (flags & InstanceShow)
      {

        /*Affichage du nom de l'instance*/
        painter.drawText(xToScreenX(SCREENSIZE / 2) + 5, yToScreenY(SCREENSIZE / 2) + 10, 100, 20, Qt::AlignLeft, QString::fromStdString(cell_->getName()));
        Symbol *s = cell_->getSymbol();
        Point p = Point(SCREENSIZE / 2, SCREENSIZE / 2);

        drawInstance(s, p, painter);
      }
  }

  void CellWidget::drawInstance(const Symbol *symbol, Point &instPos, QPainter &painter)
  {
    const vector<Shape *> &shapes = symbol->getShapes();
    for (size_t j = 0; j < shapes.size(); ++j)
    {
      // Symbole des Instances
      BoxShape *boxShape = dynamic_cast<BoxShape *>(shapes[j]);
      if (boxShape)
      {
        Box box = boxShape->getBoundingBox();
        QRect rect = boxToScreenRect(box.translate(instPos));
        painter.drawRect(rect);
      }

      // ArcShape
      ArcShape *arcShape = dynamic_cast<ArcShape *>(shapes[j]);
      if (arcShape)
      {
        Box box = arcShape->getBoundingBox();
        QRect rect = boxToScreenRect(box.translate(instPos));
        int startAngle = arcShape->getStart();
        int spanAngle = arcShape->getSpan();
        painter.drawArc(rect, startAngle * 16, spanAngle * 16);
      }

      // Dessin des connecteurs des instances
      TermShape *termShape = dynamic_cast<TermShape *>(shapes[j]);
      if (termShape)
      {
        Box box = termShape->getBoundingBox();
        QRect rect = boxToScreenRect(box.inflate(5).translate(instPos));
        painter.save();

        QString name = QString::fromStdString(termShape->getTerm()->getName());

        // Calcul des coordonnées écran
        long modelX = termShape->getX() + instPos.getX();
        long modelY = termShape->getY() + instPos.getY();
        int x = xToScreenX(modelX);
        int y = yToScreenY(modelY);

        TermShape::NameAlign align = termShape->getAlign();

        int h = 20;
        int w_symbol = 5; // Largeur du symbole
        int margin = 2;   // Petite marge pour ne pas coller au symbole

        painter.setPen(QPen(Qt::darkRed));

        int textX = 0;
        int textY = 0;
        int flags = 0;

        // Détermination de la position et des flags selon l'alignement
        switch (align)
        {
        case TermShape::TopLeft:
          // Texte à GAUCHE et AU-DESSUS du point
          textX = x - w_symbol - 100 - margin;
          textY = y - h;
          flags = Qt::AlignRight | Qt::AlignBottom;
          break;

        case TermShape::TopRight:
          // Texte à DROITE et AU-DESSUS du point
          textX = x + w_symbol + margin;
          textY = y - h;
          flags = Qt::AlignLeft | Qt::AlignBottom;
          break;

        case TermShape::BottomLeft:
          // Texte à GAUCHE et EN-DESSOUS du point
          textX = x - w_symbol - 100 - margin;
          textY = y + margin; // On descend un peu
          flags = Qt::AlignRight | Qt::AlignTop;
          break;

        case TermShape::BottomRight:
          // Texte à DROITE et EN-DESSOUS du point
          textX = x + w_symbol + margin;
          textY = y + margin; // On descend un peu
          flags = Qt::AlignLeft | Qt::AlignTop;
          break;
        }

        // Dessin du texte
        painter.drawText(textX, textY, 100, h, flags, name);

        painter.setBrush(QBrush(Qt::red));
        painter.setPen(QPen(Qt::darkRed));
        painter.drawRect(rect);
        painter.restore();
      }

      LineShape *lineShape = dynamic_cast<LineShape *>(shapes[j]);
      if (lineShape)
      {
        Point p1(lineShape->getX1(), lineShape->getY1());
        Point p2(lineShape->getX2(), lineShape->getY2());

        p1.translate(instPos);
        p2.translate(instPos);

        int x1 = xToScreenX(p1.getX());
        int y1 = yToScreenY(p1.getY());
        int x2 = xToScreenX(p2.getX());
        int y2 = yToScreenY(p2.getY());

        painter.drawLine(x1, y1, x2, y2);
      }

      EllipseShape *ellipseShape = dynamic_cast<EllipseShape *>(shapes[j]);
      if (ellipseShape)
      {
        Box box = ellipseShape->getBoundingBox();
        QRect rect = boxToScreenRect(box.translate(instPos));
        painter.drawEllipse(rect);
      }
    }
  }

}  // Netlist namespace.