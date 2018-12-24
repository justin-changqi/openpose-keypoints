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
#include "api/opk/common.hpp"

typedef std::vector<std::vector<std::pair<int, int>>> stdKeypoint;

namespace api {
namespace opk {
class kpExtractor {
 public:
  std::shared_ptr<op::Wrapper> op_wrapper_;
  kpExtractor();
  void display(const std::shared_ptr<std::vector<op::Datum>>& datumsPtr);
  void computeKp(const cv::Mat &img);
  void computeKp(const stdImage &img);
  cv::Mat getResultImg();
  stdKeypoint getKeypoints(double th);
 private:
  std::shared_ptr<std::vector<op::Datum>> datum_processed_;
  void appendFront(stdKeypoint &std_kp, const op::Array<float> &op_pk, double th);
};
}};
