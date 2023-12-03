#include <iostream>
#include <mutex>
#include <thread> // Include for std::thread
#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>
#include "../Service/my_proto/camera.grpc.pb.h"
#include "../Service/my_proto/camera.pb.h"
#include <opencv2/opencv.hpp> // Include for OpenCV
#include "GetFrame.h"

class GetFrameService final : public expcmake::Greeter::Service 
{
public:
    virtual ::grpc::Status getMat(::grpc::ServerContext* context, const ::expcmake::Frame* request, ::expcmake::CreateMat* response)
    {
        if (request == nullptr)
        {
           response->set_message("failed! Request is NULL");
           return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Request is NULL");
        }
        else
        {
           // Decode the image data
           std::vector<uchar> imageData(request->imagedata().begin(), request->imagedata().end());
           cv::Mat image = cv::imdecode(imageData, cv::IMREAD_COLOR);
           // Ensure that the image channels match the expected channels on the server
           if (image.channels() != request->channels())
               cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
           // Resize the image if necessary
           if (image.cols != request->width() || image.rows != request->height())
               cv::resize(image, image, cv::Size(request->width(), request->height()));
           //func to push frame
           GetFrame gFrame;
           gFrame.addFrameToQueue(image);
           response->set_message("ok! its succeeded");
           return grpc::Status::OK;
        }
       
    }
   
};

void openGRPC()
{
    grpc::ServerBuilder builder;
    // Make a router
    builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());
    GetFrameService my_service;
    builder.RegisterService(&my_service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server is running. Press Esc to stop." << std::endl;
export DOCKER_CLI_EXPERIMENTAL=enabled}

int main(int argc, char* argv[])
{
      GetFrame gFrame;
      // Make threads
      std::thread serverThreadPushFrame(openGRPC);
      thread serverThreadDetectFrame(&GetFrame::detection, &gFrame);
      // Release the threads
      serverThreadPushFrame.join();
      serverThreadDetectFrame.join();
      return 0;
}
