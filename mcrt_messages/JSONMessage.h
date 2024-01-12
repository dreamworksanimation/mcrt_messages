// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0


#pragma once

#include <json/value.h>

#include <message_api/ContentMacros.h>

#include <string>

namespace mcrt {

class JSONMessage : public arras4::api::ObjectContent
{
public:

    ARRAS_CONTENT_CLASS(JSONMessage, "e5152868-dad0-4879-86b3-f44ddf312eb4",0);
    JSONMessage();
    ~JSONMessage();
 
    static const char* getMessageRoot();
    static const char* getMessageId();
    static const char* getMessageName();
    static const char* getMessagePayload();

    static const char* MESSAGE_ROOT;
    static const char* MESSAGE_ID;
    static const char* MESSAGE_NAME;
    static const char* MESSAGE_PAYLOAD;

    void serialize(arras4::api::DataOutStream& to) const;
    void deserialize(arras4::api::DataInStream& from, unsigned version);


    static JSONMessage::Ptr create(const std::string& id,
                                   const std::string& name);

    std::string toString() const;

    std::string messageId() const;
    std::string messageName() const;
    Json::Value& messagePayload();
    const Json::Value& messagePayload() const;

    std::string mClientData;
    Json::Value mRoot;

};

} 
