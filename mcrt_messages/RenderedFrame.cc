// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "RenderedFrame.h"

namespace mcrt {

ARRAS_CONTENT_IMPL(RenderedFrame)

RenderedFrame::RenderedFrame()
{
}

RenderedFrame::~RenderedFrame()
{
}

size_t
RenderedFrame::serializedLength() const
{
    return ParentClass::serializedLength() + sizeof(mEye);
}

void
RenderedFrame:: serialize(arras4::api::DataOutStream& out) const
{
    ParentClass::serialize(out);
    out.write(mEye);
    out.write(mSnapshotStartTime);
}

void
RenderedFrame::deserialize(arras4::api::DataInStream& in, unsigned version)
{
    ParentClass::deserialize(in,version);
    in.read(&mEye, sizeof(mEye));
    in.read(&mSnapshotStartTime, sizeof(mSnapshotStartTime));
}

}
