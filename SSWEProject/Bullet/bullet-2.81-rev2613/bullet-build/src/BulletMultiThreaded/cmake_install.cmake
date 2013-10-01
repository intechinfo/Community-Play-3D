# Install script for directory: /Users/julienmoreau-mathis/Desktop/Soganatsu Studios/Projects/SSWE/SSWEProject/Bullet/bullet-2.81-rev2613/src/BulletMultiThreaded

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/Library/Frameworks")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE DIRECTORY FILES "/Users/julienmoreau-mathis/Desktop/Soganatsu Studios/Projects/SSWE/SSWEProject/Bullet/bullet-2.81-rev2613/bullet-build/src/BulletMultiThreaded/BulletMultiThreaded.framework" USE_SOURCE_PERMISSIONS)
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./BulletMultiThreaded.framework/Versions/2.81/BulletMultiThreaded" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./BulletMultiThreaded.framework/Versions/2.81/BulletMultiThreaded")
    EXECUTE_PROCESS(COMMAND "/usr/bin/install_name_tool"
      -id "/Library/Frameworks/BulletMultiThreaded.framework/Versions/2.81/BulletMultiThreaded"
      -change "/Users/julienmoreau-mathis/Desktop/Soganatsu Studios/Projects/SSWE/SSWEProject/Bullet/bullet-2.81-rev2613/bullet-build/src/BulletCollision/BulletCollision.framework/Versions/2.81/BulletCollision" "/Library/Frameworks/BulletCollision.framework/Versions/2.81/BulletCollision"
      -change "/Users/julienmoreau-mathis/Desktop/Soganatsu Studios/Projects/SSWE/SSWEProject/Bullet/bullet-2.81-rev2613/bullet-build/src/BulletDynamics/BulletDynamics.framework/Versions/2.81/BulletDynamics" "/Library/Frameworks/BulletDynamics.framework/Versions/2.81/BulletDynamics"
      -change "/Users/julienmoreau-mathis/Desktop/Soganatsu Studios/Projects/SSWE/SSWEProject/Bullet/bullet-2.81-rev2613/bullet-build/src/LinearMath/LinearMath.framework/Versions/2.81/LinearMath" "/Library/Frameworks/LinearMath.framework/Versions/2.81/LinearMath"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./BulletMultiThreaded.framework/Versions/2.81/BulletMultiThreaded")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./BulletMultiThreaded.framework/Versions/2.81/BulletMultiThreaded")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/Users/julienmoreau-mathis/Desktop/Soganatsu Studios/Projects/SSWE/SSWEProject/Bullet/bullet-2.81-rev2613/bullet-build/src/BulletMultiThreaded/GpuSoftBodySolvers/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

