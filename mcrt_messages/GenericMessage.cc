// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "GenericMessage.h"

namespace mcrt {

ARRAS_CONTENT_IMPL(GenericMessage);

GenericMessage::~GenericMessage()
{
}


void
GenericMessage::serialize(arras4::api::DataOutStream& out) const
{
    out.write(mValue);
}

void
GenericMessage::deserialize(arras4::api::DataInStream& in, unsigned)
{
    in.read(mValue);
}

}
