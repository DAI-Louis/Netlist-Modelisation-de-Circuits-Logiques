#include <vector>
#include "Symbol.h"
#include "Shape.h"
#include "Term.h"
#include "TermShape.h"
#include "LineShape.h"
#include "BoxShape.h"
#include "EllipseShape.h"
#include "ArcShape.h"
#include "Cell.h"
#include "Box.h"

namespace Netlist {
    Symbol::Symbol(Cell* owner)
        :owner_(owner){}
    
    Symbol::~Symbol(){
        while (not shapes_.empty() ) delete *shapes_.begin();
    }

    Cell* Symbol::getCell() const{
        return owner_;
    }

    Box Symbol::getBoundingBox() const {
        Box b;
        for (size_t i =0; i<shapes_.size(); ++i){
             b.merge(shapes_[i]->getBoundingBox());
        }
        return b;
    }

    Point Symbol::getTermPosition(Term* t) const{
        return t->getPosition();
    }

    TermShape* Symbol::getTermShape(const Term* t) const{
        for (size_t i =0; i<shapes_.size(); ++i){
            TermShape* termShape = dynamic_cast<TermShape*>(shapes_[i]);
            if(termShape and /*(termShape->getTerm()->getCell() == t->getOwnerCell()) &&
                             (termShape->getTerm()->getName()     == t->getName())*/
                             termShape->getTerm() == t) return termShape;
        }
        return NULL;
    }

    void Symbol::add(Shape* s){
        if (s == nullptr) return;
        shapes_.push_back(s);
    }

    void Symbol::remove(Shape* s) {
        if (!s) return;
        for (auto it = shapes_.begin(); it != shapes_.end(); ++it) {
            if (*it == s) {
                shapes_.erase(it);
                return;
            }
        }
    }

    void Symbol::toXml(std::ostream& stream) const{
        for (size_t i =0; i<shapes_.size(); ++i)
            shapes_[i]->toXml(stream);
        stream << --indent;
    }

    Symbol* Symbol::fromXml(Cell* owner, xmlTextReaderPtr reader){
        //Cas si mauvais arg
        if (!owner || !reader) return nullptr;

        const xmlChar* boxTag     = xmlTextReaderConstString( reader, (const xmlChar*)"box" );
        const xmlChar* lineTag    = xmlTextReaderConstString( reader, (const xmlChar*)"line" );
        const xmlChar* termTag    = xmlTextReaderConstString( reader, (const xmlChar*)"term" );
        const xmlChar* ellipseTag = xmlTextReaderConstString( reader, (const xmlChar*)"ellipse" );
        const xmlChar* arcTag     = xmlTextReaderConstString( reader, (const xmlChar*)"arc" );

        Symbol* s = owner->getSymbol();

        /*Partie qui va chercher les Nodes*/
        while ( true ) {
            int status = xmlTextReaderRead(reader);
            if (status != 1) {
                if (status != 0) {
                    std::cerr << "[ERROR] Symbol::fromXml(): Unexpected termination of the XML parser." << std::endl;
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

            if      (nodeName == lineTag    ){ if(LineShape    :: fromXml(s, reader)) continue; }
            else if (nodeName == termTag    ){ if(TermShape    :: fromXml(s, reader)) continue; }
            else if (nodeName == boxTag     ){ if(BoxShape     :: fromXml(s, reader)) continue; }
            else if (nodeName == ellipseTag ){ if(EllipseShape :: fromXml(s, reader)) continue; }
            else if (nodeName == arcTag     ){ if(ArcShape     :: fromXml(s, reader)) continue; }
            
            std::cerr << "[ERROR] Symbol::fromXml(): Unknown or misplaced tag <" << nodeName
                      << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
            break;
        }   
        //Retourne le Net
        return s;
    }

}// namespace Netlist

