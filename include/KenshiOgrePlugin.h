#pragma once

#include <ogre/OgrePrerequisites.h>
#include <ogre/Ogre.h>
#include <ogre/OgrePlugin.h>

using namespace Ogre;

class KenshiPy : public Plugin
{
public:
    KenshiPy();

    const String& getName() const;

    const String& getVersion() const;

    void install();

    void initialise();

    void shutdown();

    void uninstall();
};
