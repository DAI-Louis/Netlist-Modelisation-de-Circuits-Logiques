#pragma once
#include <libxml/xmlreader.h>
#include "Symbol.h"
#include "Shape.h"
#include "Box.h"
#include "XmlUtil.h"

namespace Netlist{
    class BoxShape : public Shape {
        public :
            BoxShape ( Symbol *, const Box & );
            BoxShape ( Symbol *, int x1 , int y1 , int x2 , int y2 );
            ~ BoxShape ();
            Box getBoundingBox () const ;
            void toXml(std::ostream& stream) const;
            static BoxShape* fromXml(Symbol* s, xmlTextReaderPtr reader);
        private :
            Box box_ ;
    };
}