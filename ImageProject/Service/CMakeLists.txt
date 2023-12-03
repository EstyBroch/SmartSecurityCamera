set(PROJECT_NAME Service)

find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Threads)

# -------------------------------------------------------
# Protobuf/Grpc source files
# -------------------------------------------------------

set(PROTO_FILES
my_proto/camera.proto
)

# -------------------------------------------------------
# Add Library target with protobuf sources
# -------------------------------------------------------

add_library(my_proto ${PROTO_FILES})
target_link_libraries(my_proto
    PUBLIC
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
)
target_include_directories(my_proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# -------------------------------------------------------
# Compile protobuf and grpc files in myproto target to cpp
# -------------------------------------------------------
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET my_proto LANGUAGE cpp)
protobuf_generate(TARGET my_proto LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")