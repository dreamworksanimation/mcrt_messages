// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0


#include "ViewportMessage.h"

namespace mcrt {

ARRAS_CONTENT_IMPL(ViewportMessage);

void ViewportMessage::serialize(arras4::api::DataOutStream& out) const
{
    out << mX << mY << mW << mH;
}

void ViewportMessage::deserialize(arras4::api::DataInStream& in, unsigned)
{
    in >> mX >> mY >> mW >> mH;
}

} 

