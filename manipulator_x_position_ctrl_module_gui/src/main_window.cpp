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
 * main_window.cpp
 *
 *  Created on: Jul 6, 2016
 *      Author: sch, Darby Lim
 */

#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "../include/manipulator_x_position_ctrl_module_gui/main_window.hpp"

namespace manipulator_x_position_ctrl_module_gui
{
using namespace Qt;

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
	: QMainWindow(parent)
  , qnode_(argc,argv)
{
  ui_.setupUi(this);
  QObject::connect(ui_.actionAbout_Qt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));

	setWindowIcon(QIcon(":/images/icon.png"));
  ui_.tab_manager->setCurrentIndex(0);
  QObject::connect(&qnode_, SIGNAL(rosShutdown()), this, SLOT(close()));

  ui_.view_logging->setModel(qnode_.loggingModel());
  QObject::connect(&qnode_, SIGNAL(loggingUpdated()), this, SLOT(updateLoggingView()));

  qRegisterMetaType<manipulator_x_position_ctrl_module_msgs::JointPose>("manipulator_x_position_ctrl_module_msgs::JointPose");
  QObject::connect(&qnode_, SIGNAL(updateJointPresentPose(manipulator_x_position_ctrl_module_msgs::JointPose)),
                   this, SLOT(updateJointPresentPoseLineEdit(manipulator_x_position_ctrl_module_msgs::JointPose)));

 // ui_.gripper_goal_position_horizontalSlider->
  QObject::connect(ui_.gripper_goal_position_horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(changeGripperPosition(int)));

  QObject::connect(ui_.manipulator_x4_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changeControlMode(int)));
  qnode_.init();
}

MainWindow::~MainWindow() {}

void MainWindow::updateLoggingView()
{
  ui_.view_logging->scrollToBottom();
}

void MainWindow::on_set_control_mode_pushButton_clicked(bool check)
{
  std_msgs::String msg;
  msg.data = "set_module";

  qnode_.sendSetModuleMsg(msg);
}

void MainWindow::on_zero_position_pushButton_clicked(bool check)
{
  std_msgs::String msg;
  msg.data = "init_position";

  qnode_.setZeroPosition(msg);
}

void MainWindow::on_init_position_pushButton_clicked(bool check)
{
  std_msgs::String msg;
  msg.data = "zero_position";

  qnode_.setInitPosition(msg);
}

void MainWindow::changeControlMode(int index)
{
  std_msgs::String str_msg;

  if (index == JOINT_CONTROL)
  {
    str_msg.data = "set_joint_control_mode";
    qnode_.sendEnableJointControlMode(str_msg);
  }
  else if (index == TASK_SPACE_CONTROL)
  {
    str_msg.data = "set_task_space_control_mode";
    qnode_.sendEnableTaskSpaceControlMode(str_msg);
  }
}

void MainWindow::on_gripper_goal_position_toggleButton_clicked(bool check)
{
  std_msgs::Float64 position;


  if (ui_.gripper_goal_position_toggleButton->isChecked())
  {
    ui_.gripper_goal_position_toggleButton->setText("Gripper On");

    position.data = 0.0 * DEGREE2RADIAN;
    qnode_.sendGripperGoalPositionMsg(position);

    ui_.gripper_present_position_lineEdit->setText(QString::number(0.0));
    ui_.gripper_goal_position_horizontalSlider->setSliderPosition(0);
  }
  else
  {
    ui_.gripper_goal_position_toggleButton->setText("Gripper Off");

    position.data = 170.0 * DEGREE2RADIAN;
    qnode_.sendGripperGoalPositionMsg(position);

    ui_.gripper_present_position_lineEdit->setText(QString::number(140.0));
    ui_.gripper_goal_position_horizontalSlider->setSliderPosition(140);
  }
}

void MainWindow::changeGripperPosition(int position)
{
  std_msgs::Float64 gripper_position;
  gripper_position.data = position * DEGREE2RADIAN;

  qnode_.sendGripperGoalPositionMsg(gripper_position);

  ui_.gripper_present_position_lineEdit->setText(QString::number(position));
}

void MainWindow::on_send_goal_position_pushButton_clicked(bool check)
{
  manipulator_x_position_ctrl_module_msgs::JointPose msg;

  msg.move_time = 1.5;
  msg.joint_name.push_back("joint1");
  msg.joint_name.push_back("joint2");
  msg.joint_name.push_back("joint3");
  msg.joint_name.push_back("joint4");

  msg.position.push_back(ui_.joint1_goal_position_doubleSpinBox->value()*DEGREE2RADIAN);
  msg.position.push_back(ui_.joint2_goal_position_doubleSpinBox->value()*DEGREE2RADIAN);
  msg.position.push_back(ui_.joint3_goal_position_doubleSpinBox->value()*DEGREE2RADIAN);
  msg.position.push_back(ui_.joint4_goal_position_doubleSpinBox->value()*DEGREE2RADIAN);

  qnode_.sendJointGoalPositionMsg(msg);
}

void MainWindow::updateJointPresentPoseLineEdit(manipulator_x_position_ctrl_module_msgs::JointPose msg)
{
  ui_.joint1_present_position_lineEdit->setText(QString::number(msg.position[0]*RADIAN2DEGREE, 1, 1));
  ui_.joint2_present_position_lineEdit->setText(QString::number(msg.position[1]*RADIAN2DEGREE, 1, 1));
  ui_.joint3_present_position_lineEdit->setText(QString::number(msg.position[2]*RADIAN2DEGREE, 1, 1));
  ui_.joint4_present_position_lineEdit->setText(QString::number(msg.position[3]*RADIAN2DEGREE, 1, 1));
}

void MainWindow::on_actionAbout_triggered()
{
  QMessageBox::about(this, tr("About ..."),tr("<p>Copyright ROBOTIS</p>"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	QMainWindow::closeEvent(event);
}

}  // namespace manipulator_x_position_ctrl_module_gui

