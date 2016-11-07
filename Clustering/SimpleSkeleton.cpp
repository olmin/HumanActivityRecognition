/* 
 * File:   SimpleSkeleton.cpp
 * Author: olmin
 * 
 * Created on 4 de noviembre de 2012, 20:49
 */

#include "SimpleSkeleton.h"
#include "vec3.h"

vec3 string2Point3D(string stringPoint){
    vec3 point;
    
    stringstream stream(stringstream::in | stringstream::out);
    stream<<stringPoint;
    
    stream>> point.x;    
    stream>> point.y ;   
    stream>> point.z ;
    
    return point;
}
float calcAngle(vec3 p1Ini, vec3 p1End,
        vec3 p2Ini, vec3 p2End) {

    vec3 v1 = vec3(p1End.x - p1Ini.x, p1End.y - p1Ini.y, p1End.z - p1Ini.z);
    vec3 v2 = vec3(p2End.x - p2Ini.x, p2End.y - p2Ini.y, p2End.z - p2Ini.z);

    float angle = angle2vectors(v1, v2)*180 / 3.1415926; //radians to degrees

    return angle;
}

float calcAngleRef(vec3 p1Ini,vec3 p1End,
        vec3 p2Ini,vec3 p2End) {

    vec3 v1 = vec3(p1End.x - p1Ini.x, p1End.y - p1Ini.y, p1End.z - p1Ini.z);
    vec3 v2 = vec3(p2End.x - p2Ini.x, p2End.y - p2Ini.y, p2End.z - p2Ini.z);

    vec3 vRef = vec3(v2);

    float angle = signed_angle(v1, v2, vRef)*180 / 3.1415926; //radians to degrees

    return angle;
}

SimpleSkeleton fillSkeletonFromDB(int idBeh, ResultSet *resSkeletons, double userHeight){
    
    SimpleSkeleton skel=fillSkeletonFromDB( idBeh,  resSkeletons);
    //relative height to the user's torso height
    skel.height= userHeight-skel.height;
    return skel;
}

SimpleSkeleton fillSkeletonFromDB(int idBeh, ResultSet *resSkeletons){
    SimpleSkeleton skel;
//    string pos3d=resSkeletons->getString("pos3D");
//    int indexHeight = pos3d.find_first_of(',')+1;
//    int lastIndexHeight = pos3d.find_last_of(',')-1;
   // int lengthHeight= pos3d.length()-indexHeight-1;
   
    skel.idBehaviour=idBeh;   
    skel.frame=resSkeletons->getInt("frame");
    
    vec3 head = string2Point3D(resSkeletons->getString(3));
    vec3 neck = string2Point3D(resSkeletons->getString(4));
    vec3 torso = string2Point3D(resSkeletons->getString(5));
    vec3 left_shoulder = string2Point3D(resSkeletons->getString(6));
    vec3 left_elbow = string2Point3D(resSkeletons->getString(7));
    vec3 left_hand = string2Point3D(resSkeletons->getString(8));
    vec3 left_knee = string2Point3D(resSkeletons->getString(9));
    vec3 left_foot = string2Point3D(resSkeletons->getString(10));
    vec3 left_hip =string2Point3D(resSkeletons->getString(11));
    vec3 right_shoulder = string2Point3D(resSkeletons->getString(12));
    vec3 right_elbow = string2Point3D(resSkeletons->getString(13));
    vec3 right_hand = string2Point3D(resSkeletons->getString(14));
    vec3 right_knee = string2Point3D(resSkeletons->getString(15));
    vec3 right_foot = string2Point3D(resSkeletons->getString(16));
    vec3 right_hip = string2Point3D(resSkeletons->getString(17));
    vec3 cameraFloor = vec3(0, 0, 0);
    vec3 frontFloor = vec3(torso.x, 0, torso.z); //height 0 means in the floor     
    
    skel.height=  torso.y;     
    
    //now we calculate the angles of the joints
    skel.angles[0] = calcAngle(neck, torso, torso, frontFloor); //angle torso with the vertical     

    skel.angles[1] = calcAngle(left_elbow, left_shoulder, left_shoulder, right_shoulder); //angle left elbow-shoulders
    skel.angles[2] = calcAngle(left_hand, left_elbow, left_elbow, left_shoulder); //angle left hand-elbow
    skel.angles[3] = calcAngle(left_knee, left_hip, left_hip, right_hip); //angle left knee-hips
    skel.angles[4] = calcAngle(left_foot, left_knee, left_knee, left_hip); //angle left ankle-knee

    skel.angles[5] = calcAngle(right_elbow, right_shoulder, right_shoulder, left_shoulder); //angle right elbow-shoulders
    skel.angles[6] = calcAngle(right_hand, right_elbow, right_elbow, right_shoulder); //angle right hand-elbow
    skel.angles[7] = calcAngle(right_knee, right_hip, right_hip, left_hip); //angle right knee-hips
    skel.angles[8] = calcAngle(right_foot, right_knee, right_knee, right_hip); //angle right ankle-knee


    skel.angles[9] = calcAngle(left_shoulder, right_shoulder, frontFloor, cameraFloor); //angle of rotation of the shoulders
    skel.angles[10] = calcAngle(left_hip, right_hip, frontFloor, cameraFloor); //angle of rotation of the hip
    //    skel.angles[0]=resSkeletons->getDouble("ang0");
    //    skel.angles[1]=resSkeletons->getDouble("ang1");
    //    skel.angles[2]=resSkeletons->getDouble("ang2");
    //    skel.angles[3]=resSkeletons->getDouble("ang3");
    //    skel.angles[4]=resSkeletons->getDouble("ang4");
    //    skel.angles[5]=resSkeletons->getDouble("ang5");
    //    skel.angles[6]=resSkeletons->getDouble("ang6");
    //    skel.angles[7]=resSkeletons->getDouble("ang7");
    //    skel.angles[8]=resSkeletons->getDouble("ang8");
    //    skel.angles[9]=resSkeletons->getDouble("ang9");
    //    skel.angles[10]=resSkeletons->getDouble("ang10");  

    return skel;
}


list<SimpleSkeleton> getAllSimpleSkeleton(Connection *con){
    
     Statement *stmt;
    ResultSet *resBehaviours, *resSkeletons;
    PreparedStatement *prep_stmt;
  
    list<SimpleSkeleton> allSkeletons;
  
    /* create a statement object */
    stmt = con -> createStatement();

    /* run a query which returns exactly one result set */
    resBehaviours = stmt -> executeQuery("SELECT * FROM Behaviour");

    /* fetch the data : retrieve all the rows in the result set */
    //vars
    int idBehaviour;
    int index = 0;

    while (resBehaviours->next()) {
        idBehaviour = resBehaviours -> getInt("id");
        prep_stmt = con->prepareStatement("SELECT * FROM Skeleton WHERE id_behaviour=?");
        prep_stmt -> setInt(1, idBehaviour);
        resSkeletons = prep_stmt -> executeQuery();

        index = 0;
        while (resSkeletons->next()) {
            SimpleSkeleton skel=fillSkeletonFromDB(idBehaviour,resSkeletons);
            skel.normalizeSkel();
            allSkeletons.push_back(skel);            
        }//end while Skeletons            
    } // while Behaviours

    /* Clean up */
    delete resBehaviours;
    delete resSkeletons;
    delete stmt;
    delete prep_stmt;
    
    return allSkeletons;
}