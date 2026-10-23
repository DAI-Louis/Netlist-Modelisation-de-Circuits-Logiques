#include "Net.h"
#include "Cell.h"
#include "Node.h"


namespace Netlist{

    Net::Net(Cell * cell, const std :: string & name, Term :: Type type)
        : owner_(cell), name_(name), id_(cell->newNetId()), type_(type), nodes_()
    {
        cell->add(this);
    }

    Net::~Net(){
        for(Node* n : nodes_){
            if(NodeTerm* nt = dynamic_cast<NodeTerm*>(n))
                nt->getTerm()->setNet(nullptr);
        }
        owner_->remove(this);
    }

    Cell* Net::getCell() const{
        return owner_;
    }

    const std::string& Net::getName() const{
        return name_;
    }

    unsigned int Net::getId() const{
        return id_;
    }

    Term :: Type Net::getType() const{
        return type_;
    }

    Node* Net::getNode ( size_t id ) const{
        return nodes_[id];
    }

    const std :: vector < Node * >& Net::getNodes () const{
        return nodes_;
    }
    size_t Net::getFreeNodeId () const {
        for(size_t i = 0; nodes_.begin()+i != nodes_.end(); i++){
            if(nodes_[i] == NULL){
                return i;
            }
        }
        return nodes_.size();
    }

    void Net::add ( Node * n ){
        size_t id = n->getId();
        if (id != Node::noid){
            if(id >= nodes_.size()){
                nodes_.resize(id+1, nullptr);
                nodes_[id] = n;
                return;
            } else if (nodes_[id]==nullptr){ nodes_[id] = n; return;} 
            else {
                nodes_[id] = n;
                return;
            }
        }
        if (int ind = getFreeNodeId() < nodes_.size()){
            nodes_[ind] = n;
            n->setId(ind);
        }
        else{
            n->setId(nodes_.size());
            nodes_.push_back(n);
        }
        
    }

    bool Net::remove ( Node * n){
        size_t ind = n->getId();
        if (ind < nodes_.size() && nodes_[ind] == n){
            nodes_[ind] = nullptr;
            return true;
        }
        return false;
    }

    void Net::toXml(std::ostream& stream){
        stream << indent++ << "<net name=\"" << name_ << "\" type =\"" << ((type_ == 1) ? "Internal" : "External") << "\">\n";
        for(Node* n : nodes_){
            if (n) n->toXml(stream);
        }
        for(Line* l : lines_){
            if (l) l->toXml(stream);
        }
        stream << --indent << "</net>\n";
    }

    Net* Net::fromXml(Cell* owner, xmlTextReaderPtr reader){
        //Cas si mauvais arg
        if (!owner || !reader) return NULL;
        //Récupération du nom
        std::string netName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
        //Récupération du Type
        std::string netType = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"type" ) );

        //Cas si balise mal écrite
        if (netName.empty() || (netType != "External" && netType != "Internal")) return NULL;
        //Création du Net
        Net* n = new Net(owner, netName, Term::toType(netType));

        const xmlChar* nodeTag = xmlTextReaderConstString( reader, (const xmlChar*)"node" );
        const xmlChar* lineTag = xmlTextReaderConstString( reader, (const xmlChar*)"line" );

        /*Partie qui va chercher les Nodes*/
        while ( true ) {
            int status = xmlTextReaderRead(reader);
            if (status != 1) {
                if (status != 0) {
                    std::cerr << "[ERROR] Net::fromXml(): Unexpected termination of the XML parser." << std::endl;
                }
            break;
            }

            if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT) break;

            switch ( xmlTextReaderNodeType(reader) ) {
                case XML_READER_TYPE_COMMENT:
                case XML_READER_TYPE_WHITESPACE:
                case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
                continue;
            }

            const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );

            if(nodeName == nodeTag){ if(Node::fromXml(n ,reader)) continue; }
            else if(nodeName == lineTag ){ if(Line::fromXml(n, reader)) continue; }

            std::cerr << "[ERROR] Net::fromXml(): Unknown or misplaced tag <" << nodeName
                      << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
            break;
        }   
        //Retourne le Net
        return n;
    }


    void  Net::add ( Line* line ) { if (line) lines_.push_back( line ); }

    bool  Net::remove ( Line* line )
    {
    if (line) {
        for ( std::vector<Line*>::iterator il = lines_.begin()
            ; il != lines_.end() ; ++il ) {
        if (*il == line) {
            lines_.erase( il );
            return true;
        }
        }
    }
    return false;
    }
}