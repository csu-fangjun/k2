# Copyright     2020 Fangjun Kuang (csukuangfj@gmail.com)
# See ../LICENSE for clarification regarding multiple authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

function(download_pybind11)
  if(CMAKE_VERSION VERSION_LESS 3.11)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
  endif()

  include(FetchContent)

  # latest commit as of 2022.10.31 that supports python 3.11
  set(pybind11_URL  "https://github.com/pybind/pybind11/archive/refs/tags/v2.12.0.tar.gz")
  set(pybind11_URL2 "https://hf-mirror.com/csukuangfj/sherpa-onnx-cmake-deps/resolve/main/pybind11-2.12.0.tar.gz")
  set(pybind11_HASH "SHA256=bf8f242abd1abcd375d516a7067490fb71abd79519a282d22b6e4d19282185a7")

  # If you don't have access to the Internet,
  # please pre-download pybind11
  set(possible_file_locations
    $ENV{HOME}/Downloads/pybind11-2.12.0.tar.gz
    ${CMAKE_SOURCE_DIR}/pybind11-2.12.0.tar.gz
    ${CMAKE_BINARY_DIR}/pybind11-2.12.0.tar.gz
    /tmp/pybind11-2.12.0.tar.gz
    /star-fj/fangjun/download/github/pybind11-2.12.0.tar.gz
  )

  foreach(f IN LISTS possible_file_locations)
    if(EXISTS ${f})
      set(pybind11_URL  "file://${f}")
      set(pybind11_URL2)
      break()
    endif()
  endforeach()

  set(double_quotes "\"")
  set(dollar "\$")
  set(semicolon "\;")
  if(NOT WIN32)
    FetchContent_Declare(pybind11
      URL
        ${pybind11_URL}
        ${pybind11_URL2}
      URL_HASH          ${pybind11_HASH}
      PATCH_COMMAND
        sed -i.bak s/\\${double_quotes}-flto\\\\${dollar}/\\${double_quotes}-Xcompiler=-flto${dollar}/g "tools/pybind11Tools.cmake" &&
        sed -i.bak s/${seimcolon}-fno-fat-lto-objects/${seimcolon}-Xcompiler=-fno-fat-lto-objects/g "tools/pybind11Tools.cmake"
    )
  else()
    FetchContent_Declare(pybind11
      URL               ${pybind11_URL}
      URL_HASH          ${pybind11_HASH}
    )
  endif()

  FetchContent_GetProperties(pybind11)
  if(NOT pybind11_POPULATED)
    message(STATUS "Downloading pybind11 from ${pybind11_URL}")
    FetchContent_Populate(pybind11)
  endif()
  message(STATUS "pybind11 is downloaded to ${pybind11_SOURCE_DIR}")
  add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR} EXCLUDE_FROM_ALL)
endfunction()

download_pybind11()
