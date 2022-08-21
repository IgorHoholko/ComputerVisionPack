#include <iostream>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "utils/creative/cartonify.h"


using namespace cv;
using namespace std;
using namespace CVP;



const String keys =
        "{help h usage ? |      | print this message   }"
        "{@camera_id     |      | camera ID   }";


int main(int argc, char** argv) {
    std::cout << 0 << endl;


    cv::CommandLineParser parser(argc, argv, keys);
    if (parser.has("help")) {
        parser.printMessage();
        return 0;
    }

    std::cout << 0 << endl;

    int camera_id = parser.get<int>(0);

    // Get access to the camera.
    VideoCapture camera(camera_id);

    if (!camera.isOpened()) {
        std::cerr << "ERROR: Could not access the camera or video!" << std::endl;
        exit(1);
    }
    // Try to set the camera resolution.
     camera.set(cv::CAP_PROP_FRAME_WIDTH, 640);
     camera.set(cv::CAP_PROP_FRAME_HEIGHT, 480);


    while (true) {
        // Grab the next camera frame.
        cv::Mat cameraFrame;
        camera >> cameraFrame;
        if (cameraFrame.empty()) {
            std::cerr << "ERROR: Couldn't grab a camera frame." << std::endl;
            exit(1);
        }

        cv::Mat displayedFrame(cameraFrame.size(), CV_8UC1);
        creative::cartonify(cameraFrame, &displayedFrame);

        imshow("Cartoonifier", displayedFrame);
        // IMPORTANT: Wait for at least 20 milliseconds,
        // so that the image can be displayed on the screen!
        // Also checks if a key was pressed in the GUI window.
        // Note that it should be a "char" to support Linux.
        char keypress = cv::waitKey(20);// Need this to see anything!
        if (keypress == 27) {           // Escape Key
            // Quit the program!
            break;
        }
    }// end while

    return 0;
}