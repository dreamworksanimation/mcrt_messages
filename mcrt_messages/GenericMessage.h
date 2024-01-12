// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#ifndef ENGINE_MESSAGES_GENERICMESSAGE_H
#define ENGINE_MESSAGES_GENERICMESSAGE_H

#include <message_api/ContentMacros.h>
#include <string>

namespace mcrt {

    class GenericMessage : public arras4::api::ObjectContent
    {
    public:
        ARRAS_CONTENT_CLASS(GenericMessage, "a840c95b-8ea3-4954-9c3d-14c381b7d022",0);
        ~GenericMessage();

        // Message implementation
        void serialize(arras4::api::DataOutStream& to) const;
        void deserialize(arras4::api::DataInStream& from, unsigned version);

        std::string mValue;
    };
}

#endif 

