#pragma once

#include <OgrePrerequisites.h>
#include <Ogre.h>
#include <OgrePlugin.h>

using namespace Ogre;

class KenshiPy_Plugin : public Plugin
{
public:
    KenshiPy_Plugin();

    const String& getName() const;

    void install();

    void initialise();

    void shutdown();

    void uninstall();
};