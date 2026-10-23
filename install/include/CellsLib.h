#pragma once
#include <QMainWindow>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include "SaveCellDialog.h"
#include "CellsModel.h"
#include "CellViewer.h"


namespace Netlist{
    class CellViewer;
    class CellsModel;
    class CellsLib : public QWidget {
        Q_OBJECT;
    public:
                        CellsLib       ( QWidget* parent=NULL );
            void        setCellViewer  ( CellViewer* cv);
            int         getSelectedRow () const;
            inline CellsModel* getBaseModel   ();
    public slots:
            void        load           ();
    private:
        CellViewer*  cellViewer_;
        CellsModel*  baseModel_;
        QTableView*  view_;
        QPushButton* load_;
    };

    CellsModel* CellsLib::getBaseModel(){ return baseModel_;}
}

