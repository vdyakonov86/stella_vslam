#ifndef STELLA_VSLAM_MATCH_BASE_H
#define STELLA_VSLAM_MATCH_BASE_H

#include "stella_vslam/type.h"

#include <array>
#include <algorithm>
#include <numeric>

#include <opencv2/core.hpp>

namespace stella_vslam {
namespace match {

static constexpr unsigned int HAMMING_DIST_THR_LOW = 50;
static constexpr unsigned int HAMMING_DIST_THR_HIGH = 100;
static constexpr unsigned int MAX_HAMMING_DIST = 256;

static constexpr float L2_DIST_THR_LOW = 0.6f;    // Нижний порог "хорошего" соответствия
static constexpr float L2_DIST_THR_HIGH = 1.2f;   // Верхний порог "приемлемого" соответствия
static constexpr float MAX_L2_DIST = 2.0f;        // Максимальное расстояние для фильтрации

inline float compute_descriptor_distance_l2(const cv::Mat& desc1, const cv::Mat& desc2) {
    return cv::norm(desc1, desc2, cv::NORM_L2);
}
  
//! ORB特徴量間のハミング距離を計算する
inline unsigned int compute_descriptor_distance_32(const cv::Mat& desc_1, const cv::Mat& desc_2) {
    // http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel

    constexpr uint32_t mask_1 = 0x55555555U;
    constexpr uint32_t mask_2 = 0x33333333U;
    constexpr uint32_t mask_3 = 0x0F0F0F0FU;
    constexpr uint32_t mask_4 = 0x01010101U;

    const auto* pa = desc_1.ptr<uint32_t>();
    const auto* pb = desc_2.ptr<uint32_t>();

    unsigned int dist = 0;

    for (unsigned int i = 0; i < 8; ++i, ++pa, ++pb) {
        auto v = *pa ^ *pb;
        v -= ((v >> 1) & mask_1);
        v = (v & mask_2) + ((v >> 2) & mask_2);
        dist += (((v + (v >> 4)) & mask_3) * mask_4) >> 24;
    }

    return dist;
}

//! ORB特徴量間のハミング距離を計算する
inline unsigned int compute_descriptor_distance_64(const cv::Mat& desc_1, const cv::Mat& desc_2) {
    // https://stackoverflow.com/questions/21826292/t-sql-hamming-distance-function-capable-of-decimal-string-uint64?lq=1

    constexpr uint64_t mask_1 = 0x5555555555555555UL;
    constexpr uint64_t mask_2 = 0x3333333333333333UL;
    constexpr uint64_t mask_3 = 0x0F0F0F0F0F0F0F0FUL;
    constexpr uint64_t mask_4 = 0x0101010101010101UL;

    const auto* pa = desc_1.ptr<uint64_t>();
    const auto* pb = desc_2.ptr<uint64_t>();

    unsigned int dist = 0;

    for (unsigned int i = 0; i < 4; ++i, ++pa, ++pb) {
        auto v = *pa ^ *pb;
        v -= (v >> 1) & mask_1;
        v = (v & mask_2) + ((v >> 2) & mask_2);
        dist += (((v + (v >> 4)) & mask_3) * mask_4) >> 56;
    }

    return dist;
}

inline float compute_descriptor_distance(const cv::Mat& desc1, const cv::Mat& desc2, const std::string dist_metric) {
    if (dist_metric == "hamming")
        return static_cast<float>(compute_descriptor_distance_32(desc1, desc2));
    else if (dist_metric == "L2")
        return compute_descriptor_distance_l2(desc1, desc2);
}

inline bool check_epipolar_constraint(const Vec3_t& bearing_1, const Vec3_t& bearing_2,
                                      const Mat33_t& E_12, float residual_rad_thr,
                                      const float bearing_1_scale_factor) {
    // Normal vector of the epipolar plane on keyframe 1
    const Vec3_t epiplane_in_1 = E_12 * bearing_2;

    // Acquire the angle formed by the normal vector and the bearing
    const auto cos_residual = std::min(1.0, std::max(-1.0, epiplane_in_1.dot(bearing_1) / epiplane_in_1.norm()));
    const auto residual_rad = std::abs(M_PI / 2.0 - std::acos(cos_residual));

    // The larger keypoint scale permits less constraints
    return residual_rad < residual_rad_thr * bearing_1_scale_factor;
}

class base {
public:
    base(const float lowe_ratio, const bool check_orientation, const std::string dist_metric)
        : lowe_ratio_(lowe_ratio), check_orientation_(check_orientation), dist_metric_(dist_metric) {
        if (dist_metric_ == "hamming") {
            dist_thr_low_ = static_cast<float>(HAMMING_DIST_THR_LOW);
            dist_thr_high_ = static_cast<float>(HAMMING_DIST_THR_HIGH);
            max_dist_ = static_cast<float>(MAX_HAMMING_DIST);
        }
        else if (dist_metric_ == "L2") {
            dist_thr_low_ = L2_DIST_THR_LOW;
            dist_thr_high_ = L2_DIST_THR_HIGH;
            max_dist_ = MAX_L2_DIST;
        }
    }

    virtual ~base() = default;

protected:
    const float lowe_ratio_;
    const bool check_orientation_;
    const std::string dist_metric_;
    float dist_thr_low_;
    float dist_thr_high_;
    float max_dist_;
};

} // namespace match
} // namespace stella_vslam

#endif // STELLA_VSLAM_MATCH_BASE_H
