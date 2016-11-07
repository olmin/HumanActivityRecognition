/* 
 * File:   main.cpp
 * Author: olmin kk
 *
 * Created on 21 de septiembre de 2011, 13:08
 */

#include <ntk/ntk.h>
#include <ntk/utils/debug.h>
#include <ntk/camera/openni_grabber.h>
#include <ntk/gesture/body_event.h>

#include <QApplication>
#include <QDir>
#include <QMutex>

#include <libfreenect/libfreenect.h>

#include <ntk/camera/rgbd_image.h>
#include <ntk/gesture/skeleton.h>
//#include <openni/Source/External/LibJPEG/jmorecfg.h>
#include <jmorecfg.h>

#include "CImg.h"

#include "BehaviourDetection.h"
#include "../Clustering/vec3.h"
#include "../Clustering/SimpleSkeleton.h"
#include <time.h>

using namespace cimg_library;
using namespace cv;
using namespace ntk;

namespace opt {
    ntk::arg<bool> high_resolution("--highres", "High resolution color image.", 0);
    ntk::arg<int> kinect_id("--kinect-id", "Kinect id", 0);
}

float calcAngle(cv::Point3f p1Ini, cv::Point3f p1End,
        cv::Point3f p2Ini, cv::Point3f p2End) {

    vec3 v1 = vec3(p1End.x - p1Ini.x, p1End.y - p1Ini.y, p1End.z - p1Ini.z);
    vec3 v2 = vec3(p2End.x - p2Ini.x, p2End.y - p2Ini.y, p2End.z - p2Ini.z);

    float angle = angle2vectors(v1, v2)*180 / 3.1415926; //radians to degrees

    return angle;
}
SimpleSkeleton createSimpleSkeleton(ntk::Skeleton* skeleton) {

    Point3f head = skeleton->getJoint(skeleton->NTK_SKEL_HEAD);
    Point3f neck = skeleton->getJoint(skeleton->NTK_SKEL_NECK);
    Point3f torso = skeleton->getJoint(skeleton->NTK_SKEL_TORSO);

    Point3f left_shoulder = skeleton->getJoint(skeleton->NTK_SKEL_LEFT_SHOULDER);
    Point3f left_elbow = skeleton->getJoint(skeleton->NTK_SKEL_LEFT_ELBOW);
    Point3f left_hand = skeleton->getJoint(skeleton->NTK_SKEL_LEFT_HAND);
    Point3f left_knee = skeleton->getJoint(skeleton->NTK_SKEL_LEFT_KNEE);
    Point3f left_foot = skeleton->getJoint(skeleton->NTK_SKEL_LEFT_FOOT);
    Point3f left_hip = skeleton->getJoint(skeleton->NTK_SKEL_LEFT_HIP);

    Point3f right_shoulder = skeleton->getJoint(skeleton->NTK_SKEL_RIGHT_SHOULDER);
    Point3f right_elbow = skeleton->getJoint(skeleton->NTK_SKEL_RIGHT_ELBOW);
    Point3f right_hand = skeleton->getJoint(skeleton->NTK_SKEL_RIGHT_HAND);
    Point3f right_knee = skeleton->getJoint(skeleton->NTK_SKEL_RIGHT_KNEE);
    Point3f right_foot = skeleton->getJoint(skeleton->NTK_SKEL_RIGHT_FOOT);
    Point3f right_hip = skeleton->getJoint(skeleton->NTK_SKEL_RIGHT_HIP);

    Point3f cameraFloor = Point3f(0, 0, 0);
    Point3f frontFloor = Point3f(torso.x, 0, torso.z); //height 0 means in the floor

    //initializing the skeleton we are going to save
    SimpleSkeleton simpleSkeleton = SimpleSkeleton();

    // simpleSkeleton.timestamp = time(NULL);
    simpleSkeleton.height = torso.y; // the 3d position is taken from the central point of the person

    //now we calculate the angles of the joints
    simpleSkeleton.angles[0] = calcAngle(neck, torso, torso, frontFloor); //angle torso with the vertical     

    simpleSkeleton.angles[1] = calcAngle(left_elbow, left_shoulder, left_shoulder, right_shoulder); //angle left elbow-shoulders
    simpleSkeleton.angles[2] = calcAngle(left_hand, left_elbow, left_elbow, left_shoulder); //angle left hand-elbow
    simpleSkeleton.angles[3] = calcAngle(left_knee, left_hip, left_hip, right_hip); //angle left knee-hips
    simpleSkeleton.angles[4] = calcAngle(left_foot, left_knee, left_knee, left_hip); //angle left ankle-knee

    simpleSkeleton.angles[5] = calcAngle(right_elbow, right_shoulder, right_shoulder, left_shoulder); //angle right elbow-shoulders
    simpleSkeleton.angles[6] = calcAngle(right_hand, right_elbow, right_elbow, right_shoulder); //angle right hand-elbow
    simpleSkeleton.angles[7] = calcAngle(right_knee, right_hip, right_hip, left_hip); //angle right knee-hips
    simpleSkeleton.angles[8] = calcAngle(right_foot, right_knee, right_knee, right_hip); //angle right ankle-knee


    simpleSkeleton.angles[9] = calcAngle(left_shoulder, right_shoulder, frontFloor, cameraFloor); //angle of rotation of the shoulders
    simpleSkeleton.angles[10] = calcAngle(left_hip, right_hip, frontFloor, cameraFloor); //angle of rotation of the hip

    
    return simpleSkeleton;
}

