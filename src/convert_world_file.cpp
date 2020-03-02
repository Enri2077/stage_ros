/*
 *  stageros
 *  Copyright (c) 2008, Willow Garage, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sys/stat.h>

#include <stage.hh>
#include <ros/ros.h>

using namespace std;

int main(int argc, char** argv){

    if(argc != 3){
        cerr << "Usage: convert_world_file input_world_path output_yaml_path" << endl;
        exit(-1);
    }
    cerr << argv[0] << ", " << argv[1] << ", " << argv[2] << endl;

    auto world_file_path = argv[1];
    auto output_file_path = argv[2];

    struct stat world_file_path_stat;
    if(stat(world_file_path, &world_file_path_stat) != 0){
        cerr << "The world file does not exist: " << world_file_path << endl;
        exit(-1);
    }

    // initialize libstage and load the worldfile
    Stg::Init( &argc, &argv );
    auto* world = new Stg::World();
    world->Load(world_file_path);

    struct stat output_file_path_stat;
    if(stat(world_file_path, &output_file_path_stat) != 0){
        cout << "Writing output file " << output_file_path << endl;
    } else {
        cout << "Overwriting output file " << output_file_path << endl << endl;
    }

    ofstream out_file(output_file_path);

    Stg::Model* robot = world->GetModel("robot");
    Stg::Pose robot_pose = robot->GetPose();
    Stg::Model* floorplan = world->GetModel("map");
    Stg::Pose floorplan_pose = floorplan->GetPose();
    Stg::Size floorplan_size = floorplan->GetGeom().size;
    
    out_file << "robot: " << endl;
    out_file << "  pose: " << endl;
    out_file << "    x: " << robot_pose.x << endl;
    out_file << "    y: " << robot_pose.y << endl;
    out_file << "    z: " << robot_pose.z << endl;
    out_file << "    theta: " << robot_pose.a << endl;
    out_file << "map: " << endl;
    out_file << "  pose: " << endl;
    out_file << "    x: " << floorplan_pose.x << endl;
    out_file << "    y: " << floorplan_pose.y << endl;
    out_file << "    z: " << floorplan_pose.z << endl;
    out_file << "    theta: " << floorplan_pose.a << endl;
    out_file << "  size: " << endl;
    out_file << "    x: " << floorplan_size.x << endl;
    out_file << "    y: " << floorplan_size.y << endl;
    out_file << "    z: " << floorplan_size.z << endl;
    out_file << "  resolution: " << 1.0/world->Resolution() << endl;

    out_file.close();

    exit(0);
}

