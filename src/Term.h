#pragma once
#include <string>
#include <vector>
#include <libxml/xmlreader.h>
#include "Indentation.h"
#include "Point.h"
#include "XmlUtil.h"

namespace Netlist {

    class Cell;
    class NodeTerm;
    class Instance;
    class Net;
    class Point;

    class Term {
        public :
            enum Type { Internal =1 , External =2 };
            enum Direction { In =1 , Out =2 , Inout =3 , Tristate =4 , Transcv =5, Unknown =6 };
        private :
            void * owner_ ;
            std :: string name_ ;
            Direction direction_ ;
            Type type_ ;
            Net * net_ ;
            NodeTerm * node_ ;

        public :
            Term ( Cell * , const std :: string & name , Direction );
            Term ( Instance * , const Term * modelTerm );
            ~ Term ();
            bool isInternal () const ;
            bool isExternal () const ;
            const std :: string & getName () const ;
            NodeTerm * getNode ();
            Net * getNet () const ;
            Cell * getCell () const ;
            Cell * getOwnerCell () const ;
            Instance * getInstance () const ;
            Direction getDirection () const ;
            Point getPosition () const ;
            Type getType () const ;
            void setNet ( Net * );
            void setNet ( const std :: string & );
            void setPosition ( const Point & );
            void setPosition ( int x , int y );
            void setDirection ( Direction );
            void toXml(std::ostream& stream);
            static std::string toString(Direction d);
            static std::string toString(Type);
            static Direction   toDirection(std::string);
            static Type        toType(std::string);
            static Term* fromXml(Cell*, xmlTextReaderPtr);
    };
}