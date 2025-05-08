#include "xDecal.h"

#include <types.h>

//Doesn't OK if you use it from header, so assuming it's global.
extern U32 active_emitters_size;

bool static_queue<xDecalEmitter::unit_data>::empty() const
{
    return this->size() == 0;
}

U32 static_queue<xDecalEmitter::unit_data>::size() const
{
    return this->_size;
}

void xDecalEmitter::debug_init(const char*)
{
}

bool xDecalEmitter::need_update() const
{
    bool result = false;

    if (!this->units.empty()) {
        goto result_true;
    }

    if (this->debug_need_update()) {
        goto result_true;
    }

    goto return_result;

result_true:
    result = true;

return_result:
    return result;
}

bool xDecalEmitter::debug_need_update() const
{
    return false;
}

void xDecalEmitter::debug_update_curve()
{
}

void xDecalEmitter::debug_update(F32)
{
}

void xDecalInit()
{
    active_emitters_size = 0;
}
