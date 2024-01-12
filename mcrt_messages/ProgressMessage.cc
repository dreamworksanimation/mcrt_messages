// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "ProgressMessage.h"
#include <message_api/MessageFormatError.h>

namespace mcrt {

ARRAS_CONTENT_IMPL(ProgressMessage);

void ProgressMessage::serialize(arras4::api::DataOutStream& to) const
{
    to << arras4::api::objectToString(mObject);
}

void ProgressMessage::deserialize(arras4::api::DataInStream& from, 
                                       unsigned /*version*/)
{
    std::string s;
    from >> s;
    try {
	arras4::api::stringToObject(s,mObject);
    } catch (std::exception& e) {
	throw arras4::api::MessageFormatError(e.what());
    }
}

}
 
