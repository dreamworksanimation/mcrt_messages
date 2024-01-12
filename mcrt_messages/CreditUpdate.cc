// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "CreditUpdate.h"

namespace mcrt {

ARRAS_CONTENT_IMPL(CreditUpdate);

CreditUpdate::~CreditUpdate()
{
}


void
CreditUpdate::serialize(arras4::api::DataOutStream& out) const
{
    out << mName << static_cast<int>(mMode) << mValue;
}

void
CreditUpdate::deserialize(arras4::api::DataInStream& in, unsigned)
{
    int mode;
    in >> mName >> mode >> mValue;
    mMode = static_cast<CreditUpdateMode>(mode);
}

void
CreditUpdate::applyTo(int& counter,int initial) const
{
    switch (mMode) {
    case CreditUpdateMode::Update: counter += mValue; break;
    case CreditUpdateMode::Set: counter = mValue; break;
    case CreditUpdateMode::Reset: counter = initial; break; 
    } 
}

}
