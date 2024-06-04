#include "serverControl.h"
#include "serverData.h"

serverControl::serverControl(const serverData& linkedData) : gameTile(linkedData.serverName),
linkedData(linkedData)
{

}

