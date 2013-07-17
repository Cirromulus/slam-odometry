#ifndef ODOMETRY_FOOT_CONTACT_HPP__ 
#define ODOMETRY_FOOT_CONTACT_HPP__

#include <odometry/ContactState.hpp>
#include <odometry/Gaussian.hpp>
#include <odometry/Configuration.hpp>
#include <base/odometry.h>

namespace odometry
{
typedef Eigen::Matrix<double,6,6> Matrix6d;
typedef Eigen::Matrix<double,6,1> Vector6d;

class FootContact : 
    public base::odometry::Gaussian3D,
    public base::odometry::Sampling3D,
    public base::odometry::Sampling2D
{
public:
    enum contact_state
    {
	NO_CONTACT = 0,
	TOUCHDOWN = 1,
	CONTACT = 2,
	LIFTOFF = 3
    };

    static const float contact_threshold = 0.5;

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    FootContact(const Configuration& config);
    /**
     * calls updateState, updateMean and updateCovariance methods
     */
    void update(const odometry::BodyContactState& state, const Eigen::Quaterniond& orientation);
    /**
     * update the the bodystate and calculate the contact_states
     */
    void updateState( const odometry::BodyContactState& state, const Eigen::Quaterniond& orientation );
    /**
     * calculate the mean based on the current state and the provided weights
     */
    void updateMean( const std::vector<float>& weights = std::vector<float>() );
    /**
     * update the covariance
     */
    void updateCovariance();

    base::Pose getPoseDelta();
    Eigen::Matrix3d getPositionError();
    Eigen::Matrix3d getOrientationError();
    Matrix6d getPoseError();

public:
    base::Pose getPoseDeltaSample();
    base::Pose2D getPoseDeltaSample2D();

    Eigen::Quaterniond orientation, prevOrientation;
    base::odometry::State<odometry::BodyContactState> state;

    std::vector<FootContact::contact_state>& getContactStates();

private:
    /** Odometry configuration */
    Configuration config;

    GaussianSamplingPose3D sampling;

    std::vector<contact_state> contact_states;

    Eigen::Quaterniond delta_rotq;
    Eigen::Vector3d mean;

    bool zeroVelocity;
};

}

#endif
