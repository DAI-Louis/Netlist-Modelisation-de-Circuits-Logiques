#pragma once
#include <string>
#include <libxml/xmlreader.h>
#include "Symbol.h"
#include "Shape.h"
#include "Box.h"
#include "XmlUtil.h"

namespace Netlist{
    class TermShape : public Shape {
        public :
            enum NameAlign { TopLeft=1, TopRight, BottomLeft, BottomRight };

            TermShape ( Symbol *, std::string name , int x , int y, NameAlign na );
            ~ TermShape ();
            Box getBoundingBox () const ;
            inline Term * getTerm () const ;
            inline int getX () const;
            inline int getY () const;
            inline NameAlign getAlign() const;
            inline void setX (int x);
            inline void setY (int y);
            static std::string toString(NameAlign a);
            static NameAlign toAlign(std::string s);
            void toXml(std::ostream& stream) const;
            static TermShape* fromXml( Symbol* s, xmlTextReaderPtr reader );
        private :
            Term * term_ ;
            int x_ , y_ ;
            NameAlign align_;
    };
    inline Term * TermShape::getTerm () const{ return term_; }
    inline int TermShape::getX () const{ return x_; }
    inline int TermShape::getY () const{ return y_; }
    inline TermShape::NameAlign TermShape::getAlign () const{ return align_; }
    inline void TermShape::setX (int x) { x_=x; }
    inline void TermShape::setY (int y) { y_=y; }
}