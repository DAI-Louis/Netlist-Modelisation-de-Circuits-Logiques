#include "CellViewer.h"
#include <QMenuBar>
#include <QAction>
// #include

namespace Netlist{
  CellViewer :: CellViewer ( QWidget * parent )
  : QMainWindow ( parent )
  , cellWidget_ ( NULL )
  , saveCellDialog_ ( NULL )
  { 
    cellWidget_ = new CellWidget ();
    /*Partie saveCellDialog*/
    saveCellDialog_ = new SaveCellDialog ( this );
    setCentralWidget ( cellWidget_ );
    QMenu * fileMenu = menuBar ()->addMenu ( "&File" );
    QAction * action = new QAction ( "&Save As" , this );
    action->setStatusTip ( "Save to disk ( rename ) the Cell" );
    action->setShortcut ( QKeySequence ( "CTRL+S" ) );
    action->setVisible ( true );
    fileMenu->addAction ( action );
    connect ( action , SIGNAL ( triggered ()) , this , SLOT ( saveCell ()) );

    cellsLib_ = new CellsLib();
    cellsLib_->setCellViewer(this);
    instancesWidget_ = new InstancesWidget();
    instancesWidget_->setCellViewer(this);

    /*openCellDialog*/
    QAction * actionOpen = new QAction ( "&Open" , this );
    actionOpen->setStatusTip ( "&Open from disk" );
    actionOpen->setShortcut ( QKeySequence ( "CTRL+O" ) );
    actionOpen->setVisible ( true );
    fileMenu->addAction(actionOpen);
    connect ( actionOpen , SIGNAL ( triggered ()) , this , SLOT ( openCell ()) );
    
    /*CellsLib*/
    QAction * actionCells = new QAction ( "&Cells" , this );
    actionCells->setStatusTip ( "&Show the amazing&incredible-looking Cell table" );
    actionCells->setShortcut ( QKeySequence ( "CTRL+U" ) );
    actionCells->setVisible ( true );
    fileMenu->addAction(actionCells);
    connect ( actionCells , SIGNAL ( triggered ()) , this , SLOT ( showCellsLib ()) );

    /*InstancesWidget*/
    QAction * actionInstances = new QAction ( "&Instances" , this );
    actionInstances->setStatusTip ( "&Show the cool good-looking Instance table" );
    actionInstances->setShortcut ( QKeySequence ( "CTRL+I" ) );
    actionInstances->setVisible ( true );
    fileMenu->addAction(actionInstances);
    connect ( actionInstances , SIGNAL ( triggered ()) , this , SLOT ( showInstancesWidget ()) );

    /*Quit*/
    action = new QAction ( "&Quit" , this );
    action->setStatusTip ( "Exit the Netlist Viewer" );
    action->setShortcut ( QKeySequence ( "CTRL+Q" ) );
    action->setVisible ( true );
    fileMenu->addAction ( action );
    connect ( action , SIGNAL ( triggered ()) , this , SLOT ( close ()) );
  }

  CellViewer::~CellViewer() {}

  void CellViewer :: saveCell ()
  {
    Cell * cell = getCell ();
    if ( cell == NULL ) return ;
    QString cellName = cell->getName (). c_str ();
    if ( saveCellDialog_->run ( cellName )) {
      cell->setName ( cellName . toStdString () );
      cell->save ();
    }
  }

  /*Fonction qui va changer le Cell de CellWidget*/
  void CellViewer::setCell(Cell* c) {cellWidget_->setCell(c);}
  /*Fonction qui va récupérer la Cell depui cellWidget*/
  Cell* CellViewer::getCell() const {return cellWidget_->getCell();}

  void CellViewer::openCell(){
    OpenCellDialog* open = new OpenCellDialog(this);
    QString name = OpenCellDialog::run(open);
    std::string namestr = name.toStdString();
    if (name!=NULL){
      Cell* c = Cell::find(namestr);
      //Si cellule non chargé
      if (c == NULL){
        if(Cell* cl = Cell::load(namestr)){
          this->cellWidget_->setCell(cl); 
          emit cellLoaded();
        }
        else {
          std::cerr << "ERR : file_not_found !!!! (CellViewer::openCell)" << std::endl; 
          return;
        }
      }
      else
        this->cellWidget_->setCell(c);
    
    }
    //Partie debug pour tester la fonction
    // cellWidget_->getCell()->toXml(std::cout);
    CellWidget::InstancesShapes = 1;
    CellWidget::InstanceShow =0;
    return;
  }

  //TME 9+

  /*Affiche le tableau des instances*/
  void CellViewer::showInstancesWidget(){
    instancesWidget_->setCell(this->cellWidget_->getCell());
    instancesWidget_->show();
  }

  /*Affiche le tableau des cellules*/
  void CellViewer::showCellsLib(){
    cellsLib_->show();
    std::cout << cellsLib_->getBaseModel() << std::endl;

  }

}