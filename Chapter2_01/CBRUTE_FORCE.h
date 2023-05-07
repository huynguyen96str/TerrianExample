#ifndef CBRUTE_FORCE_H
#define CBRUTE_FORCE_H

#include "CTERRAIN.h"

class CBRUTE_FORCE: public CTERRAIN
{
public:
    CBRUTE_FORCE();

    // CTERRAIN interface
public:
    void Render() override;
};

#endif // CBRUTE_FORCE_H
