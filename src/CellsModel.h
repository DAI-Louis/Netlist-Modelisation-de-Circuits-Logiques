#pragma once
#include <QAbstractTableModel>
#include "Cell.h"
#include "CellViewer.h"

namespace Netlist{
    class CellViewer;
    class CellsModel : public QAbstractTableModel {
        Q_OBJECT ;
        public :
            CellsModel ( QObject * parent = NULL);
            ~CellsModel ();
            void setCells ( std::vector<Cell *> );
            Cell * getModel ( int row );
            int rowCount    ( const QModelIndex& parent = QModelIndex ()) const;
            int columnCount ( const QModelIndex& parent = QModelIndex ()) const;
            QVariant data ( const QModelIndex & index , int role = Qt::DisplayRole) const;
            QVariant headerData ( int section
            , Qt :: Orientation orientation
            , int role = Qt :: DisplayRole ) const ;
            void setCellViewer(CellViewer *);
            inline std::vector<Cell* > getCells();  

        public slots :
            void updateDatas();

        private :
            std::vector<Cell *> cells_ ;
            CellViewer * cellViewer_;
    };

    std::vector<Cell* > CellsModel::getCells() { return cells_;}
}