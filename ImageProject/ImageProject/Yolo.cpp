#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include "Yolo.h"
#include "../../ImageProject/sqlite_files/sqlite3.h"
#ifdef _WIN32
#define LIST_OBJECT Configuration::getConfiguration().getConfigValue(eConfig::windows_objectsListPath)
#else
#define LIST_OBJECT Configuration::getConfiguration().getConfigValue(eConfig::linux_objectsListPath)
#endif

void Yolo::CalcAvg(Mat img, double& avgR, double& avgG, double& avgB)
{
	uint32_t sumR = 0, sumG = 0, sumB = 0;
	//Go over the rectangle pixel by pixel
	for (int row = 0; row < img.rows; row++) {
		for (int col = 0; col < img.cols; col++) {
			uchar* row_data = img.ptr(row) + col * 3;
			//Sum up each color channel separately in order BGR
			sumB += row_data[0];
			sumG += row_data[1];
			sumR += row_data[2];
		}
	}
	//Calculate the average
	int totalPixels = img.rows * img.cols;
	avgR = static_cast<double>(sumR) / totalPixels;
	avgG = static_cast<double>(sumG) / totalPixels;
	avgB = static_cast<double>(sumB) / totalPixels;
}


void Yolo::CalcSaveAvg(Mat img, int x, int y, int width, int height)
{
	Logging::getFile()->info("CalcSaveAvg");
	double avgR, avgG, avgB;
	Mat view;
	if (x > 0 && y > 0 && width > 0 && height > 0)
	{
		//check the borders of rect in frame
		if (y + height > img.rows && x + width > img.cols)
		{
			double difSizeRow = (y + height) - img.rows;
			double difSizecol = (x + width) - img.cols;
			Rect ROI(x, y, width - difSizecol, height - difSizeRow);
			view = img(ROI);
		}
		else
			if (x + width > img.cols)
			{
				double difSize = (x + width) - img.cols;
				Rect ROI(x, y, width - difSize, height);
				view = img(ROI);
			}
			else
				if (y + height > img.rows)
				{
					double difSize = (y + height) - img.rows;
					Rect ROI(x, y, width, height - difSize);
					view = img(ROI);
				}
				else
				{
					Rect ROI(x, y, width, height);
					view = img(ROI);
				}
       
	     CalcAvg(view, avgR, avgG, avgB);
		//save at csv file
		//CSV_Save data= CSV_Save();
		//save at sqlite
		Sqlite data = Sqlite();
		data.save(avgR, avgG, avgB, x, y, width, height);
		Logging::getFile()->info("finish calc the frames");
	}
}

void Yolo::draw_label(Mat& input_image, string label, int left, int top)
{
	// Display the label at the top of the bounding box.
	int baseLine;
	Size label_size = getTextSize(label, FONT_FACE, FONT_SCALE, THICKNESS, &baseLine);
	top = max(top, label_size.height);
	Point tlc = Point(left, top);
	Point brc = Point(left + label_size.width, top + label_size.height + baseLine);
	// Draw white rectangle.
	cv::rectangle(input_image, tlc, brc, BLACK, FILLED);
	// Put the label on the black rectangle.
	putText(input_image, label, cv::Point(left, top + label_size.height), FONT_FACE, FONT_SCALE, YELLOW, THICKNESS);
}

vector<Mat> Yolo::pre_process(Mat& input_image, Net& net)
{
	// Convert to blob(Binary Large Object).
	cv::Mat blob;
	blobFromImage(input_image, blob, 1. / 255., Size(INPUT_WIDTH, INPUT_HEIGHT), Scalar(), true, false);

	net.setInput(blob);

	// Forward propagate.
	vector<Mat> outputs;
	net.forward(outputs, net.getUnconnectedOutLayersNames());

	return outputs;
}

