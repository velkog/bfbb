#include "xDecal.h"

#include <types.h>

//Doesn't OK if you use it from header, so assuming it's global.
extern U32 active_emitters_size;

void xDecalEmitter::debug_init(const char*)
{
    return;
}

void xDecalInit()
{
    active_emitters_size = 0;
}
