include("/home/vboxuser/WorkSpace/Machine_Vision_Course/Qt_Projects/1st_Projects/show_image/build/Desktop_Qt_6_9_3-Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/show_image-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "/home/vboxuser/WorkSpace/Machine_Vision_Course/Qt_Projects/1st_Projects/show_image/build/Desktop_Qt_6_9_3-Debug/show_image"
    GENERATE_QT_CONF
)
