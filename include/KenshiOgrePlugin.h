#pragma once

#include <OgrePrerequisites.h>
#include <Ogre.h>
#include <OgrePlugin.h>

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
