

set(command "D:/cmake-3.9.1-win64-x64/bin/cmake.exe;--build;.;--config;MinSizeRel")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "F:/from_github/Horde3D/build-vs/project_glfw-prefix/src/project_glfw-stamp/project_glfw-build-out.log"
  ERROR_FILE "F:/from_github/Horde3D/build-vs/project_glfw-prefix/src/project_glfw-stamp/project_glfw-build-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  F:/from_github/Horde3D/build-vs/project_glfw-prefix/src/project_glfw-stamp/project_glfw-build-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "project_glfw build command succeeded.  See also F:/from_github/Horde3D/build-vs/project_glfw-prefix/src/project_glfw-stamp/project_glfw-build-*.log")
  message(STATUS "${msg}")
endif()
