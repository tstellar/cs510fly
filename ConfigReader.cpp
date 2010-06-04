#include "ConfigReader.h"

ConfigReader::ConfigReader(const Ogre::String& filename) :
        configFile(), sectionIterator((configFile.load(filename), configFile.getSectionIterator())) { }

bool ConfigReader::hasNextSection() const {
    return sectionIterator.hasMoreElements();
}

void ConfigReader::advanceSection() {
    sectionIterator.moveNext();
}

Ogre::String ConfigReader::getSectionName() const {
    return sectionIterator.peekNextKey();
}

static Ogre::String identity(const Ogre::String& str) {
    return str;
}

Ogre::String ConfigReader::get(const Ogre::String& name, const Ogre::String& dflt) const {
    return parse(name, &identity, dflt);
}