void addTiltAngle(double angle2add){
    /****by Olmo set the kinect tilt position*****/   
    static double angle=KINECT_TILT_ANGLE;   
    freenect_device *dev;
    freenect_context *ctx;
    int index=0;
    int response;
    
    angle+=angle2add;
    
    //cout<<"Starting Freenect for adjusting tilt degrees"<<endl;    
    response= freenect_init(&ctx, NULL);
    response= freenect_open_device(ctx, &dev, index);   
    response= freenect_set_tilt_degs(dev, angle);    
    response= freenect_shutdown(ctx);    
    cout<<"Freenect: set tilt angle to "<<angle<<endl<<endl;
    /********END by Olmo titl position*********/
}

int main(int argc, char **argv) {
    // Parse command line options.
    arg_base::set_help_option("-h");
    arg_parse(argc, argv);

    // Set debug level to 1.
    ntk::ntk_debug_level = 0;

    //set tilt angle for the Kinect device
    addTiltAngle(0);
    
    // Set current directory to application directory.
    // This is to find Nite config in config/ directory.
    QApplication app(argc, argv);
    QDir::setCurrent(QApplication::applicationDirPath());

    // Declare the global OpenNI driver. Only one can be instantiated in a program.
    OpenniDriver ni_driver;

    // Declare the frame grabber.
    OpenniGrabber grabber(ni_driver, opt::kinect_id());

    // High resolution 1280x1024 RGB Image.
    if (opt::high_resolution())
        grabber.setHighRgbResolution(true);

    // Use mirrored images.
    grabber.setMirrored(true);

    // Start the grabber.
    grabber.connectToDevice();
    grabber.start();   

    // Holder for the current image.
    RGBDImage image;

    // Image post processor. Compute mappings when RGB resolution is 1280x1024.
    OpenniRGBDProcessor post_processor;

    //for calculating fps
    unsigned int frameCounter = 1;
    time_t lastCheckTime =time (NULL);
    time_t seconds;

    //continue yes
    char cont = 'y';
    while (cont == 'y') {

        //Init vars
        BehaviourDetection actionsDetection(argc, argv);
        string actionDetected;
        
        bool first = true;
        int count = 0;     
        char last_c = 0;
        cv::Mat3b debug_color_img;

        while (true && (last_c != 27)) {
            // Wait for a new frame, get a local copy and postprocess it.
            grabber.waitForNextFrame();
            grabber.copyImageTo(image);
            post_processor.processImage(image);

            // Prepare the color view (mapped to depth frame), with skeleton and handpoint.

            image.mappedRgb().copyTo(debug_color_img);           
            if (image.skeleton())
                image.skeleton()->drawOnImage(debug_color_img);


            /*****************************CONTROL BEHAVIOUR******************************************************/
            Point3f iniPoint = Point3f(0, 0, 0);
            if (image.skeleton() && (image.skeleton()->getJoint(image.skeleton()->NTK_SKEL_TORSO) != iniPoint)) {//tracking user                   
                if (first) {
                    /*We wait 90 frames so the user has time to prepare the initial position of the behaviour*/
                    if (count < 90) {
                        if (count % 30 == 0)
                            cout << "Starting in " << 3 - count / 30 << endl;
                        count++;
                    } else {
                        first = false;
                    }
                } else {//not first
                    //here we pass the skeletons to the behaviourdetection
                   string answer= actionsDetection.addFrame(createSimpleSkeleton(image.skeletonRef()));                  
                   if(answer!="")
                       actionDetected=answer;
                }

            }
            /*****************************END CONTROL BEHAVIOUR******************************************************/

            /*shows the image*/
            imshow("color", debug_color_img);                  

            seconds = time (NULL);
            float elapsed = seconds - lastCheckTime;
            if (elapsed > 1.0) {
                float fps = (float) frameCounter / (float) elapsed;
                lastCheckTime = time (NULL);
                frameCounter = 0;
//                double elapsedSeconds = elapsedMS / 1000.0; // Convert to seconds
//                double fps = numFrames / elapsedSeconds; // FPS is Frames / Seconds             
                stringstream bar;
                bar << "Action Detected: "<<actionDetected<<"                   Frame rate: " << fps << " fps";

                displayStatusBar("color", bar.str());
                
            }
            ++frameCounter;           

            //waits for escape key
            last_c = (cv::waitKey(10) & 0xff);

        }
       
        //continue
        cout << "Do you want to continue? (y/n):";
        cin >> cont;
    }

    
}
