// Copyright 2018 Justin Zhang
#ifndef INCLUDE_API_OPK_KP_EXTRACTOR_HPP_
#define INCLUDE_API_OPK_KP_EXTRACTOR_HPP_
#endif  // INCLUDE_API_OPK_KP_EXTRACTOR_HPP_

#include <iostream>
#include <vector>
// Command-line user intraface
#define OPENPOSE_FLAGS_DISABLE_POSE
// #include <openpose/flags.hpp>
// OpenPose dependencies
#include <openpose/headers.hpp>

namespace api {
namespace opk {
class kpExtractor {
 public:
  std::shared_ptr<op::Wrapper> op_wrapper_;
  kpExtractor();
  void display(const std::shared_ptr<std::vector<op::Datum>>& datumsPtr);
  void computeKp(const cv::Mat &img);
  cv::Mat getResultImg();
 private:
  std::shared_ptr<std::vector<op::Datum>> datum_processed_;
};
}};
