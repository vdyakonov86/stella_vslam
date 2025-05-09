#ifndef STELLA_VSLAM_MATCH_FUSE_H
#define STELLA_VSLAM_MATCH_FUSE_H

#include "stella_vslam/type.h"
#include "stella_vslam/match/base.h"

#include <memory>

namespace stella_vslam {

namespace data {
class keyframe;
class landmark;
class map_database;
} // namespace data

namespace match {

class fuse final {
public:
    explicit fuse(float lowe_ratio, const std::string dist_metric)
        : lowe_ratio_(lowe_ratio), dist_metric_(dist_metric) {
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

    virtual ~fuse() = default;

    //! 3次元点(landmarks_to_check)をkeyframeに再投影し，keyframeで観測している3次元点と重複しているものを探す
    template<typename T>
    unsigned int detect_duplication(const std::shared_ptr<data::keyframe>& keyfrm,
                                    const Mat33_t& rot_cw,
                                    const Vec3_t& trans_cw,
                                    const T& landmarks_to_check,
                                    const float margin,
                                    std::unordered_map<std::shared_ptr<data::landmark>, std::shared_ptr<data::landmark>>& duplicated_lms_in_keyfrm,
                                    std::unordered_map<unsigned int, std::shared_ptr<data::landmark>>& new_connections,
                                    bool do_reprojection_matching = false) const;

protected:
    const float lowe_ratio_;
    const std::string dist_metric_;
    float dist_thr_low_;
    float dist_thr_high_;
    float max_dist_;
};

} // namespace match
} // namespace stella_vslam

#endif // STELLA_VSLAM_MATCH_FUSE_H
