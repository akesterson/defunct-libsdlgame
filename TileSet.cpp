#include "Display2D.h"
#include "TileSet.h"
#include <libxml/xmlreader.h>
#include <cstdlib>

int TileDisplay::loadFromTMX(std::string filename)
{
    xmlTextReaderPtr reader;
    const xmlChar *name;
    const xmlChar *value;
    int ret;

    reader = xmlReaderForFile(filename.c_str(), NULL, 0);
    if ( reader != NULL ) {
	ret = xmlTextReaderRead(reader);
	while ( ret == 1) {
	    name = xmlTextReaderConstName(reader);
	    if ( name != NULL ) {
		if ( !strcmp(name, "map") ) {
		    
		}
		value = xmlTextReaderConstValue(reader);
	    }
	    // process current node
	}
	xmlFreeTextReader(reader);
	if ( ret == 0 ) {
	    return 1;
	}
    }
    return 0;
}
