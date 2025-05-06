#ifndef STELLA_VSLAM_FEATURE_SUPERPOINT_EXTRACTOR_H
#define STELLA_VSLAM_FEATURE_SUPERPOINT_EXTRACTOR_H

#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>

#include <ort_utility/ort_utility.hpp>
#include <ort-superpoint/SuperPoint.hpp>


namespace stella_vslam {
namespace feature {

using KeyPointAndDesc = std::pair<std::vector<cv::KeyPoint>, cv::Mat>;

class superpoint_extractor {
public:
    superpoint_extractor() = delete;

    //! Constructor
    superpoint_extractor(Ort::SuperPoint* superPoint,
                  const unsigned int min_area,
                  const std::vector<std::vector<float>>& mask_rects = {});

    //! Destructor
    virtual ~superpoint_extractor() = default;

    //! Extract keypoints and each descriptor of them
    void extract(const cv::_InputArray& in_image, const cv::_InputArray& in_image_mask,
                 std::vector<cv::KeyPoint>& keypts, const cv::_OutputArray& out_descriptors);

    //! A vector of keypoint area represents mask area
    //! Each areas are denoted as form of [x_min / cols, x_max / cols, y_min / rows, y_max / rows]
    std::vector<std::vector<float>> mask_rects_;

private:
    KeyPointAndDesc process_frame(const Ort::SuperPoint& osh, const cv::Mat& inputImg, float* dst, int borderRemove = 4, float confidenceThresh = 0.015, bool alignCorners = true, int distThresh = 2);
    void filter_keypoints(std::vector<cv::KeyPoint>& keypts, const cv::Mat& mask);

    // TODO: move to base_extractor class
    //! Create a mask matrix that constructed by rectangles
    void create_rectangle_mask(const unsigned int cols, const unsigned int rows);

    // TODO: move to base_extractor class
    //! Pick computed keypoints on the image uniformly 
    std::vector<cv::KeyPoint> distribute_keypoints(const std::vector<cv::KeyPoint>& keypts_to_distribute,
                                                   const int min_x, const int max_x, const int min_y, const int max_y,
                                                   const float scale_factor) const;

    Ort::SuperPoint* superPoint_;
    //! Area of node occupied by one feature point
    unsigned int min_area_sqrt_;

    // TODO: move to base_extractor class
    //! rectangle mask has been already initialized or not
    bool mask_is_initialized_ = false;
    cv::Mat rect_mask_;
};

} // namespace feature
} // namespace stella_vslam

#endif // STELLA_VSLAM_FEATURE_SUPERPOINT_EXTRACTOR_H
