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
 using namespace cimg_library;
 
// #include <boost/thread.hpp>
//#include <thread>
#include <pthread.h>
#include <time.h>
  
#include "behaviour.h"

 

using namespace cv;
using namespace ntk;
//using namespace boost; 
using namespace std;

namespace opt
{
ntk::arg<bool> high_resolution("--highres", "High resolution color image.", 0);
ntk::arg<int> kinect_id("--kinect-id", "Kinect id", 0);
}

volatile bool start=false;

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

void * askStart(void * argument){
     char cont;     
    cout << "Start? (y/n):";
    cin >> cont;
    if (cont == 'y') { //start the behaviour                
        start = true;
    }
}


int main(int argc, char **argv)
{
    // Parse command line options.
    arg_base::set_help_option("-h");
    arg_parse(argc, argv);

    // Set debug level to 1.
    ntk::ntk_debug_level = 0;
    
    //set tilt angle for the Kinect device
    addTiltAngle(0);

    // Set current directory to application directory.
    // This is to find Nite config in config/ directory.
    QApplication app (argc, argv);
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
    
    int userId;
     cout<<"User id: ";
     cin>> userId;    
      
    //continue yes
    char cont = 'y';    
    while (cont == 'y') {
    
        //Init vars
        Behaviour behaviour;
        behaviour.initialize(userId);
        bool first = true;
        int count = 0;
        bool complete;
        char last_c = 0;
        cv::Mat3b debug_color_img;
//        thread thread_1(askStart);     
        pthread_t thread_1; 
        bool threadlaunched=false;
        start=false;
        
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
                if (!start && !threadlaunched) {                                                                      
                    pthread_create( &thread_1, NULL, askStart,NULL); 
                    threadlaunched=true;
                   
                } else if(start) {//started
                    /*We wait 90 frames so the user has time to prepare the initial position of the behaviour*/ 
                    if (count < 90) {
                        if (count % 30 == 0)
                            cout << "Comenzando en " << 3 - count / 30 << endl;
                        count++;
                    }else{
                        if (first) {
                            behaviour.start();
                            first = false;
                        }
                        complete = behaviour.saveData(image.skeletonRef());
                        if (complete) {
                            //debug_color_img.
                            break;
                        }
                    }
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
                bar << "Frame rate: " << fps << " fps";

                displayStatusBar("color", bar.str());
                
            }
            ++frameCounter;           
            //waits for escape key
            last_c = (cv::waitKey(10) & 0xff);

        }
        //Once the loop is finished we save the behaviour
        cout << "Do you want to save the behavoiur? (y/n):";
        cin >> cont;       
        if(cont == 'y') 
           behaviour.finish(&debug_color_img);        

        //continue doing behaviours
        cout << "Do you want to continue? (y/n):";
        cin >> cont;       
    }
    
}
