// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0


#pragma once

#include <message_api/ContentMacros.h>

#include <string>

namespace mcrt {

class RDLMessage : public arras4::api::ObjectContent
{
public:
    ARRAS_CONTENT_CLASS(RDLMessage, "3a1e0d57-871d-4f29-b3f5-7de6fdeb190a",0);
 
    RDLMessage() : mForceReload(false) {}

    void serialize(arras4::api::DataOutStream& to) const;
    void deserialize(arras4::api::DataInStream& from, unsigned version);

    // required for legacy message chunking
    size_t serializedLength() const;

    // These are byte strings, not ASCII strings.
    std::string mManifest;
    std::string mPayload;
    bool mForceReload;

    // This is used to sync mcrt result logic @ merger computation
    int mSyncId; // default is -1
};

} 

