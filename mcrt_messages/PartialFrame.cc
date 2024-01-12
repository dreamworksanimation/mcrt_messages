// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0


#include "PartialFrame.h"

#include <stdint.h>

namespace mcrt {

ARRAS_CONTENT_IMPL(PartialFrame);

PartialFrame::~PartialFrame()
{

}

void
PartialFrame::serialize(arras4::api::DataOutStream& out) const
{
    ParentClass::serialize(out);
    out.write(mMachineId);
    out.write(mSnapshotStartTime);
}

void
PartialFrame::deserialize(arras4::api::DataInStream& in, unsigned version)
{
    ParentClass::deserialize(in,version);
    in.read(&mMachineId, sizeof(mMachineId));
    in.read(&mSnapshotStartTime, sizeof(mSnapshotStartTime));
}

} 

