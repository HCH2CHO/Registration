#ifndef ARTI_REGISTRATION_H
#define ARTI_REGISTRATION_H

#include<Eigen>

using namespace Eigen;

void calculateMatrix(std::vector<Vector3d> source_input_point,std::vector<Vector3d> target_input_point,int arg,Matrix4f &GlobalTransform);


#endif // ARTI_REGISTRATION_H

