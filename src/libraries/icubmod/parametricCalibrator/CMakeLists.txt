# Copyright: (C) 2010 RobotCub Consortium
# Authors: Alberto Cardellino
# CopyPolicy: Released under the terms of the GNU GPL v2.0.


yarp_prepare_device(parametricCalibrator TYPE yarp::dev::parametricCalibrator INCLUDE parametricCalibrator.h)

if(ENABLE_parametricCalibrator)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                       ${YARP_INCLUDE_DIRS}
                       ${iCubDev_INCLUDE_DIRS})

    yarp_add_plugin(parametricCalibrator parametricCalibrator.h parametricCalibrator.cpp)
    target_link_libraries(parametricCalibrator ${YARP_LIBRARIES})
    icub_export_plugin(parametricCalibrator)
    yarp_install(FILES parametricCalibrator.ini  DESTINATION ${ICUB_PLUGIN_MANIFESTS_INSTALL_DIR})
endif()
