

####################################################################################
# Auto generated @PACKAGE_INIT@ by rocm_configure_package_config_file() 
# from rocfft-config.cmake.in
# Any changes to this file will be overwritten by the next CMake run
####################################################################################

get_filename_component(_ROCM_CMAKE_CURRENT_LIST_FILE_REAL "${CMAKE_CURRENT_LIST_FILE}" REALPATH)
get_filename_component(_ROCM_CMAKE_CURRENT_LIST_DIR_REAL "${_ROCM_CMAKE_CURRENT_LIST_FILE_REAL}" DIRECTORY)
get_filename_component(PACKAGE_PREFIX_DIR "/opt/rocm/" ABSOLUTE)



macro(set_and_check _var _file)
    set(${_var} "${_file}")
    if(NOT EXISTS "${_file}")
        message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
    endif()
endmacro()

include(CMakeFindDependencyMacro OPTIONAL RESULT_VARIABLE _ROCMCMakeFindDependencyMacro_FOUND)
if (NOT _ROCMCMakeFindDependencyMacro_FOUND)
    macro(find_dependency dep)
        if (NOT ${dep}_FOUND)
            set(rocm_fd_version)
            if (${ARGC} GREATER 1)
                set(rocm_fd_version ${ARGV1})
            endif()
            set(rocm_fd_exact_arg)
            if(${CMAKE_FIND_PACKAGE_NAME}_FIND_VERSION_EXACT)
                set(rocm_fd_exact_arg EXACT)
            endif()
            set(rocm_fd_quiet_arg)
            if(${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
                set(rocm_fd_quiet_arg QUIET)
            endif()
            set(rocm_fd_required_arg)
            if(${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED)
                set(rocm_fd_required_arg REQUIRED)
            endif()
            find_package(${dep} ${rocm_fd_version}
                ${rocm_fd_exact_arg}
                ${rocm_fd_quiet_arg}
                ${rocm_fd_required_arg}
            )
            string(TOUPPER ${dep} cmake_dep_upper)
            if (NOT ${dep}_FOUND AND NOT ${cmake_dep_upper}_FOUND)
                set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE "${CMAKE_FIND_PACKAGE_NAME} could not be found because dependency ${dep} could not be found.")
                set(${CMAKE_FIND_PACKAGE_NAME}_FOUND False)
                return()
            endif()
            set(rocm_fd_version)
            set(rocm_fd_required_arg)
            set(rocm_fd_quiet_arg)
            set(rocm_fd_exact_arg)
        endif()
    endmacro()
endif()

macro(check_required_components _NAME)
    foreach(comp ${${_NAME}_FIND_COMPONENTS})
        if(NOT ${_NAME}_${comp}_FOUND)
            if(${_NAME}_FIND_REQUIRED_${comp})
                set(${_NAME}_FOUND FALSE)
            endif()
        endif()
    endforeach()
endmacro()

####################################################################################

    
    
set_and_check(rocfft_INCLUDE_DIR ${PACKAGE_PREFIX_DIR}/rocfft/include)

set_and_check(rocfft_INCLUDE_DIRS ${PACKAGE_PREFIX_DIR}/rocfft/include)

set_and_check(ROCFFT_INCLUDE_DIR ${PACKAGE_PREFIX_DIR}/rocfft/include)

set_and_check(ROCFFT_INCLUDE_DIRS ${PACKAGE_PREFIX_DIR}/rocfft/include)

set_and_check(rocfft_INCLUDE_DIR ${PACKAGE_PREFIX_DIR}/rocfft/include)

set_and_check(rocfft_INCLUDE_DIRS ${PACKAGE_PREFIX_DIR}/rocfft/include)

set_and_check(rocfft_TARGET_FILE ${PACKAGE_PREFIX_DIR}/rocfft/lib/cmake/rocfft/rocfft-targets.cmake)

#set_and_check(rocfft_TARGET_FILE /work/home/jlabtest/zyz/gromacs-2018.4/cmake/rocfft-targets.cmake)
find_dependency(hip)

include(${rocfft_TARGET_FILE})
message(status ${rocfft_TARGET_FILE})
link_directories(/opt/rocm/rocfft/lib)
set (rocfft-targets rocfft rocfft-device)
set(rocfft_LIBRARIES ${rocfft-targets})

set(rocfft_LIBRARY ${rocfft-targets})

set(ROCFFT_LIBRARIES ${rocfft-targets})

set(ROCFFT_LIBRARY ${rocfft-targets})

set(rocfft_LIBRARIES ${rocfft-targets})

set(rocfft_LIBRARY ${rocfft-targets})
