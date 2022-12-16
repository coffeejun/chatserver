cmake_minimum_required(VERSION 3.0)#要求的最小版本
project(chat)#给工程一个名字
#配置编译选项
#第一个是字段 后面是在原来的基础上加上-g选项，代表可以调试
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

#设置可执行文件最终存储的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#配置头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/server)
include_directories(${PROJECT_SOURCE_DIR}/include/server/db)
include_directories(${PROJECT_SOURCE_DIR}/include/server/model)
include_directories(${PROJECT_SOURCE_DIR}/include/server/redis)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)

#加载子目录
add_subdirectory(src)