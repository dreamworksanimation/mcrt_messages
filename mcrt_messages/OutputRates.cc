// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0


#include "OutputRates.h"
#include <message_api/MessageFormatError.h>
#include <arras4_log/Logger.h>

#if defined(JSONCPP_VERSION_MAJOR)
#define memberName name
#endif

namespace mcrt {

const char* OutputRates::SET_OUTPUT_RATES_ID = "67e036dc-cc53-4d9b-bbc0-2c00aaf97445";
const char* OutputRates::SET_OUTPUT_RATES_NAME = "SetOutputRates";

namespace {
    OutputRates::IntervalAndOffset rateFromJson(const Json::Value& val)
    {
         if (!val.isObject()) {
             ARRAS_LOG_ERROR("In SetOutputRates:  rates value must be an object");
             throw arras4::api::MessageFormatError("In SetOutputRates:  rates value must be an object");
         }
         const Json::Value& interval = val["interval"];
         const Json::Value& offset = val["offset"];
         if (!interval.isInt() || !offset.isInt()) {
             ARRAS_LOG_ERROR("In SetOutputRates:  'interval' and 'offset' values must be integers");
             throw arras4::api::MessageFormatError("In SetOutputRates:  'interval' and 'offset' values must be integers");
         }
         return OutputRates::IntervalAndOffset(interval.asInt(),offset.asInt());
    }
    Json::Value rateToJson(const OutputRates::IntervalAndOffset& rate) 
    {
        Json::Value jrate(Json::objectValue);
        jrate["interval"] = rate.interval;
        jrate["offset"] = rate.offset;
        return jrate;
    }
}

void
OutputRates::setFromMessage(JSONMessage::ConstPtr jm)
{
    mOutputRates.clear();
    mDefaultRate = IntervalAndOffset(1,0);
    mSendAllWhenComplete = true;

    const Json::Value& payload = jm->messagePayload();
    if (payload.isMember("outputs")) {
        const Json::Value& outputs = payload["outputs"];
        if (!outputs.isObject()) {
            ARRAS_LOG_ERROR("In SetOutputRates:  'outputs' field must be an object");
            throw arras4::api::MessageFormatError("In SetOutputRates:  'outputs' field must be an object");
        }
        for (auto it = outputs.begin(); it != outputs.end(); ++it) {
            mOutputRates.insert(OutputRateMap::value_type(it.memberName(), rateFromJson(*it)));
        }
        mDefaultRate = rateFromJson(payload["defaultRate"]);
        const Json::Value& sendAll = payload["sendAllWhenComplete"];
        if (!sendAll.isBool()) {
            ARRAS_LOG_ERROR("In SetOutputRates:  'sendAllWhenComplete' field must be a boolean");
            throw arras4::api::MessageFormatError("In SetOutputRates:  'outputs' field must be a boolean");
        }
        mSendAllWhenComplete = sendAll.asBool();
    }
}
        
JSONMessage::Ptr 
OutputRates::getAsMessage() const
{ 
    JSONMessage::Ptr msg = JSONMessage::create(SET_OUTPUT_RATES_ID,SET_OUTPUT_RATES_NAME);
    Json::Value& payload = msg->messagePayload();
    Json::Value jrates(Json::objectValue);
    for (auto& entry : mOutputRates) {
        jrates[entry.first] = rateToJson(entry.second);
    }
    payload["outputs"] = jrates;
    payload["defaultRate"] = rateToJson(mDefaultRate);
    payload["sendAllWhenComplete"] = mSendAllWhenComplete;
    return msg;
}

void 
OutputRates::getRate(const std::string& output,
                     /* out*/ unsigned& interval,
                     /* out*/ unsigned& offset) 
{
    auto it = mOutputRates.find(output);
    if (it == mOutputRates.end()) {
        interval = mDefaultRate.interval;
        offset = mDefaultRate.offset;
    } else {
        interval = it->second.interval;
        offset = it->second.offset;
    }
}

void
OutputRates::infoDump()
{
    ARRAS_LOG_INFO("Render Output Rates:");
    for (auto& entry : mOutputRates) {
        ARRAS_LOG_INFO("   Output: %s   Interval: %d   Offset %d",entry.first.c_str(),entry.second.interval,entry.second.offset);
    }
    ARRAS_LOG_INFO("   Default:    Interval: %d   Offset %d",mDefaultRate.interval,mDefaultRate.offset);
    if (mSendAllWhenComplete) {
        ARRAS_LOG_INFO("All outputs are sent when render is complete");
    } else {
        ARRAS_LOG_INFO("Usual rates apply when render is complete");
    }
}

}

