#pragma once
#include "Symbol.h"
#include <libxml/xmlreader.h>

namespace Netlist {
    class Shape {
        public :
        Shape ( Symbol * );
        virtual ~ Shape ();
        inline Symbol * getSymbol () const ;
        virtual Box getBoundingBox () const = 0;
        virtual void toXml( std::ostream& stream ) const = 0;
        static Shape* fromXml ( Symbol* owner, xmlTextReaderPtr reader );

        private :
        Symbol * owner_ ;
    };

    inline Symbol * Shape :: getSymbol () const { return owner_ ; };
}