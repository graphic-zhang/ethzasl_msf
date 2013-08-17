/*
 * Copyright (C) 2012-2013 Simon Lynen, ASL, ETH Zurich, Switzerland
 * You can contact the author at <slynen at ethz dot ch>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MSF_STATEDEF_HPP_
#define MSF_STATEDEF_HPP_

#include <Eigen/Dense>
#include <msf_core/msf_fwds.h>
#include <boost/fusion/container.hpp>
namespace msf_updates {
/*
 * This file contains the state definition of the EKF as defined for a given set
 * of sensors / states to estimate.
 */
enum StateDefinition {  //must not manually set the enum values!
  p,
  v,
  q,
  b_w,
  b_a,
  L,
  q_wv,
  p_wv,
  q_ic,
  p_ic,
  p_ip
};

namespace {
/***
 * Setup core state, then auxiliary state.
 */
typedef boost::fusion::vector<
    // States varying during propagation - must not change the ordering here for
    // now, CalcQ has the ordering hardcoded.
    msf_core::StateVar_T<Eigen::Matrix<double, 3, 1>, p,
        msf_core::CoreStateWithPropagation>,  ///< Position (IMU centered).         (0-2 / 0-2)
    msf_core::StateVar_T<Eigen::Matrix<double, 3, 1>, v,
        msf_core::CoreStateWithPropagation>,  ///< Velocity.                         (3- 5 / 3- 5)
    msf_core::StateVar_T<Eigen::Quaternion<double>, q,
        msf_core::CoreStateWithPropagation>,  ///< Attitude.                         (6- 9 / 6- 8)
    msf_core::StateVar_T<Eigen::Matrix<double, 3, 1>, b_w,
        msf_core::CoreStateWithoutPropagation>,  ///< Gyro biases.                      (10-12 / 9-11)
    msf_core::StateVar_T<Eigen::Matrix<double, 3, 1>, b_a,
        msf_core::CoreStateWithoutPropagation>,  ///< Acceleration biases.              (13-15 / 12-14)

    // States not varying during propagation.
    msf_core::StateVar_T<Eigen::Matrix<double, 1, 1>, L>,      ///< Visual scale.
    msf_core::StateVar_T<Eigen::Quaternion<double>, q_wv,///< Vision-world position drift.
        msf_core::AuxiliaryNonTemporalDrifting>,  ///< Vision-world attitude drift.
    msf_core::StateVar_T<Eigen::Matrix<double, 3, 1>, p_wv>,  ///< Vision world position drift.
    msf_core::StateVar_T<Eigen::Quaternion<double>, q_ic>,  ///< Relative attitude of the camera w.r.t. the IMU expressed in the IMU frame.
    msf_core::StateVar_T<Eigen::Matrix<double, 3, 1>, p_ic>,  ///< Relative position of the camera w.r.t. the IMU expressed in the IMU frame.
    msf_core::StateVar_T<Eigen::Matrix<double, 3, 1>, p_ip>  ///< Relative attitude of the position sensor w.r.t. the IMU expressed in the IMU frame.
    > fullState_T;
}
///< The state we want to use in this EKF.
typedef msf_core::GenericState_T<fullState_T, StateDefinition> EKFState;
typedef shared_ptr<EKFState> EKFStatePtr;
typedef shared_ptr<const EKFState> EKFStateConstPtr;
}
#include <msf_updates/static_ordering_assertions.h> //DO NOT REMOVE THIS
#endif /* MSF_STATEDEF_HPP_ */
