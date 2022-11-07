#include <iostream>
#include <algorithm>
#include <vector>

#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>

#include "Tools.h"

using namespace std;
using namespace cv;

int main(int argc, char **argv) {

    cv::Mat markerImage, landscape, imgIn, imageCopy, dinosaur, asteroid, ash, markersSource;

    cv::VideoCapture inputVideo;
    inputVideo.open(0);

    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::namedWindow("Out", 0);

    //Tools::createMarker(42);
    markersSource = imread("../Resources/markers-source.png");

    landscape = imread("../Resources/new_scenery.jpg");
    dinosaur = imread("../Resources/dinosaur.png", IMREAD_UNCHANGED);
    asteroid = imread("../Resources/asteroid.png", IMREAD_UNCHANGED);
    ash = imread("../Resources/ash.png", IMREAD_UNCHANGED);

    /*cv::VideoCapture cap("../Resources/sample.mp4");
    // Check if camera opened successfully
    if(!cap.isOpened()){
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
    cap>>landscape;*/

    bool showDino = false;
    bool showAsteroid = false;
    bool byPassAsteroid = false;

    Point2f dinoLoc;
    int dinoCornerId;
    Point2f asteroidLoc;
    double distance;

    while (inputVideo.grab()) {
        //inputVideo.retrieve(imgIn);

        markersSource.copyTo(imgIn);
        imgIn.copyTo(imageCopy);

        //cap>>landscape;

        cv::Mat frame = imgIn.clone();

        std::vector<int> markerIds;
        std::vector<std::vector<cv::Point2f>> markerCorners;
        cv::aruco::detectMarkers(imgIn, dictionary, markerCorners, markerIds);

        try {
            // if at least one marker detected
            if (!markerIds.empty()) {
                cv::aruco::drawDetectedMarkers(imgIn, markerCorners, markerIds);

                std::vector<int> idsToFind { 203, 124, 98, 62 };
                std::vector<cv::Point2f> ptsOut = Tools::findMarkersWithId(idsToFind, markerIds, markerCorners);
                if (ptsOut.size() == 4) {
                    Tools::warpImage(frame, landscape, imgIn, ptsOut);
                }

                for (int i = 0; i < (int) markerIds.size(); i++) {
                    if (markerIds[i] == 23) {
                        showDino = true;
                        dinoCornerId = i;
                        dinoLoc = Tools::findMarkerCenter(markerCorners[i]);
                    } else if (markerIds[i] == 40) {
                        showAsteroid = true;
                        asteroidLoc = Tools::findMarkerCenter(markerCorners[i]);
                    }
                }


                if(showDino) {
                    Tools::warpOverlayImage(imgIn, dinosaur, imgIn, markerCorners[dinoCornerId]);
                    //Tools::overlayImage(imgIn, dinosaur, imgIn, markerCorners[dinoCornerId][0]);
                }

                if(showAsteroid && !byPassAsteroid) {
                    Tools::overlayImage(imgIn, asteroid, imgIn, asteroidLoc);
                }

                if(showAsteroid && showDino) {
                    distance = Tools::euclideanDist(dinoLoc, asteroidLoc);
                    if(distance < 200) {
                        dinosaur = ash;
                        byPassAsteroid = true;
                    }
                }


            }

        } catch (const std::exception &e) {
            cout << endl << " e : " << e.what() << endl;
            cout << "Could not do warpPerspective !! " << endl;
        }

        cv::imshow("Out", imgIn);
        char key = (char) cv::waitKey(40);
        if (key == 27)
            break;
    }

    return 0;
}
