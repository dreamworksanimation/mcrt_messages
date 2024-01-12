# Copyright 2023-2024 DreamWorks Animation LLC
# SPDX-License-Identifier: Apache-2.0

set(component mcrt_messages)

set(InstallIncludeDir ${PACKAGE_NAME})
set(exportGroup ${PROJECT_NAME}Targets)

add_library(${component} SHARED "")
add_library(${PROJECT_NAME}::${component} ALIAS ${component})

target_sources(${component}
    PRIVATE
        BaseFrame.cc
        CreditUpdate.cc
        JSONMessage.cc
        JSONUtils.cc
        GenericMessage.cc
        GeometryData.cc
        OutputRates.cc
        PartialFrame.cc
	ProgressiveFeedback.cc
        ProgressiveFrame.cc
        ProgressMessage.cc
        RDLMessage.cc
        RDLMessage_LeftEye.cc
        RDLMessage_RightEye.cc
        RenderedFrame.cc
        RenderMessages.cc
        ViewportMessage.cc
)

set_property(TARGET ${component}
    PROPERTY PUBLIC_HEADER
        BaseFrame.h
        CreditUpdate.h
        JSONMessage.h
        JSONUtils.h
        GenericMessage.h
        GeometryObjects.h
        GeometryData.h
        OutputRates.h
        PartialFrame.h
	ProgressiveFeedback.h
        ProgressiveFrame.h
        ProgressMessage.h
        RDLMessage.h
        RDLMessage_LeftEye.h
        RDLMessage_RightEye.h
        RenderedFrame.h
        RenderMessages.h
        ViewportMessage.h
)

target_include_directories(${component}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${component}
    PUBLIC
        ArrasCore::message_api
        ArrasCore::arras4_log
)

# If at Dreamworks add a SConscript stub file so others can use this library.
SConscript_Stub(${component})

# Set standard compile/link options
McrtMessages_cxx_compile_definitions(${component})
McrtMessages_cxx_compile_features(${component})
McrtMessages_cxx_compile_options(${component})
McrtMessages_link_options(${component})

# -------------------------------------
# Install the target and the export set
# -------------------------------------
include(GNUInstallDirs)

# install the target
install(TARGETS ${component}
    COMPONENT ${component}
    EXPORT ${exportGroup}
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        NAMELINK_SKIP
    RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${InstallIncludeDir}
)

# # install the export set
# install(
#     EXPORT ${exportGroup}
#     NAMESPACE ${PROJECT_NAME}::
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
# )
