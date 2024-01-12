// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#ifndef __ARRAS4_PROGRESS_MESSAGEH__
#define __ARRAS4_PROGRESS_MESSAGEH__

#include <message_api/ContentMacros.h>
#include <message_api/Object.h>
#include <string>

namespace mcrt {

class ProgressMessage : public arras4::api::ObjectContent
{
public:

    ARRAS_CONTENT_CLASS(ProgressMessage,"54dbf584-67df-4ec4-ac9f-7ccb7951e0e1",0);
    
    ProgressMessage() {}

    ~ProgressMessage() {}

    void serialize(arras4::api::DataOutStream& to) const;
    void deserialize(arras4::api::DataInStream& from, unsigned version);

    arras4::api::ObjectRef object() { return mObject; }
    arras4::api::ObjectConstRef object() const { return mObject; }

private:
    arras4::api::Object mObject;
};

}

#endif
