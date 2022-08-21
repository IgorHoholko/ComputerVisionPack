#include <iostream>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "common/common.h"
#include "common/imgs_manipulation.h"
#include "utils/creative/cartonify.h"


using namespace cv;
using namespace std;
using namespace CVP;

size_t constexpr FRAME_WIDTH  = 640;
size_t constexpr FRAME_HEIGHT = 480;

int constexpr MIN_REPETITIONS           = 1;
int constexpr MAX_REPETITIONS           = 15;
int constexpr MIN_RESCALE_FACTOR        = 1;
int constexpr MAX_RESCALE_FACTOR        = 6;
int constexpr MIN_MEDIAN_FILTER_SIZE    = 3;
int constexpr MAX_MEDIAN_FILTER_SIZE    = 21;
int constexpr MIN_LAPLACIAN_FILTER_SIZE = 3;
int constexpr MAX_LAPLACIAN_FILTER_SIZE = 21;
int constexpr MIN_BILATERAL_KETNEL_SIZE = 3;
int constexpr MAX_BILATERAL_KETNEL_SIZE = 33;
int constexpr MIN_EDGES_THRESHOLD       = 5;
int constexpr MAX_EDGES_THRESHOLD       = 255;
int constexpr MIN_SIGMA_SPACE           = 3;
int constexpr MAX_SIGMA_SPACE           = 100;
int constexpr MIN_SIGMA_COLOR           = 3;
int constexpr MAX_SIGMA_COLOR           = 100;


const String keys =
        "{help h usage ? |        | Specify camera id OR image path OR video path  }"
        "{@camera_id     |    0   | camera ID   }";


static void empty(int, void*) {}


int main(int argc, char** argv) {
    std::cout << 0 << endl;


    cv::CommandLineParser parser(argc, argv, keys);
    if (parser.has("help")) {
        parser.printMessage();
        return 0;
    }

    int camera_id = parser.get<int>(0);

    VideoCapture camera(camera_id);

    if (!camera.isOpened()) {
        std::cerr << "ERROR: Could not access the camera or video!" << std::endl;
        exit(1);
    }
    // Try to set the camera resolution.
    camera.set(cv::CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    camera.set(cv::CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);


    cv::Mat frame;
    cv::Mat frame_cartoon(frame.size(), CV_8UC3);
    cv::Mat frame_edges;
    cv::Mat frame_cartoon_raw;

    creative::CartoonSettings settings;

    cv::namedWindow("TrackBars");

    // need to process not int values separetely. Becasue Trackbar doesn't support it directly
    int edges_threshold = settings.edges_threshold,
        sigma_space     = settings.sigma_space,
        sigma_color     = settings.sigma_color;


    cv::createTrackbar("repetitions", "TrackBars", &settings.repetitions, MAX_REPETITIONS, empty);
    cv::createTrackbar("rescale_fcr", "TrackBars", &settings.rescale_factor, MAX_RESCALE_FACTOR, empty);
    cv::createTrackbar("median_ks", "TrackBars", &settings.median_filter_size, MAX_MEDIAN_FILTER_SIZE, empty);
    cv::createTrackbar("laplacian_ks", "TrackBars", &settings.laplacian_filter_size, MAX_LAPLACIAN_FILTER_SIZE, empty);
    cv::createTrackbar("bilateral_ks", "TrackBars", &settings.bilateral_kernel_size, MAX_BILATERAL_KETNEL_SIZE, empty);

    cv::createTrackbar("edges_t", "TrackBars", &edges_threshold, MAX_EDGES_THRESHOLD, empty);
    cv::createTrackbar("sigma_space", "TrackBars", &sigma_space, MAX_SIGMA_SPACE, empty);
    cv::createTrackbar("sigma_color", "TrackBars", &sigma_color, MAX_SIGMA_COLOR, empty);

    while (true) {
        camera >> frame;
        if (frame.empty()) {
            std::cerr << "ERROR: Couldn't grab a camera frame." << std::endl;
            exit(1);
        }

        make_odd(settings.median_filter_size);
        make_odd(settings.laplacian_filter_size);
        make_odd(settings.bilateral_kernel_size);

        settings.median_filter_size = std::max(settings.median_filter_size, MIN_MEDIAN_FILTER_SIZE);
        settings.laplacian_filter_size = std::max(settings.laplacian_filter_size, MIN_LAPLACIAN_FILTER_SIZE);
        settings.bilateral_kernel_size = std::max(settings.bilateral_kernel_size, MIN_BILATERAL_KETNEL_SIZE);
        settings.rescale_factor = std::max(settings.rescale_factor, MIN_RESCALE_FACTOR);
        settings.repetitions = std::max(settings.repetitions, MIN_REPETITIONS);

        settings.edges_threshold = std::max(edges_threshold, MIN_EDGES_THRESHOLD);
        settings.sigma_space = std::max(sigma_space, MIN_SIGMA_SPACE);
        settings.sigma_color = std::max(sigma_color, MIN_SIGMA_COLOR);


        creative::cartonify(frame, &frame_cartoon, settings, &frame_edges, &frame_cartoon_raw);


        cv::Mat res = stackImages({frame, frame_cartoon, frame_edges, frame_cartoon_raw}, frame.size());

        //        imshow("Cartoonifier", res);
        imshow("TrackBars", res);
        char keypress = cv::waitKey(50);
        if (keypress == 27) {// Esc
            break;
        }
    }

    return 0;
}