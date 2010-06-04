#include "Ogre.h"

class ConfigReader {
    Ogre::ConfigFile configFile;
    mutable Ogre::ConfigFile::SectionIterator sectionIterator; // mutable because it's not const-correct ...
    
public:
    ConfigReader(const Ogre::String& filename);
    
    bool hasNextSection() const;
    void advanceSection();
    
    Ogre::String getSectionName() const;
    Ogre::String get(const Ogre::String& name, const Ogre::String& dflt = Ogre::StringUtil::BLANK) const;
    
    template<typename T>
    T parse(const Ogre::String& name, T (* func)(const Ogre::String&), const T& dflt) const {
        // Need to define this here so that the template can be instantiated. *grmbl*
    
        const Ogre::ConfigFile::SettingsMultiMap * settings = sectionIterator.peekNextValue();
        const Ogre::ConfigFile::SettingsMultiMap::const_iterator iter = settings->find(name);
        if (iter == settings->end())
            return dflt;
        else
            return func(iter->second);
    }
};