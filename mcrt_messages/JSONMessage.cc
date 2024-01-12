// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0


#include "JSONMessage.h"

#include <message_api/MessageFormatError.h>

#include <json/reader.h>
#include <sstream>

#if !defined(JSONCPP_VERSION_MAJOR)
// Newer jsoncpp versions fix the spelling error.
#define getFormattedErrorMessages  getFormatedErrorMessages
#endif

namespace mcrt {

ARRAS_CONTENT_IMPL(JSONMessage);

namespace {

    void
    throwMessageFormatError(const Json::Value& val, const std::string& key)
    {
        std::ostringstream oss;
        oss << "Error parsing JSON for message. " << "Expected: " << key
                << " not found in " << val.asString() << " node.";
        throw arras4::api::MessageFormatError(oss.str());
    }


    Json::Value
    fromJsonValue(const Json::Value& val, const std::string& key)
    {
        Json::Value result = val[key];
        if (result.isNull()) {
            throwMessageFormatError(val, key);
        }
        return result;
    }

    const std::string
    fromJsonString(const Json::Value& val, const std::string& key)
    {
        auto result = fromJsonValue(val, key);
        if (!result.isString()) {
            throwMessageFormatError(val, key);
        }
        return result.asString();
    }

    void
    validateMessage(const std::string& val, Json::Value& root)
    {
        Json::Reader reader;
        bool isValidJson = reader.parse(val, root);
        if (!isValidJson ||
            root[JSONMessage::MESSAGE_ROOT].isNull() ||
            root[JSONMessage::MESSAGE_ROOT][JSONMessage::MESSAGE_ID].isNull() ||
            root[JSONMessage::MESSAGE_ROOT][JSONMessage::MESSAGE_NAME].isNull()) {

            std::ostringstream oss;
            oss << "JSON Parse Error!" << reader.getFormattedErrorMessages();
            throw arras4::api::MessageFormatError(oss.str());
        }
    }

}

JSONMessage::JSONMessage()
{
    arras4::api::UUID uuid;
    uuid.regenerate();
    mClientData = uuid.toString();
}

JSONMessage::~JSONMessage()
{
}

const char* JSONMessage::MESSAGE_ROOT = "message";
const char* JSONMessage::MESSAGE_ID = "id";
const char* JSONMessage::MESSAGE_NAME = "name";
const char* JSONMessage::MESSAGE_PAYLOAD = "payload";

void
JSONMessage::serialize(arras4::api::DataOutStream& out) const
{
    out.write(mClientData);
    out.write(toString());
}

void
JSONMessage::deserialize(arras4::api::DataInStream& in, unsigned)
{
    in.read(mClientData);
    std::string result;
    in.read(result);
    validateMessage(result, mRoot);
}

/* static */
JSONMessage::Ptr
JSONMessage::create(const std::string& id, const std::string& name)
{
    JSONMessage* msg = new JSONMessage;
    msg->mRoot[MESSAGE_ROOT][MESSAGE_ID] = id;
    msg->mRoot[MESSAGE_ROOT][MESSAGE_NAME] = name;
    msg->mRoot[MESSAGE_ROOT][MESSAGE_PAYLOAD] = Json::Value();
    return JSONMessage::Ptr(msg);
}

std::string
JSONMessage::toString() const
{
    return mRoot.toStyledString();
}


std::string
JSONMessage::messageId() const
{
    return fromJsonString(mRoot[MESSAGE_ROOT], MESSAGE_ID);
}

std::string
JSONMessage::messageName() const
{
    return fromJsonString(mRoot[MESSAGE_ROOT], MESSAGE_NAME);
}

Json::Value&
JSONMessage::messagePayload()
{
    return mRoot[MESSAGE_ROOT][MESSAGE_PAYLOAD];
}

const Json::Value&
JSONMessage::messagePayload() const
{
    return mRoot[MESSAGE_ROOT][MESSAGE_PAYLOAD];
}

const char* JSONMessage::getMessageRoot() 
{
    return MESSAGE_ROOT;
}

const char* JSONMessage::getMessageId()
{
    return MESSAGE_ID;
}

const char* JSONMessage::getMessageName()
{
    return MESSAGE_NAME;
}

const char* JSONMessage::getMessagePayload()
{
    return MESSAGE_PAYLOAD;
}


} 

