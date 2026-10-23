#ifndef NETLIST_NET_H
#define NETLIST_NET_H
#include <string>
#include <vector>
#include <libxml/xmlreader.h>
#include "Indentation.h"
#include "Term.h"
#include "Line.h"

namespace Netlist{

    class Cell;
    class Node;
    // class Term;

    class Net {
        public :
            Net ( Cell *, const std :: string &, Term :: Type );
            ~ Net ();
            Cell * getCell () const ;
            const std :: string & getName () const ;
            unsigned int getId () const ;
            Term :: Type getType () const ;
            Node * getNode ( size_t id ) const ;
            const std :: vector < Node * >& getNodes () const ;
            size_t getFreeNodeId () const ;
            void add ( Node * );
            bool remove ( Node * );
            void toXml(std::ostream& stream);
            static Net* fromXml(Cell*, xmlTextReaderPtr);
            inline const std::vector<Line*>& getLines () const;
            void  add ( Line* line );
            bool  remove( Line* line );
        // public :
        //     Net ( Cell * , const std :: string & name , Term :: Type dir );
        //     Net ( Instance * , const std :: string & name , Term :: Type dir );
        //     ~ Net ();
        //     private :
        //     Net ( const Net & );
            // ...
        private :
            Cell * owner_ ; 
            std :: string name_ ;
            unsigned int id_ ;
            Term :: Type type_ ;
            std::vector < Node * > nodes_ ;
            std::vector<Line*> lines_;
    };


    inline const std::vector<Line*>& Net::getLines () const { return lines_; }
}

#endif