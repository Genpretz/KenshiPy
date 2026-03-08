%{
#include "kenshi/PhysicsActual.h"
%}

%include "std_string.i"

// PhysX types (NxScene, NxActor, etc.) and Scythe namespace types are
// opaque C-side internal types; suppress the full class bodies.
// Expose only the NxShapeFlag enum and NxUserTriggerReport interface.

%ignore NxUserTriggerReport;

enum NxShapeFlag
{
    NX_TRIGGER_ON_ENTER             = 0x1,
    NX_TRIGGER_ON_LEAVE             = 0x2,
    NX_TRIGGER_ON_STAY              = 0x4,
    NX_TRIGGER_ENABLE               = 0x7,
    NX_SF_VISUALIZATION             = 0x8,
    NX_SF_DISABLE_COLLISION         = 0x10,
    NX_SF_FEATURE_INDICES           = 0x20,
    NX_SF_DISABLE_RAYCASTING        = 0x40,
    NX_SF_POINT_CONTACT_FORCE       = 0x80,
    NX_SF_FLUID_DRAIN               = 0x100,
    NX_SF_FLUID_DISABLE_COLLISION   = 0x400,
    NX_SF_FLUID_TWOWAY              = 0x800,
    NX_SF_DISABLE_RESPONSE          = 0x1000,
    NX_SF_DYNAMIC_DYNAMIC_CCD       = 0x2000,
    NX_SF_DISABLE_SCENE_QUERIES     = 0x4000,
    NX_SF_CLOTH_DRAIN               = 0xFFFF8000,
    NX_SF_CLOTH_DISABLE_COLLISION   = 0x10000,
    NX_SF_CLOTH_TWOWAY              = 0x20000,
    NX_SF_SOFTBODY_DRAIN            = 0x40000,
    NX_SF_SOFTBODY_DISABLE_COLLISION = 0x80000,
    NX_SF_SOFTBODY_TWOWAY           = 0x100000
};

#include "kenshi/PhysicsActual.h"