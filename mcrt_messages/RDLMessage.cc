// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0


#include "RDLMessage.h"

namespace mcrt {

ARRAS_CONTENT_IMPL(RDLMessage);

void RDLMessage::serialize(arras4::api::DataOutStream& out) const
{
    out.write(mSyncId);
    out.writeLongString(mManifest);
    out.writeLongString(mPayload);
    out.write(mForceReload);
}

void RDLMessage::deserialize(arras4::api::DataInStream& in, unsigned)
{
    in.read(mSyncId);
    in.readLongString(mManifest);
    in.readLongString(mPayload);
    in.read(mForceReload);
}

size_t RDLMessage::serializedLength() const
{
    return (sizeof(mSyncId) +
            sizeof(size_t) + mManifest.length() + 
            sizeof(size_t) + mPayload.length() +
            sizeof(mForceReload));
}

} 

