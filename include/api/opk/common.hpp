// Copyright 2018 Justin Zhang
#ifndef INCLUDE_API_OPK_COMMON_HPP_
#define INCLUDE_API_OPK_COMMON_HPP_
#endif  // INCLUDE_API_OPK_COMMON_HPP_

#include <tuple>
#include <vector>
#include <utility>
#include <opencv2/opencv.hpp>

typedef std::tuple<std::pair<int, int>,
        std::vector<std::array<std::uint8_t, 4>>> stdImage;

namespace api {
namespace opk {
namespace common {
  cv::Mat stdToCvimg(const stdImage &std_img);
}}}
