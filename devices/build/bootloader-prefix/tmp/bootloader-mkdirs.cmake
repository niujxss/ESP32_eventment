# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "F:/Project/ESP32/IDF/Espressif/frameworks/esp-idf-v5.1.2/components/bootloader/subproject"
  "F:/Project/work/myproject/getenvironmentstatus/devices/build/bootloader"
  "F:/Project/work/myproject/getenvironmentstatus/devices/build/bootloader-prefix"
  "F:/Project/work/myproject/getenvironmentstatus/devices/build/bootloader-prefix/tmp"
  "F:/Project/work/myproject/getenvironmentstatus/devices/build/bootloader-prefix/src/bootloader-stamp"
  "F:/Project/work/myproject/getenvironmentstatus/devices/build/bootloader-prefix/src"
  "F:/Project/work/myproject/getenvironmentstatus/devices/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "F:/Project/work/myproject/getenvironmentstatus/devices/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "F:/Project/work/myproject/getenvironmentstatus/devices/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