Mat Yolo::post_process(Mat& input_image, vector<Mat>& outputs, const vector<string>& class_name)
{
	Logging::getFile()->info("enter to post_process function");
	//cout << "-------------enter to post_process function--------------" << endl;
	// Initialize vectors to hold respective outputs while unwrapping     detections.
	vector<int> class_ids;
	vector<float> confidences;
	vector<Rect> boxes;
	// Resizing factor.
	float x_factor = input_image.cols / INPUT_WIDTH;
	float y_factor = input_image.rows / INPUT_HEIGHT;
	float* data = (float*)outputs[0].data;
	const int dimensions = 85;
	const int rows = 25200;

	for (int i = 0; i < rows; ++i)
	{
		//cout << "first for in post_process " << endl;
		float confidence = data[4];
		// Discard bad detections and continue.
		if (confidence >= CONFIDENCE_THRESHOLD)
		{
			float* classes_scores = data + 5;
			// Create a 1x85 Mat and store class scores of 80 classes.
			Mat scores(1, class_name.size(), CV_32FC1, classes_scores);
			// Perform minMaxLoc and acquire the index of best class  score.
			Point class_id;
			double max_class_score;
			minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
			// Continue if the class score is above the threshold.
			if (max_class_score > SCORE_THRESHOLD)
			{
				// Store class ID and confidence in the pre-defined respective vectors.
				confidences.push_back(confidence);
				class_ids.push_back(class_id.x);
				float cx = data[0];
				float cy = data[1];
				// Box dimension.
				float w = data[2];
				float h = data[3];
				// Bounding box coordinates.
				int left = int((cx - 0.5 * w) * x_factor);
				int top = int((cy - 0.5 * h) * y_factor);
				int width = int(w * x_factor);
				int height = int(h * y_factor);
				// Store good detections in the boxes vector.
				boxes.push_back(Rect(left, top, width, height));
			}
		}
		// Jump to the next row.
		data += 85;
	}
	vector<int> indices;
	cout << ".........." << indices.size() << "..........." << endl;
	//  After processing all detections, it performs non-maximum suppression (NMS)
	NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, indices);
	for (int i = 0; i < indices.size(); i++)
	{
		Logging::getFile()->info("ForLopNMSBoxes");
		int idx = indices[i];
		Rect box = boxes[idx];
		int left = box.x;
		int top = box.y;
		int width = box.width;
		int height = box.height;
		cout << "entercala" << endl;
		CalcSaveAvg(input_image, left, top, width, height);
		
		rectangle(input_image, Point(left, top), Point(left + width, top + height), BLUE, 3 * THICKNESS);
		// Get the label for the class name and its confidence.
		string label = format("%.2f", confidences[idx]);
		label = class_name[class_ids[idx]] + ":" + label;
		draw_label(input_image, label, left, top);
	}
	return input_image;
}

vector<string> Yolo::createClass_List()
{
   vector<string> class_list;
   ifstream ifs((string)LIST_OBJECT);
   string line;
   while (getline(ifs, line))
   {
   	class_list.push_back(line);
   }
   return class_list;
}

Mat Yolo::createYoloImage(Yolo& image, Mat frame, Net net)
{
	Logging::getFile()->info("createYoloImage");
   vector<string> class_list = createClass_List();
   vector<Mat> detections;
   vector<double> layersTimes;
   // Process the image.
   detections = image.pre_process(frame, net);
   Mat img = image.post_process(frame, detections, class_list);
   // Put efficiency information.
   double freq = getTickFrequency() / 1000;
   double t = net.getPerfProfile(layersTimes) / freq;
   string label = format("Inference time : %.2f ms", t);
   putText(img, label, Point(20, 40), image.getFONT_FACE(), image.getFONT_SCALE(), image.getRED());
   Mat smallImageC(1080, 1920, CV_8UC3);
   resize(img, smallImageC, smallImageC.size());
   return smallImageC;
	
}

float Yolo::getINPUT_WIDTH() const
{
	return INPUT_WIDTH;
}

float Yolo::getINPUT_HEIGHT() const
{
	return INPUT_HEIGHT;
}

float Yolo::getSCORE_THRESHOLD() const
{
	return SCORE_THRESHOLD;
}

float Yolo::getNMS_THRESHOLD() const
{
	return NMS_THRESHOLD;
}

float Yolo::getCONFIDENCE_THRESHOLD() const
{
	return CONFIDENCE_THRESHOLD;
}

float Yolo::getFONT_SCALE() const
{
	return FONT_SCALE;
}

int Yolo::getFONT_FACE() const
{
	return FONT_FACE;
}

int Yolo::getTHICKNESS() const
{
	return THICKNESS;
}

Scalar Yolo::getBLACK() const
{
	return BLACK;
}

Scalar Yolo::getBLUE() const
{
	return BLUE;
}

Scalar Yolo::getYELLOW() const
{
	return YELLOW;
}

Scalar Yolo::getRED() const
{
	return RED;
}

