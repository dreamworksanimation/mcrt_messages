// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0


// OutputRates is used by the mcrt computation to control how often
// particular render outputs are included in the render messages send
// back to the client. It is passed from the client to
// the computation via a JSON message SetOutputRates.
// 
#pragma once

#include "JSONMessage.h"

#include <string>

namespace mcrt {

class OutputRates 
{
public:
    struct IntervalAndOffset {
        IntervalAndOffset() : interval(1), offset(0) {}
        IntervalAndOffset(unsigned i,unsigned o) : interval(i), offset(o) {}
        unsigned interval;
        unsigned offset;
    };

    OutputRates() : mSendAllWhenComplete(true) {}

    static const char* SET_OUTPUT_RATES_ID;
    static const char* SET_OUTPUT_RATES_NAME;

    // throws InvalidParameterException if message is invalid
    void setFromMessage(JSONMessage::ConstPtr jm);

    JSONMessage::Ptr getAsMessage() const;

    void setRate(const std::string& output,unsigned interval,unsigned offset = 0) {
        mOutputRates[output] = IntervalAndOffset(interval,offset);
    }
    void setDefaultRate(unsigned interval,unsigned offset = 0) {
        mDefaultRate = IntervalAndOffset(interval,offset);
    }
    void setSendAllWhenComplete(bool aFlag) { mSendAllWhenComplete = aFlag; }

    void getRate(const std::string& output,
                 /* out*/ unsigned& interval,
                 /* out*/ unsigned& offset);
    bool getSendAllWhenComplete() const { return mSendAllWhenComplete; }

    void infoDump();

private:

    // Counters used to send render outputs less frequently than every frame.
    // Each entry maps an output name to frame interval and offset
    // 0 means 'never send', 1 means 'every frame', 2 'every other frame' & so on
    // mDefaultInterval is used for unlisted frames
    typedef std::map<std::string,IntervalAndOffset> OutputRateMap;
    OutputRateMap mOutputRates;
    IntervalAndOffset mDefaultRate;
    // When true, ignore rates when render is complete
    bool mSendAllWhenComplete = true;
};

}


