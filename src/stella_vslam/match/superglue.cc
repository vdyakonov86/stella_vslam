#include "stella_vslam/camera/base.h"
#include "stella_vslam/data/common.h"
#include "stella_vslam/data/frame.h"
#include "stella_vslam/data/frame_observation.h"
#include "stella_vslam/data/keyframe.h"
#include "stella_vslam/data/landmark.h"
#include "stella_vslam/match/superglue.h"
#include "stella_vslam/util/angle.h"
#include <spdlog/spdlog.h>


namespace stella_vslam {
namespace match {

unsigned int superglue::match_current_and_last_frames(data::frame& curr_frm, const data::frame& last_frm) const {
    unsigned int num_matches = 0;

    std::pair<std::vector<cv::KeyPoint>, cv::Mat> currObservation;
    std::pair<std::vector<cv::KeyPoint>, cv::Mat> lastObservation;

    currObservation.first = curr_frm.frm_obs_.undist_keypts_;
    currObservation.second = curr_frm.frm_obs_.descriptors_;

    lastObservation.first = last_frm.frm_obs_.undist_keypts_;
    lastObservation.second = last_frm.frm_obs_.descriptors_;

    Ort::SuperGlue superGlueOsh("/stella_vslam/src/onnx_runtime_cpp/weights/super_glue.onnx", 0);
    std::vector<cv::DMatch> matches = superGlueOsh.inference(superGlueOsh, lastObservation, currObservation, curr_frm.img_size_);
    
    for (auto& match_ : matches) {
      auto idx_last = match_.queryIdx;
      auto curr_idx = match_.trainIdx;

      const auto& lm = last_frm.get_landmark(idx_last);

      if (!lm) {
          continue;
          spdlog::warn("NOT LANDMARK, idx {}, frame id {}", idx_last, last_frm.id_);
      }

      if (lm->will_be_erased()) {
          spdlog::warn("will_be_erased, idx {}, frame id {}", idx_last, last_frm.id_);
          continue;
      }

      const auto& curr_lm = curr_frm.get_landmark(curr_idx);
      if (curr_lm && curr_lm->has_observation()) {
          continue;
      }

      curr_frm.add_landmark(lm, curr_idx);
      ++num_matches;
    }

    // for (unsigned int idx_last = 0; idx_last < last_frm.frm_obs_.undist_keypts_.size(); ++idx_last) {
    //     const auto& lm = last_frm.get_landmark(idx_last);
    //     if (!lm) {
    //         continue;
    //         spdlog::warn("NOT LANDMARK, idx {}, frame id {}", idx_last, last_frm.id_);
    //     }
    //     // spdlog::info("EXIST LANDMARK, idx {}, frame id {}", idx_last, last_frm.id_);

    //     if (lm->will_be_erased()) {
    //         spdlog::warn("will_be_erased, idx {}, frame id {}", idx_last, last_frm.id_);
    //         continue;
    //     }

    //     // 3D point coordinates with the global reference
    //     const Vec3_t pos_w = lm->get_pos_in_world();

    //     // Reproject and compute visibility
    //     Vec2_t reproj;
    //     float x_right;
    //     const bool in_image = curr_frm.camera_->reproject_to_image(rot_cw, trans_cw, pos_w, reproj, x_right);

    //     // Ignore if it is reprojected outside the image
    //     if (!in_image) {
    //         spdlog::warn("NOT IN IMAGE, idx {}, frame id {}", idx_last, last_frm.id_);
    //         continue;
    //     }

    //     // Acquire keypoints in the cell where the reprojected 3D points exist
    //     const unsigned int last_scale_level = last_frm.frm_obs_.undist_keypts_.at(idx_last).octave;
    //     int min_level;
    //     int max_level;
    //     if (assume_forward) {
    //         min_level = last_scale_level;
    //         max_level = std::min(last_frm.orb_params_->num_levels_ - 1, last_scale_level + 1);
    //     }
    //     else if (assume_backward) {
    //         min_level = std::max(0, static_cast<int>(last_scale_level) - 1);
    //         max_level = last_scale_level;
    //     }
    //     else {
    //         min_level = std::max(0, static_cast<int>(last_scale_level) - 1);
    //         max_level = std::min(last_frm.orb_params_->num_levels_ - 1, last_scale_level + 1);
    //     }
    //     auto indices = curr_frm.get_keypoints_in_cell(reproj(0), reproj(1),
    //                                                   margin * curr_frm.orb_params_->scale_factors_.at(last_scale_level),
    //                                                   min_level, max_level);
        
    //     if (indices.empty()) {
    //         spdlog::warn("INDICES EMPTY, idx {}, frame id {}", idx_last, last_frm.id_);
    //         continue;
    //     }

    //     const auto lm_desc = lm->get_descriptor();

    //     auto best_dist = max_dist_;
    //     int best_idx = -1;

    //     for (const auto curr_idx : indices) {
    //         const auto& curr_lm = curr_frm.get_landmark(curr_idx);
    //         if (curr_lm && curr_lm->has_observation()) {
    //             // spdlog::warn("NO OBSERVATION, idx: {}", curr_idx);
    //             continue;
    //         }

    //         if (!curr_frm.frm_obs_.stereo_x_right_.empty() && curr_frm.frm_obs_.stereo_x_right_.at(curr_idx) > 0) {
    //             const float reproj_error = std::fabs(x_right - curr_frm.frm_obs_.stereo_x_right_.at(curr_idx));
    //             if (margin * curr_frm.orb_params_->scale_factors_.at(last_scale_level) < reproj_error) {
    //                 // spdlog::warn("reproj_error: {}", reproj_error);
    //                 continue;
    //             }
    //         }

    //         if (check_orientation_ && std::abs(util::angle::diff(last_frm.frm_obs_.undist_keypts_.at(idx_last).angle, curr_frm.frm_obs_.undist_keypts_.at(curr_idx).angle)) > 30.0) {
    //              spdlog::warn("check_orientation_");
    //             continue;
    //         }

    //         const auto& desc = curr_frm.frm_obs_.descriptors_.row(curr_idx);

    //         const float dist = compute_descriptor_distance(lm_desc, desc, dist_metric_);

    //         if (dist < best_dist) {
    //             best_dist = dist;
    //             best_idx = curr_idx;
    //         }
    //     }

    //     if (dist_thr_high_ < best_dist) {
    //         continue;
    //     }

    //     // The matching is valid
    //     curr_frm.add_landmark(lm, best_idx);
    //     ++num_matches;
    // }

    return num_matches;
}

} // namespace match
} // namespace stella_vslam
