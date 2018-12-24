// Copyright 2018 Justin Zhang
#include "api/opk/kp_extractor.hpp"

// DEFINE_string(image_path, "../img/28.jpeg",
//     "Process an image. Read all standard formats (jpg, png, bmp, etc.).");

api::opk::kpExtractor::kpExtractor() {
  op_wrapper_ = std::make_shared<op::Wrapper>
                (op::ThreadManagerMode::Asynchronous);
  op_wrapper_.get()->configure(op::WrapperStructFace{true});
  op_wrapper_.get()->configure(op::WrapperStructHand{true});
  op::log("Starting thread(s)...", op::Priority::High);
  op_wrapper_.get()->start();
}

void api::opk::kpExtractor::display(const std::shared_ptr<std::vector<op::Datum>>& datumsPtr)
{
  // User's displaying/saving/other processing here
      // datum.cvOutputData: rendered frame with pose or heatmaps
      // datum.poseKeypoints: Array<float> with the estimated pose
  if (datumsPtr != nullptr && !datumsPtr->empty()) {
    // Display image
    cv::imshow("User worker GUI", datumsPtr->at(0).cvOutputData);
    cv::waitKey(0);
  } else {
    op::log("Nullptr or empty datumsPtr found.", op::Priority::High);
  }
}

void api::opk::kpExtractor::computeKp(const cv::Mat &img) {
  this->datum_processed_ =  op_wrapper_.get()->emplaceAndPop(img);
}

void api::opk::kpExtractor::computeKp(const stdImage &img) {
  cv::Mat cv_img = api::opk::common::stdToCvimg(img);
  this->datum_processed_ =  op_wrapper_.get()->emplaceAndPop(cv_img);
}

cv::Mat api::opk::kpExtractor::getResultImg() {
  if (this->datum_processed_ != nullptr && !this->datum_processed_->empty()) {
    return this->datum_processed_->at(0).cvOutputData.clone();
  }
}

stdKeypoint api::opk::kpExtractor::getKeypoints() {
  stdKeypoint out_pt;
  if (this->datum_processed_ != nullptr && !this->datum_processed_->empty()) {
    auto pose_kp = this->datum_processed_->at(0).poseKeypoints;
    auto face_kp = this->datum_processed_->at(0).faceKeypoints;
    auto hand_l_kp = this->datum_processed_->at(0).handKeypoints[0];
    auto hand_r_kp = this->datum_processed_->at(0).handKeypoints[1];
    this->appendFront(out_pt, pose_kp);
    this->appendFront(out_pt, face_kp);
    this->appendFront(out_pt, hand_l_kp);
    this->appendFront(out_pt, hand_r_kp);
  }
  return out_pt;
}

void api::opk::kpExtractor::appendFront(stdKeypoint &std_kp, const op::Array<float> &op_pk) {
  int num_ppl = op_pk.getSize()[0];
  int num = op_pk.getSize()[1];
  int dim = op_pk.getSize()[2];
  std::cout << num_ppl << ", ";
  std::cout << num << ", ";
  std::cout << dim << std::endl;
  static std::pair<int, int> xy;
  for (int i = 0; i < num; i++) {
    if (op_pk.at(dim*i+dim-1) != 0) {
      xy.first = op_pk.at(dim*i);
      xy.second = op_pk.at(dim*i+1);
      // std::cout << xy.first << ", " << xy.second << std::endl;
      if (std_kp.size() == 0) {
        std::vector<std::pair<int, int>> front;
        front.push_back(xy);
        std_kp.push_back(front);
      } else {
        std_kp[0].push_back(xy);
      }
    }
  }
}
