#include "Instance.h"
#include "Cell.h"
#include "TermShape.h"
#include <iostream>

namespace Netlist{
    
    Instance::Instance( Cell * owner , Cell * model , const std :: string & name)
        : owner_(owner), masterCell_(model), name_(name), terms_(), position_(new Point())
    {
        owner->add(this);
        for (Term* t : model->getTerms()){
            (*this).add(t);
        }
    }

    Instance::~Instance(){
        while ( not terms_.empty() ) delete *terms_.begin();
        owner_->remove(this);
    }

    const std :: string & Instance::getName () const{
        return name_;
    }

    Cell * Instance::getMasterCell () const{
        return masterCell_;
    }

    Cell * Instance::getCell () const{
        return owner_;
    }
    
    const std::vector < Term * >& Instance::getTerms () const{
        return terms_;
    }

    Term * Instance::getTerm ( const std :: string & name) const{
        for(Term* t : terms_){
            if(t->getName() == name){
                return t;
            }
        }
        return nullptr;
    }

    Point Instance::getPosition () const{
        return *(position_);
    }

    bool Instance::connect ( const std :: string & name , Net * net){
        Term* term = getTerm( name );
        if (term == NULL) return false;
 
        term->setNet( net );
        return true;
    }

    void Instance::add (Term * t){
        terms_.push_back(new Term(this, t));
    }

    void Instance::remove ( Term * t){
        for ( std::vector<Term*>::iterator iterm=terms_.begin() ; iterm != terms_.end() ; ++iterm ) {
            if (*iterm == t){
                terms_.erase( iterm );
                return;
            }
        }
    }

    void Instance::setPosition ( const Point& p){
        int oldx = position_->getX();
        int oldy = position_->getY();

        position_ = (Point*)(&p);

        int distx = position_->getX()-oldx,
            disty = position_->getY()-oldy;

        // Symbol* s = masterCell_->getSymbol();

        for(Term* t : terms_){
            // TermShape* tShape = s->getTermShape(t);
            // tShape->setX(tShape->getX()+distx);
            // tShape->setY(tShape->getY()+disty);
            t->getPosition().translate(distx, disty);
            // t->setPosition(p);
        }
    }

    void Instance::setPosition ( int x , int y ){
        int oldx = position_->getX();
        int oldy = position_->getY();

        position_ = new Point(x, y);

        int distx = x-oldx, disty = y-oldy;

        // Symbol* s = masterCell_->getSymbol();

        for(Term* t : terms_){
            // t->setPosition(x,y);
            // t->getPosition().translate(distx, disty);
            t->setPosition(t->getPosition().getX()+distx, t->getPosition().getY()+disty);    
            // if(TermShape* tShape = s->getTermShape(t)){
            //     tShape->setX(tShape->getX()+distx);
            //     tShape->setY(tShape->getY()+disty);
            //     std::cout << "DANS LA BOUCLE" << std::endl;
            //     // t->getPosition().translate(distx, disty);
            // }
        }
        
    }

    void Instance::toXml(std::ostream& stream){ 
        stream << indent <<"<instance name=\"" << name_ << "\" mastercell=\"" << masterCell_->getName() << "\" " << "x=\"" << position_->getX() << "\" y=\"" << position_->getY() << "\"/>\n";
    }

    Instance* Instance::fromXml(Cell* owner, xmlTextReaderPtr reader){
        //Cas si mauvais arg
        if(!owner or !reader) return NULL;
        //Récupération du nom
        std::string instName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );

        //Récupération de la masterCell
        std::string mCellName = xmlCharToString(xmlTextReaderGetAttribute( reader, (const xmlChar*)"mastercell" ));
        Cell* instMasterCell;
        if( !(instMasterCell=Cell::find(mCellName)) ) /* Si la MasterCell n'existe pas on la charge */
            if( !(instMasterCell=Cell::load(mCellName)) ) return NULL;

        //Récupération de x
        std::string instX = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x" ) );
        //Récupération de y
        std::string instY = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y" ) );


        //Cas si balise mal écrite
        if(instName.empty() || instX.empty() || instY.empty()) return NULL;

        //Création de l'Instance
        Instance* i = new Instance(owner, instMasterCell, instName);
        //Changement de la position
        i->setPosition(stoi(instX), stoi(instY));
        //Tout c'est bien passé alors on le retourne
        return i;

    }
}