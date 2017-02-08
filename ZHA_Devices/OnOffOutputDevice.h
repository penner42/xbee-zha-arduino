#include "../ZHA_Devices.h"
#include "../ZHA_Clusters/BasicCluster.h"
#include "../ZHA_Clusters/IdentifyCluster.h"
#include "../ZHA_Clusters/OnOffCluster.h"
#include "../ZHA_Clusters/ScenesCluster.h"
#include "../ZHA_Clusters/GroupsCluster.h"
#include "../ZHA_Clusters/LevelControlCluster.h"
#include "../ZHA_Clusters/ZLLCommissioningCluster.h"

class OnOffOutputDevice:public ZHA_Device {
  public:
    void on();
    void off();
    void toggle();
};

