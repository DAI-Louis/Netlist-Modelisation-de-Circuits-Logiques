#include <iostream>
#include "CellsModel.h"


namespace Netlist{
    CellsModel::CellsModel ( QObject * parent)
        : QAbstractTableModel ( parent )
        , cells_ ()
    { }

    CellsModel ::~ CellsModel (){}

    void CellsModel :: setCells ( std::vector<Cell *> cells ) {
        emit layoutAboutToBeChanged ();
        cells_ = cells ;
        emit layoutChanged ();
    }

    int CellsModel :: rowCount    ( const QModelIndex & parent) const
        { return ( not cells_.empty() ) ? cells_.size() : 0; }

    int CellsModel :: columnCount ( const QModelIndex & parent) const
        { return 1; }
        
    QVariant CellsModel :: data ( const QModelIndex & index, int role ) const
    {
        if ( cells_.empty() or not index . isValid ()) return QVariant ();
        if ( role == Qt :: DisplayRole ) {
            int row = index . row ();
            return cells_ [ row ] -> getName (). c_str ();
        }
        return QVariant ();
    }

    QVariant CellsModel :: headerData ( int section
    , Qt :: Orientation orientation
    , int role ) const
    {
        if ( orientation == Qt :: Vertical ) return QVariant ();
        if ( role != Qt :: DisplayRole ) return QVariant ();
        switch ( section ) {
        case 0: return " Cell " ;
        }
        return QVariant ();
    }


    Cell * CellsModel :: getModel ( int row ){
        if ( cells_.empty() ) return NULL ;
        if ( row >= ( int ) cells_.size()) return NULL ;
        return cells_[ row ];
    }

    void CellsModel::setCellViewer(CellViewer * cv){
        cellViewer_ = cv;
        setCells(cellViewer_->getCell()->getAllCells());
        connect ( cv , SIGNAL (cellLoaded()), this , SLOT ( updateDatas ()) );
    }

    void CellsModel::updateDatas(){
        setCells(cellViewer_->getCell()->getAllCells());
    }
}