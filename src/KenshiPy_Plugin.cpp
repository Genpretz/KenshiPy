#include "KenshiPy_Plugin.h"
#include "dllmain.h"

const String sPluginName = "KenshiPy_Plugin";
//---------------------------------------------------------------------
KenshiPy_Plugin::KenshiPy_Plugin()
{
}
//---------------------------------------------------------------------
const String& KenshiPy_Plugin::getName() const
{
	return sPluginName;
}
//---------------------------------------------------------------------
void KenshiPy_Plugin::install()
{
	//Init();
}
//---------------------------------------------------------------------
void KenshiPy_Plugin::initialise()
{
	//intialise called - register stuff here
}
//---------------------------------------------------------------------
void KenshiPy_Plugin::shutdown()
{
	//ShutdownPython();
}
//---------------------------------------------------------------------
void KenshiPy_Plugin::uninstall()
{
	//uninstall called - delete stuff here
}