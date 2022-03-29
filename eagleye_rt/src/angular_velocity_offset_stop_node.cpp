// Copyright (c) 2019, Map IV, Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of the Map IV, Inc. nor the names of its contributors
//   may be used to endorse or promote products derived from this software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/*
 * angular_velocity_offset_stop.cpp
 * Author MapIV Sekino
 */

#include "ros/ros.h"
#include "coordinate/coordinate.hpp"
#include "navigation/navigation.hpp"

static geometry_msgs::TwistStamped velocity;
static ros::Publisher pub;
static eagleye_msgs::AngularVelocityOffset angular_velocity_offset_stop;
static sensor_msgs::Imu imu;

struct AngularVelocityOffsetStopParameter angular_velocity_offset_stop_parameter;
struct AngularVelocityOffsetStopStatus angular_velocity_offset_stop_status;

void velocity_callback(const geometry_msgs::TwistStamped::ConstPtr& msg)
{
  velocity = *msg;
}

void imu_callback(const sensor_msgs::Imu::ConstPtr& msg)
{
  imu = *msg;
  angular_velocity_offset_stop.header = msg->header;
  angular_velocity_offset_stop_estimate(velocity, imu, angular_velocity_offset_stop_parameter, &angular_velocity_offset_stop_status, &angular_velocity_offset_stop);
  pub.publish(angular_velocity_offset_stop);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "angular_velocity_offset_stop");
  ros::NodeHandle n;

  std::string subscribe_twist_topic_name = "/can_twist";
  std::string subscribe_imu_topic_name = "/imu/data_raw";

  n.getParam("twist_topic",subscribe_twist_topic_name);
  n.getParam("imu_topic",subscribe_imu_topic_name);
  n.getParam("reverse_imu", angular_velocity_offset_stop_parameter.reverse_imu);
  n.getParam("angular_velocity_offset_stop/stop_judgment_velocity_threshold",angular_velocity_offset_stop_parameter.stop_judgment_velocity_threshold);
  n.getParam("angular_velocity_offset_stop/estimated_number",angular_velocity_offset_stop_parameter.estimated_number);
  n.getParam("angular_velocity_offset_stop/outlier_threshold",angular_velocity_offset_stop_parameter.outlier_threshold);

  std::cout<< "subscribe_twist_topic_name "<<subscribe_twist_topic_name<<std::endl;
  std::cout<< "subscribe_imu_topic_name "<<subscribe_imu_topic_name<<std::endl;
  std::cout<< "reverse_imu "<<angular_velocity_offset_stop_parameter.reverse_imu<<std::endl;
  std::cout<< "stop_judgment_velocity_threshold "<<angular_velocity_offset_stop_parameter.stop_judgment_velocity_threshold<<std::endl;
  std::cout<< "estimated_number "<<angular_velocity_offset_stop_parameter.estimated_number<<std::endl;
  std::cout<< "outlier_threshold "<<angular_velocity_offset_stop_parameter.outlier_threshold<<std::endl;

  ros::Subscriber sub1 = n.subscribe(subscribe_twist_topic_name, 1000, velocity_callback, ros::TransportHints().tcpNoDelay());
  ros::Subscriber sub2 = n.subscribe(subscribe_imu_topic_name, 1000, imu_callback, ros::TransportHints().tcpNoDelay());
  pub = n.advertise<eagleye_msgs::AngularVelocityOffset>("angular_velocity_offset_stop", 1000);

  ros::spin();

  return 0;
}
