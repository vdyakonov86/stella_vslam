#ifndef STELLA_VSLAM_MATCH_SUPERGLUE_H
#define STELLA_VSLAM_MATCH_SUPERGLUE_H

#include "stella_vslam/type.h"
#include "stella_vslam/match/base.h"
#include "ort-superglue/SuperGlue.hpp"

#include <set>
#include <memory>

namespace stella_vslam {

namespace data {
class frame;
struct frame_observation;
class keyframe;
class landmark;
} // namespace data

namespace match {

class superglue final {
public:
    explicit superglue() {}

    virtual ~superglue() final = default;

    unsigned int match_current_and_last_frames(data::frame& curr_frm, const data::frame& last_frm) const;
};

} // namespace match
} // namespace stella_vslam

#endif // STELLA_VSLAM_MATCH_SUPERGLUE_H
