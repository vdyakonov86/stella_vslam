#include "stella_vslam/feature/superpoint_extractor.h"
#include "stella_vslam/type.h"

#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

#include <iostream>

#include <spdlog/spdlog.h>

namespace stella_vslam {
namespace feature {

superpoint_extractor::superpoint_extractor(Ort::SuperPoint* superPoint, 
                                           const unsigned int min_area,
                                           const std::vector<std::vector<float>>& mask_rects): 
                                           superPoint_(superPoint),
                                           mask_rects_(mask_rects), 
                                           min_area_sqrt_(std::sqrt(min_area)) {}

void superpoint_extractor::extract(const cv::_InputArray& in_image, const cv::_InputArray& in_image_mask,
                            std::vector<cv::KeyPoint>& keypts, const cv::_OutputArray& out_descriptors) {
    if (in_image.empty()) {
        return;
    }

    // get cv::Mat of image
    const auto image = in_image.getMat();
    assert(image.type() == CV_8UC1);

    unsigned int desc_dim = 256;

    KeyPointAndDesc result = (*superPoint_).inference(*superPoint_, image);

    keypts = result.first;
    unsigned int keyptsN = keypts.size();

    if (keyptsN == 0)
        out_descriptors.release();
    else {
        out_descriptors.create(keypts.size(), desc_dim, CV_32FC1);
        cv::Mat descriptors = out_descriptors.getMat();

        // cv::Mat trimmed_descriptors(result.second.rows, 32, result.second.type());
        // for (int i = 0; i < result.second.rows; ++i) {
        //     cv::Mat row = result.second.row(i);
        //     cv::Mat trimmed_row = row.colRange(0, 32);
        //     trimmed_row.copyTo(trimmed_descriptors.row(i));
        // }
        descriptors = result.second;
    }
    
    // mask initialization
    // if (!mask_is_initialized_ && !mask_rects_.empty()) {
    //     create_rectangle_mask(image.cols, image.rows);
    //     mask_is_initialized_ = true;
    // }

    // select mask to use
    // if (!in_image_mask.empty()) {
    //     // Use image_mask if it is available
    //     const auto image_mask = in_image_mask.getMat();
    //     assert(image_mask.type() == CV_8UC1);
    //     filter_keypoints(keypts, image_mask);
    // }
    // else if (!rect_mask_.empty()) {
    //     // Use rectangle mask if it is available and image_mask is not used
    //     assert(rect_mask_.type() == CV_8UC1);
    //     filter_keypoints(keypts, rect_mask_);
    // }
}

void superpoint_extractor::create_rectangle_mask(const unsigned int cols, const unsigned int rows) {
    if (rect_mask_.empty()) {
        rect_mask_ = cv::Mat(rows, cols, CV_8UC1, cv::Scalar(255));
    }
    // draw masks
    for (const auto& mask_rect : mask_rects_) {
        // draw black rectangle
        const unsigned int x_min = std::round(cols * mask_rect.at(0));
        const unsigned int x_max = std::round(cols * mask_rect.at(1));
        const unsigned int y_min = std::round(rows * mask_rect.at(2));
        const unsigned int y_max = std::round(rows * mask_rect.at(3));
        cv::rectangle(rect_mask_, cv::Point2i(x_min, y_min), cv::Point2i(x_max, y_max), cv::Scalar(0), -1, cv::LINE_AA);
    }
}

std::vector<cv::KeyPoint> superpoint_extractor::distribute_keypoints(const std::vector<cv::KeyPoint>& keypts_to_distribute,
                                                              const int min_x, const int max_x, const int min_y, const int max_y,
                                                              const float scale_factor) const {
    double scaled_min_area_sqrt = min_area_sqrt_ / scale_factor;
    unsigned int num_x_grid = std::ceil((max_x - min_x) / scaled_min_area_sqrt);
    unsigned int num_y_grid = std::ceil((max_y - min_y) / scaled_min_area_sqrt);
    double delta_x = static_cast<double>(max_x - min_x) / num_x_grid;
    double delta_y = static_cast<double>(max_y - min_y) / num_y_grid;
    std::vector<cv::KeyPoint> result_keypts;
    result_keypts.reserve(num_x_grid * num_y_grid);
    std::unordered_map<unsigned int, std::pair<cv::KeyPoint, double>> keypt_response_map;
    std::vector<std::vector<cv::KeyPoint>> keypts_on_grid(num_x_grid * num_y_grid);

    for (const auto& keypt : keypts_to_distribute) {
        const unsigned int ix = keypt.pt.x / delta_x;
        const unsigned int iy = keypt.pt.y / delta_y;
        const unsigned int idx = ix + iy * num_x_grid;
        keypts_on_grid[idx].push_back(keypt);
    }

    for (unsigned int i = 0; i < keypts_on_grid.size(); ++i) {
        auto& keypts = keypts_on_grid[i];
        if (keypts.empty()) {
            continue;
        }
        auto& selected_keypt = keypts.at(0);
        double max_response = selected_keypt.response;

        for (unsigned int k = 1; k < keypts.size(); ++k) {
            const auto& keypt = keypts[k];
            if (keypt.response > max_response) {
                selected_keypt = keypt;
                max_response = keypt.response;
            }
        }

        result_keypts.push_back(selected_keypt);
    }

    return result_keypts;
}

void superpoint_extractor::filter_keypoints(std::vector<cv::KeyPoint>& keypts, const cv::Mat& mask) {}

} // namespace feature
} // namespace stella_vslam
