#pragma once

#include <ogre/OgrePrerequisites.h>
#include <ogre/Ogre.h>
#include <ogre/OgrePlugin.h>

using namespace Ogre;

class KenshiOgrePlugin : public Plugin
{
public:
    KenshiOgrePlugin();

    const String& getName() const;

    void install();

    void initialise();

    void shutdown();

    void uninstall();
};
