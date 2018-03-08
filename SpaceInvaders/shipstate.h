#ifndef SHIPSTATE_H
#define SHIPSTATE_H


class ShipState
{
public:
    ShipState() {}
    virtual ~ShipState() {}

    virtual int processShoot() = 0;
};

#endif // SHIPSTATE_H
