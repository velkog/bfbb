#include "xDecal.h"

#include <types.h>

//Doesn't OK if you use it from header, so assuming it's global.
extern U32 active_emitters_size;

void xDecalInit()
{
    active_emitters_size = 0;
}

bool xDecalEmitter::need_update() const
{
    bool result = false;
    if (!this->units.empty() || this->debug_need_update()) {
        goto need_update;
    }
    else {
        goto return_result;
    }

need_update:
    result = true;

return_result:
    return result;
}

void xDecalEmitter::debug_init(const char*) {}

bool xDecalEmitter::debug_need_update() const {
    return false;
}

void xDecalEmitter::debug_update_curve() {}

void xDecalEmitter::debug_update(F32) {}
