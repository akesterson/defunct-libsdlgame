#include "Display2D.h"
#include "TileSet.h"
#include <libxml/xmlreader.h>
#include <cstdlib>

int TileDisplay::loadFromTMX(std::string filename)
{
    xmlTextReaderPtr reader;
    const xmlChar *name;
    const xmlChar *attr;
    const xmlChar *value;
    int ret;

    reader = xmlReaderForFile(filename.c_str(), NULL, 0);
    if ( reader == NULL )
      goto loadfromtmx_fail;
    ret = xmlTextReaderRead(reader);

    name = xmlTextReaderConstName(reader);
    // -- buildup the map flags
    if ( name != NULL ) {
      if ( !strcmp(name, "map") ) {
	goto loadfromtmx_fail;
      }
      attr = xmlTextReaderGetAttribute(reader, ATTR_ORIENTATION_KEY);
      if ( !strcmp(attr, ATTR_ORIENTATION_ORTHO) )
	this->flags = this->flags & TILEMAP_ORTHOGONAL;
      else if ( !strcmp(attr, ATTR_ORIENTATION_ISO) )
	this->flags = this->flags & TILEMAP_ISOMETRIC;
      else
	goto loadfromtmx_fail;
      this->w = atoi(xmlTextReaderGetAttribute(reader, ATTR_MAPWIDTH_KEY));
      this->h = atoi(xmlTextReaderGetAttribute(reader, ATTR_MAPHEIGHT_KEY));
      this->tw = atoi(xmlTextReaderGetAttribute(reader, ATTR_TILEWIDTH_KEY));
      this->th = atoi(xmlTextReaderGetAttribute(reader, ATTR_TILEHEIGHT_KEY));
    }

    // -- load the actual map
    while ( ret == 1) {
      name = xmlTextReaderConstName(reader);
      if ( name != NULL ) {
	if ( !strcmp(name, "map") ) {
	  goto loadfromtmx_fail;
	}
	value = xmlTextReaderConstValue(reader);
      }
      // process current node
    }
    xmlTextReaderClose(reader);
    xmlFreeTextReader(reader);
    return 0;
loadfromtmx_fail:
    ret = 1;
    if ( reader ) {
      xmlTextReaderClose(reader);
      xmlFreeTextReader(reader);
    }
    return ret;
}
