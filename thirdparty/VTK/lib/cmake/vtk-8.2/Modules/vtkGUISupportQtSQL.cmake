set(vtkGUISupportQtSQL_LOADED 1)
set(vtkGUISupportQtSQL_DEPENDS "vtkCommonCore;vtkIOSQL;vtksys")
set(vtkGUISupportQtSQL_LIBRARIES "vtkGUISupportQtSQL")
set(vtkGUISupportQtSQL_INCLUDE_DIRS "${VTK_INSTALL_PREFIX}/include/vtk-8.2")
set(vtkGUISupportQtSQL_LIBRARY_DIRS "")
set(vtkGUISupportQtSQL_RUNTIME_LIBRARY_DIRS "${VTK_INSTALL_PREFIX}/bin")
set(vtkGUISupportQtSQL_WRAP_HIERARCHY_FILE "${CMAKE_CURRENT_LIST_DIR}/vtkGUISupportQtSQLHierarchy.txt")
set(vtkGUISupportQtSQL_KIT "")
set(vtkGUISupportQtSQL_TARGETS_FILE "")
set(vtkGUISupportQtSQL_EXCLUDE_FROM_WRAPPING 1)

if(NOT Qt5_DIR)
  set(Qt5_DIR "D:/ProgramFiles/Qt/5.15.0/msvc2019_64/lib/cmake/Qt5")
endif()
find_package(Qt5 REQUIRED QUIET COMPONENTS Sql Widgets)


