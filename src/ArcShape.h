#pragma once
#include <libxml/xmlreader.h>
#include "Symbol.h"
#include "Shape.h"
#include "Box.h"
#include "XmlUtil.h"

namespace Netlist{
    class ArcShape : public Shape {
        public :
            ArcShape ( Symbol *, const Box &, int start, int span );
            ArcShape ( Symbol *, int x1 , int y1 , int x2 , int y2, int start, int span );
            ~ ArcShape ();
            Box getBoundingBox () const ;
            void toXml(std::ostream& stream) const;
            static ArcShape* fromXml(Symbol* s, xmlTextReaderPtr reader);
            inline int getStart();
            inline int getSpan();
        private :
            Box box_ ;
            int start_, span_;
    };

    int ArcShape::getStart(){return start_;}
    int ArcShape::getSpan() {return span_;}
}