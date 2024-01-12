// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0


#pragma once

#include "JSONMessage.h"
#include <vector>

namespace mcrt {

class RenderMessages {
public:

    enum RenderStatus {
        STARTED = 0,
        RENDERING,
        FINISHED,
        CANCELLED,
        ERROR
    };

    static const char* RENDER_CONTROL_ID;
    static const char* RENDER_CONTROL_NAME;
    static const char* RENDER_CONTROL_PAYLOAD_OPERATION;
    static const char* RENDER_CONTROL_PAYLOAD_OPERATION_START;
    static const char* RENDER_CONTROL_PAYLOAD_OPERATION_STOP;

    static const char* PICK_MESSAGE_ID;
    static const char* PICK_MESSAGE_NAME;
    static const char* PICK_MESSAGE_PAYLOAD_PIXEL;
    static const char* PICK_MESSAGE_PAYLOAD_MODE;

    static const char* PICK_DATA_MESSAGE_ID;
    static const char* PICK_DATA_MESSAGE_NAME;
    static const char* PICK_DATA_MESSAGE_PAYLOAD_LIGHTS;
    static const char* PICK_DATA_MESSAGE_PAYLOAD_LIGHT_CONTRIBUTIONS;
    static const char* PICK_DATA_MESSAGE_PAYLOAD_GEOMETRY;
    static const char* PICK_DATA_MESSAGE_PAYLOAD_GEOMETRY_PARTS;
    static const char* PICK_DATA_MESSAGE_PAYLOAD_MATERIALS;
    enum PickMode {
        QUERY_NONE = -1,
        QUERY_MATERIAL,
        QUERY_LIGHT_CONTRIBUTIONS,
        QUERY_GEOMETRY,
        QUERY_GEOMETRY_PART,
        QUERY_POSITION_AND_NORMAL,
        QUERY_CELL_INSPECTOR,
    };

    static const char* LOGGING_CONFIGURATION_MESSAGE_ID;
    static const char* LOGGING_CONFIGURATION_MESSAGE_NAME;

    static const char* LOGGING_MESSAGE_ID;
    static const char* LOGGING_MESSAGE_NAME;
    static const char* LOGGING_MESSAGE_PAYLOAD_STRING;
    static const char* LOGGING_MESSAGE_PAYLOAD_LEVEL;
    
    static const char* SET_ROI_OPERATION_ID;
    static const char* SET_ROI_OPERATION_NAME;

    static const char* SET_ROI_STATUS_OPERATION_NAME;
    static const char* SET_ROI_STATUS_OPERATION_ID;

    static const char* INVALIDATE_RESOURCES_ID ;
    static const char* INVALIDATE_RESOURCES_MESSAGE_NAME;
    static const char* INVALIDATE_RESOURCES_PAYLOAD_LIST;

    static const char* RENDER_SETUP_ID;
    static const char* RENDER_SETUP_NAME;
   
    static const char* RENDER_FILE_MESSAGE_ID;
    static const char* RENDER_FILE_MESSAGE_NAME;
    static const char* RENDER_FILE_PAYLOAD_FORCE_RELOAD;
    static const char* RENDER_FILE_PAYLOAD_FILE;
    static const char* RENDER_FILE_PAYLOAD_FILES;

    static JSONMessage::Ptr createPickDataMessage(int x, int y, const std::string& clientData);
    static JSONMessage::Ptr createLogMessage(const int level, const std::string& message);
    static JSONMessage::Ptr createControlMessage(bool stop);
    static JSONMessage::Ptr createRenderSetupMessage();
    static JSONMessage::Ptr createRoiMessage(int minx, int miny, int maxx, int maxy);
    static JSONMessage::Ptr createRoiStatusMessage(bool status);
    static JSONMessage::Ptr createInvalidateResourcesMessage (const std::vector<std::string>& resources);
    static JSONMessage::Ptr createRenderFileMessage(bool forceReload, const std::string& file);
    static JSONMessage::Ptr createRenderFileMessage(bool forceReload,
                                                    const std::vector<std::string>& files);
};

}
