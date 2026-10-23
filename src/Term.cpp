#include "Term.h"
#include "TermShape.h"
#include "Instance.h"
#include "Cell.h"
#include "Node.h"
#include "XmlUtil.h"
#include "string"

namespace Netlist{
    Term::Term ( Cell* cell, const std::string & name , Direction dir)
        : owner_(cell), name_(name), direction_(dir), type_(External), net_(nullptr), node_(new NodeTerm(this, Node::noid)) 
    {
        cell->add(this);
    } 

    Term::Term ( Instance * instance, const Term * modelTerm )
        : owner_(instance), name_(modelTerm->name_), direction_(modelTerm->direction_), type_(Internal), net_(nullptr), node_(new NodeTerm(this, Node::noid)) 
    {
        this->setPosition(instance->getMasterCell()->getSymbol()->getTermShape(modelTerm)->getX(),
                          instance->getMasterCell()->getSymbol()->getTermShape(modelTerm)->getY());
    }

    Term::~Term (){
        if(net_!=nullptr && node_->getId()!=Node::noid){
            net_->remove(node_);
        }
        delete node_;
        if (isExternal()) getCell()->remove(this);
        else getInstance()->remove(this);
    }

    bool Term::isInternal () const{
        return (type_==Internal);
    }

    bool Term::isExternal () const{
        return (type_==External);
    }

    const std::string& Term::getName () const{
        return name_;
    }

    NodeTerm * Term::getNode (){
        return node_;
    }

    Net * Term::getNet () const{
        return net_;
    }

    Cell * Term::getCell () const{
        return isExternal() ? static_cast<Cell*>(owner_) : nullptr;
    }

    Cell * Term::getOwnerCell () const{
        return isInternal() ? (static_cast<Instance*>(owner_))->getMasterCell() : nullptr;
    }

    Instance * Term::getInstance () const{
        return isInternal() ? static_cast<Instance*>(owner_) : nullptr;
    }

    Term::Direction Term::getDirection () const{
        return direction_;
    }

    Point Term::getPosition () const{
        return node_->getPosition();
    }

    Term::Type Term::getType () const {
        return type_;
    }
    
    void Term::setNet ( Net * net){
        if(net==nullptr && node_->getId()!=Node::noid){
            net_->remove(node_);
            return;
        }
        net_ = net;
        net_->add(node_);
    }

    void Term::setNet ( const std::string & name){
        isExternal() ? net_ = getCell()->getNet(name) : net_ = getOwnerCell()->getNet(name);
        net_->add(node_);
    }

    void Term::setPosition ( const Point & Point){
        node_->setPosition(Point);
    }

    void Term::setPosition ( int x , int y ){
        node_->setPosition(x, y);
    }

    void Term::setDirection ( Direction dir){
        direction_ = dir;
    }

    std::string Term::toString(Direction d) {
        switch (d) {
            case In:        return "In";
            case Out:       return "Out";
            case Inout:     return "Inout";
            case Tristate:  return "Tristate";
            case Transcv:   return "Transcv";
            case Unknown:   return "Unknown";
            default:        return "Invalid";
        }
    }

    std::string Term::toString(Type t){
        return ((t == 1) ? "Internal" : "External");
    }

    Term::Direction Term::toDirection(const std::string s) {
        if (s == "In")            return Term::In;
        else if (s == "Out")      return Term::Out;
        else if (s == "Inout")    return Term::Inout;
        else if (s == "Tristate") return Term::Tristate;
        else if (s == "Transcv")  return Term::Transcv;
        else                      return Term::Unknown;
    }

    //TME6 pour avoir le type depuis une chaîne de caractère 
    Term::Type Term::toType(std::string s){
        if (s == "External") return Term::External;
        
        return Term::Internal;
    }

    void Term::toXml(std::ostream& stream){ 
        stream << indent << "<term name=\"" << name_ << "\" direction=\"" << toString(direction_) << "\" x=\"" << node_->getPosition().getX() << "\" y=\""<< node_->getPosition().getY() << "\"/>\n";
    }

    Term* Term::fromXml(Cell* owner, xmlTextReaderPtr reader){
        //Cas si mauvais arg
        if (!owner || !reader) return NULL;
        //Récupération du nom (ligne depuis Cell)
        std::string termName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
        //Récupération de la direction
        std::string termDir = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"direction" ) );
        //Récupération de x
        std:: string termX = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x" ) );
        //Récupération de y
        std:: string termY = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y" ) );
        
        //Cas si balise mal écrite
        if (termName.empty() || termDir.empty() || toDirection(termDir)==Term::Unknown || termX.empty() || termY.empty()) return NULL;

        //Création du terminal
        Term* t = new Term(owner ,termName, toDirection(termDir));
        //Changement de la position
        t->setPosition(stoi(termX), stoi(termY));
        //Tout c'est bien passé alors on le retourne
        return t;
    }
}