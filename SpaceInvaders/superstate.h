#include "shipState.h"

class superState : public ShipState
{
public:
    superState();
    virtual ~superState();

    int processShoot() {return 3;};
};

