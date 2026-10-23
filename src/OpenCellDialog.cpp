#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialog>
#include "OpenCellDialog.h"

namespace Netlist{
    OpenCellDialog :: OpenCellDialog ( QWidget * parent )
    : QDialog ( parent )
    , lineEdit_ ( NULL )
    {   
        //Création du titre
        setWindowTitle ( tr ( " Open Cell " ) );
        //Création de la fenêtre
        QLabel * label = new QLabel ();
        //Partie texte
        label->setText ( tr ( " Enter Cell name ( without extention ) " ) );
        lineEdit_ = new QLineEdit ();
        lineEdit_->setMinimumWidth ( 400 );
        //Bouton Okay
        QPushButton * okButton = new QPushButton ();
        okButton->setText ( " OK " );
        okButton->setDefault ( true );
        //Bouton Cancel
        QPushButton * cancelButton = new QPushButton ();
        cancelButton->setText ( " Cancel " );
        //Positionnement des objets
        QHBoxLayout * hLayout = new QHBoxLayout ();
        hLayout->addStretch ();
        hLayout->addWidget ( okButton );
        hLayout->addStretch ();
        hLayout->addWidget ( cancelButton );
        hLayout->addStretch ();
        QFrame * separator = new QFrame ();
        separator->setFrameShape ( QFrame :: HLine );
        separator->setFrameShadow ( QFrame :: Sunken );
        QVBoxLayout * vLayout = new QVBoxLayout ();
        vLayout->setSizeConstraint ( QLayout :: SetFixedSize );
        vLayout->addWidget ( label );
        vLayout->addWidget ( lineEdit_ );
        vLayout->addLayout ( hLayout );
        setLayout ( vLayout );
        connect ( okButton , SIGNAL ( clicked ()) , this , SLOT ( accept ()) );
        connect ( cancelButton , SIGNAL ( clicked ()) , this , SLOT ( reject ()) );

    }

    const QString OpenCellDialog :: getCellName () const
    { return lineEdit_->text (); }

    // void OpenCellDialog :: setCellName ( const QString & name )
    // { return lineEdit_->setText ( name ); }


    QString& OpenCellDialog::run(OpenCellDialog* dialog)
{
    static QString result;

    if (dialog->exec() == QDialog::Accepted)
        result = dialog->getCellName();
    else
        result.clear();
    //Sert pour le debug pourra être enlevé plus tard
    // std::cout << result.toStdString () << std::endl;
    return result;
}

}