#include "shipState.h"

class defaultState : public ShipState
{
public:
    defaultState();
    virtual ~defaultState();

    int processShoot() {return 1;};
};

