#include "FramesQueue.h"

ProtectedFramesQueue::ProtectedFramesQueue(){
	Logging::getFile()->info("protected frames queue constructor");
}

void ProtectedFramesQueue::push(Mat mat)
{
	Logging::getFile()->info("I'm in frame queue");
	unique_lock<std::mutex> lock(mtx);
	if (framesQueue.size() == QUEUE_SIZE)
		framesQueue.pop();
	Logging::getFile()->info("I push to queue");
	framesQueue.push(mat);
	condition.notify_all();
}
cv::Mat ProtectedFramesQueue::pop()
{
	unique_lock<mutex> lock(mtx);
	condition.wait(lock, [this]() { return !framesQueue.empty(); });
	Mat item = framesQueue.front();
	framesQueue.pop();
	return item;
}
cv::Mat ProtectedFramesQueue::copyTo(Mat other)
{
	lock_guard<mutex> lock(mtx);
	framesQueue.front().copyTo(other);
	framesQueue.pop();
	//condition.notify_all();//???????????????
	return other;
}
cv::Mat ProtectedFramesQueue::front()
{
	unique_lock<mutex> lock(mtx);
	condition.wait(lock, [this]() { return !framesQueue.empty(); });
	Mat item = framesQueue.front();
	condition.notify_all();
	return item;
}
bool ProtectedFramesQueue::empty()
{
	lock_guard<mutex> lock(mtx);
	return framesQueue.empty();
}
size_t ProtectedFramesQueue::size()
{
	lock_guard<mutex> lock(mtx);
	return framesQueue.size();
}
ProtectedFramesQueue& ProtectedFramesQueue::getFramesQueue()
{
	if (protectedFramesQueue == NULL)
		protectedFramesQueue = new ProtectedFramesQueue();
	return *protectedFramesQueue;
}