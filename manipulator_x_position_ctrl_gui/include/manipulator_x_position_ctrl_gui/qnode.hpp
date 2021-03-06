/*******************************************************************************
 * Copyright (c) 2016, ROBOTIS CO., LTD.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of ROBOTIS nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

/*
 * qnode.hpp
 *
 *  Created on: Jul 22, 2016
 *      Author: sch
 */

/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef manipulator_x_position_ctrl_gui_QNODE_HPP_
#define manipulator_x_position_ctrl_gui_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#ifndef Q_MOC_RUN

#include <ros/ros.h>
#include <string>
#include <QThread>
#include <QStringListModel>

#include <ros/package.h>
#include <yaml-cpp/yaml.h>

#include <std_msgs/String.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float64.h>
#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Wrench.h>

#include "robotis_math/robotis_math.h"
#include "robotis_controller_msgs/StatusMsg.h"

#include "manipulator_x_position_ctrl_module_msgs/JointPose.h"
#include "manipulator_x_position_ctrl_module_msgs/KinematicsPose.h"

#include "manipulator_x_position_ctrl_module_msgs/GetJointPose.h"
#include "manipulator_x_position_ctrl_module_msgs/GetKinematicsPose.h"

#endif

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace manipulator_x_position_ctrl_gui {

/*****************************************************************************
** Class
*****************************************************************************/

class QNode : public QThread {
    Q_OBJECT
public:
    QNode(int argc, char** argv );
    virtual ~QNode();
    bool init();
    void run();

    /*********************
    ** Logging
    **********************/
    enum LogLevel
    {
        Debug,
        Info,
        Warn,
        Error,
        Fatal
    };

    QStringListModel* loggingModel() { return &logging_model; }
    void log( const LogLevel &level, const std::string &msg, std::string sender);

    void statusMsgCallback(const robotis_controller_msgs::StatusMsg::ConstPtr &msg);

    void sendSetModeMsg(std_msgs::String msg);
    void sendInitialPoseMsg(std_msgs::String msg);
    void sendJointPoseMsg(manipulator_x_position_ctrl_module_msgs::JointPose msg);
    void sendKinematicsPoseMsg(manipulator_x_position_ctrl_module_msgs::KinematicsPose msg);
    void sendGripperPoseMsg(std_msgs::Float64 msg);

    void enableJointSpaceControl(std_msgs::Bool msg);
    void enableTaskSpaceControl(std_msgs::Bool msg);
    void enableMotionPlanning(std_msgs::Bool msg);

    void getJointPose();
    void getKinematicsPose();

Q_SIGNALS:
    void loggingUpdated();
    void rosShutdown();

    void updateJointPose(manipulator_x_position_ctrl_module_msgs::JointPose);
    void updateKinematicsPose(manipulator_x_position_ctrl_module_msgs::KinematicsPose);

private:
    int init_argc;
    char** init_argv;
    QStringListModel logging_model;

    ros::Publisher set_position_ctrl_mode_msg_pub_;
    ros::Publisher set_gripper_mode_msg_pub_;
    ros::Publisher set_initial_pose_msg_pub_;
    ros::Publisher set_joint_pose_msg_pub_;
    ros::Publisher set_kinematics_pose_msg_pub_;
    ros::Publisher set_gripper_pose_msg_pub_;

    ros::Publisher enable_joint_space_control_pub_;
    ros::Publisher enable_task_space_control_pub_;
    ros::Publisher enable_motion_planning_pub_;

    ros::ServiceClient get_joint_pose_client_;
    ros::ServiceClient get_kinematics_pose_client_;

    ros::Subscriber status_msg_sub_;
};

}  // namespace manipulator_x_position_ctrl_gui

#endif /* manipulator_x_position_ctrl_gui_QNODE_HPP_ */
