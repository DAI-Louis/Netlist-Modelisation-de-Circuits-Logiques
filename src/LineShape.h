#include <libxml/xmlreader.h>
#include "Symbol.h"
#include "Shape.h"
#include "Box.h"
#include "XmlUtil.h"

namespace Netlist{
    class LineShape : public Shape {
        public :
            LineShape ( Symbol *, int x1 , int y1 , int x2 , int y2 );
            ~ LineShape ();
            Box getBoundingBox () const ;
            void toXml(std::ostream& stream) const;
            static LineShape* fromXml( Symbol*, xmlTextReaderPtr );
            inline int getX1();
            inline int getX2();
            inline int getY1();
            inline int getY2();
        private :
            int x1_ , y1_ , x2_ , y2_ ;
    };

    int LineShape::getX1(){return x1_;}
    int LineShape::getX2(){return x2_;}
    int LineShape::getY1(){return y1_;}
    int LineShape::getY2(){return y2_;}
}