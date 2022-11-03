#include <iostream>
#include <algorithm>
#include <vector>

#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>

#include "Tools.h"

using namespace std;
using namespace cv;

int main(int argc, char **argv) {


    cv::Mat markerImage, landscape, imgIn, imageCopy, dinosaur, asteroid, ash;

    cv::VideoCapture inputVideo;
    inputVideo.open(0);
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::namedWindow("Out", 0);

    //Tools::createMarker(56);

    landscape = imread("../Resources/new_scenery.jpg");
    dinosaur = imread("../Resources/dinosaur.png", IMREAD_UNCHANGED);
    asteroid = imread("../Resources/asteroid.png", IMREAD_UNCHANGED);
    ash = imread("../Resources/ash.png", IMREAD_UNCHANGED);

    //cv::VideoCapture capOut("../sample.mp4", 2200);
    //capOut>>landscape;

    bool showH2o = false;
    bool showDino = false;
    bool showAsteroid = false;
    bool byPassAsteroid = false;

    Point2i dinoLoc;
    int dinoCornerId;
    Point2i asteroidLoc;

    while (inputVideo.grab()) {
        inputVideo.retrieve(imgIn);
        imgIn.copyTo(imageCopy);

        //capOut>>landscape;

        cv::Mat frame = imgIn.clone();

        std::vector<int> markerIds;
        std::vector<std::vector<cv::Point2f>> markerCorners;
        cv::aruco::detectMarkers(imgIn, dictionary, markerCorners, markerIds);
        std::vector<cv::Point2i> ptsOut(4);

        try {
            // if at least one marker detected
            if (!markerIds.empty()) {
                cv::aruco::drawDetectedMarkers(imgIn, markerCorners, markerIds);

                int cnt = 0;

                for (int i = 0; i < (int) markerIds.size(); i++) {

                    if (markerIds[i] == 98) {
                        cnt++;
                        int x = (int) (markerCorners[i][0].x + markerCorners[i][2].x) / 2;
                        int y = (int) (markerCorners[i][0].y + markerCorners[i][2].y) / 2;

                        ptsOut[0] = cv::Point2i(x, y);

                        /*if (!showH2o)
                            circle(imgIn, ptsOut[0], 5, cv::Scalar(0, 0, 255), 50);*/

                    } else if (markerIds[i] == 124) {
                        cnt++;

                        int x = (int) (markerCorners[i][0].x + markerCorners[i][2].x) / 2;
                        int y = (int) (markerCorners[i][0].y + markerCorners[i][2].y) / 2;

                        ptsOut[1] = cv::Point2i(x, y);

                        /*if (!showH2o)
                            circle(imgIn, ptsOut[1], 5, cv::Scalar(255, 0, 0), 50);*/

                    } else if (markerIds[i] == 203) {
                        cnt++;

                        int x = (int) (markerCorners[i][0].x + markerCorners[i][2].x) / 2;
                        int y = (int) (markerCorners[i][0].y + markerCorners[i][2].y) / 2;

                        ptsOut[2] = cv::Point2i(x, y);

                        /*if (!showH2o)
                            circle(imgIn, ptsOut[2], 5, cv::Scalar(255, 0, 0), 50);*/

                    } else if (markerIds[i] == 62) {
                        cnt++;

                        int x = (int) (markerCorners[i][0].x + markerCorners[i][2].x) / 2;
                        int y = (int) (markerCorners[i][0].y + markerCorners[i][2].y) / 2;

                        ptsOut[3] = cv::Point2i(x, y);
                    }

                    if (markerIds[i] == 23) {
                        showDino = true;

                        int x = (int) (markerCorners[i][0].x + markerCorners[i][2].x) / 2;
                        int y = (int) (markerCorners[i][0].y + markerCorners[i][2].y) / 2;

                        dinoCornerId = i;
                        dinoLoc = cv::Point2i(x, y);
                    }

                    if (markerIds[i] == 40) {
                        showAsteroid = true;
                        int x = (int) (markerCorners[i][0].x + markerCorners[i][2].x) / 2;
                        int y = (int) (markerCorners[i][0].y + markerCorners[i][2].y) / 2;
                        asteroidLoc = cv::Point2i(x, y);
                    }
                }


                /*if (cnt == 3) {
                    // Show euclidean distance between two point
                    double distA = Tools::euclideanDist(ptsOut[0], ptsOut[1]);
                    double distB = Tools::euclideanDist(ptsOut[1], ptsOut[2]);

                    if (distA < 250 && distB < 250) {
                        showH2o = true;
                        int x = (int) ((ptsOut[0].x + ptsOut[1].x) / 2);
                        int y = (int) ((ptsOut[0].y + ptsOut[1].y) / 2);
                        circle(imgIn, cv::Point2i(x - 30, y), 5, cv::Scalar(255, 0, 0), 50);
                        circle(imgIn, cv::Point2i(x + 30, y), 5, cv::Scalar(255, 0, 0), 50);
                        circle(imgIn, cv::Point2i(x, y), 5, cv::Scalar(0, 0, 255), 50);
                    } else {
                        showH2o = false;
                    }
                }*/

                if (cnt == 4) {
                    //destination points
                    cv::Point2f outPoints[4];
                    outPoints[0] = ptsOut[0];
                    outPoints[1] = ptsOut[1];
                    outPoints[2] = ptsOut[2];
                    outPoints[3] = ptsOut[3];

                    Tools::warpImage(frame, landscape, imgIn, outPoints);
                }

                if(showDino) {
                    //Tools::warpOverlayImage(imgIn, dinosaur, imgIn, markerCorners[dinoCornerId]);
                    Tools::overlayImage(imgIn, dinosaur, imgIn, markerCorners[dinoCornerId][0]);
                }

                if(showAsteroid && !byPassAsteroid) {
                    Tools::overlayImage(imgIn, asteroid, imgIn, asteroidLoc);
                }

                if(showAsteroid && showDino) {
                    double distc = Tools::euclideanDist(dinoLoc, asteroidLoc);
                    if(distc < 200) {
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
