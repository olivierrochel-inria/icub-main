# Copyright: (C) 2012 RobotCub Consortium
# Authors: Alberto Cardellino
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

yarp_prepare_device(embObjInertials TYPE yarp::dev::embObjInertials
                                       INCLUDE embObjInertials.h
                                       WRAPPER AnalogServer)

IF (NOT SKIP_embObjInertials)

    if(ETHRES_DEBUG_DONTREADBACK)
        add_definitions( -DETHRES_DEBUG_DONTREADBACK)
    endif(ETHRES_DEBUG_DONTREADBACK)

  set(ICUB_COMPILE_EMBOBJ_LIBRARY ON CACHE INTERNAL "use the embObjLib lib")
  INCLUDE(../embObjLib/embObjLib.cmake)

#   message(INFO " embObjInertials - embObj_includes: ${embObj_includes}, ${CMAKE_CURRENT_SOURCE_DIR}/")
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${YARP_INCLUDE_DIRS} ${iCubDev_INCLUDE_DIRS} )

  yarp_add_plugin(embObjInertials embObjInertials.cpp embObjInertials.h)
  TARGET_LINK_LIBRARIES(embObjInertials ethResources ${YARP_LIBRARIES} ${ACE_LIBRARIES})
  icub_export_plugin(embObjInertials)

  yarp_install(FILES embObjInertials.ini  DESTINATION ${ICUB_PLUGIN_MANIFESTS_INSTALL_DIR})

ENDIF (NOT SKIP_embObjInertials)

