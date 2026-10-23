#include "TermShape.h"
#include "Cell.h"

namespace Netlist{
    TermShape :: TermShape ( Symbol * owner , std::string name , int x1 , int y1, NameAlign na)
    : Shape ( owner ) , term_ ( NULL ), x_ ( x1 ), y_ ( y1 ), align_(na)
    {
        Cell * cell = owner->getCell ();
        term_ = cell -> getTerm ( name );
    }

    TermShape ::~ TermShape () { }

    Box TermShape :: getBoundingBox () const
    { return Box ( x_ , y_ , x_ , y_ ); }

    std::string TermShape::toString(NameAlign a) {
        switch (a) {
            case TopLeft:       return "top_left";
            case TopRight:      return "top_right";
            case BottomLeft:    return "bottom_left";
            case BottomRight:   return "bottom_right";
            default:            return "Invalid";
        }
    }

    TermShape::NameAlign TermShape::toAlign(std::string s) {
        if      (s == "top_left")       return TopLeft;
        else if (s == "top_right")      return TopRight;
        else if (s == "bottom_left")    return BottomLeft;
        else                           return BottomRight;
    }

    void TermShape::toXml(std::ostream& stream) const{
        stream << indent << "<term name=\"" << term_->getName() << "\" x1=\"" << x_ << "\" y1=\"" << y_ << "\" align=\"" << toString(align_) << "\" />\n";
    }

    TermShape* TermShape::fromXml( Symbol* s, xmlTextReaderPtr reader ){
        //Cas si mauvais arg
        if(!s or !reader) return NULL;
        
        //Récupération du nom
        std::string name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
        //Récupération de x1
        std::string x1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
        //Récupération de y1
        std::string y1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
        //Récupération de l'alignement
        std::string align = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"align" ) );

        //Cas si mauvaise valeur
        if (x1.empty() || y1.empty() || (align != "top_right"    && align != "top_left" &&
                                         align != "bottom_right" && align != "bottom_left")) return NULL;


        return new TermShape(s, name, stoi(x1), stoi(y1), toAlign(align));
    }
}