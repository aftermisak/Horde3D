

set(command "D:/cmake-3.9.1-win64-x64/bin/cmake.exe;-DCMAKE_INSTALL_PREFIX:PATH=F:/from_github/Horde3D/build-vs/project_glfw-prefix;-DGLFW_BUILD_DOCS:BOOL=OFF;-DGLFW_BUILD_EXAMPLES:BOOL=OFF;-DGLFW_BUILD_TESTS:BOOL=OFF;-GVisual Studio 14 2015;F:/from_github/Horde3D/build-vs/project_glfw-prefix/src/project_glfw")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "F:/from_github/Horde3D/build-vs/project_glfw-prefix/src/project_glfw-stamp/project_glfw-configure-out.log"
  ERROR_FILE "F:/from_github/Horde3D/build-vs/project_glfw-prefix/src/project_glfw-stamp/project_glfw-configure-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  F:/from_github/Horde3D/build-vs/project_glfw-prefix/src/project_glfw-stamp/project_glfw-configure-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "project_glfw configure command succeeded.  See also F:/from_github/Horde3D/build-vs/project_glfw-prefix/src/project_glfw-stamp/project_glfw-configure-*.log")
  message(STATUS "${msg}")
endif()
