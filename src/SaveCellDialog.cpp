#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "SaveCellDialog.h"

//On met des commentaires pour comprendre le code 

namespace Netlist{

    /*Sauvegarde d'une Cell*/
    SaveCellDialog :: SaveCellDialog ( QWidget * parent )
    : QDialog ( parent )
    , lineEdit_ ( NULL )
    {
        /*Titre de la fenêtre*/
        setWindowTitle ( tr ( " Save ␣ Cell " ) );
        /*Création d'un label*/
        QLabel * label = new QLabel ();
        /*Donné un nom au label*/
        label->setText ( tr ( " Enter ␣ Cell ␣ name ␣ ( without ␣ extention ) " ) );
        /*Création d'un objet qui permet de décrire une line*/
        lineEdit_ = new QLineEdit ();
        /*Longueur minimum à 400*/
        lineEdit_->setMinimumWidth ( 400 );

        /*Création du bouton okay et le set-up*/
        QPushButton * okButton = new QPushButton ();
        okButton->setText ( " OK " );
        okButton->setDefault ( true );

        /*Création du bouton nope et le set-up*/
        QPushButton * cancelButton = new QPushButton ();
        cancelButton->setText ( " Cancel " );

        /*Création d'une direction et d'un parent widget, et configuration de celui-ci*/
        QHBoxLayout * hLayout = new QHBoxLayout ();
        hLayout->addStretch ();
        hLayout->addWidget ( okButton );
        hLayout->addStretch ();
        hLayout->addWidget ( cancelButton );
        hLayout->addStretch ();
        QFrame * separator = new QFrame ();
        separator->setFrameShape ( QFrame :: HLine );
        separator->setFrameShadow ( QFrame :: Sunken );

        /*Afficher les boutons séparés*/
        QVBoxLayout * vLayout = new QVBoxLayout ();
        vLayout->setSizeConstraint ( QLayout :: SetFixedSize );
        vLayout->addWidget ( label );
        vLayout->addWidget ( lineEdit_ );
        vLayout->addLayout ( hLayout );
        setLayout ( vLayout );
        connect ( okButton , SIGNAL ( clicked ()) , this , SLOT ( accept ()) );
        connect ( cancelButton , SIGNAL ( clicked ()) , this , SLOT ( reject ()) );
    }

    const QString SaveCellDialog :: getCellName () const
    { return lineEdit_->text (); }

    void SaveCellDialog :: setCellName ( const QString & name )
    { return lineEdit_->setText ( name ); }

    bool SaveCellDialog :: run ( QString & name )
    {
        setCellName ( name );
        int dialogResult = exec ();
        name = getCellName ();
        return ( dialogResult == Accepted );
    }
}