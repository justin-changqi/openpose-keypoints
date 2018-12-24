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

stdKeypoint api::opk::kpExtractor::getKeypoints(double th) {
  stdKeypoint out_pt;
  if (this->datum_processed_ != nullptr && !this->datum_processed_->empty()) {
    auto pose_kp = this->datum_processed_->at(0).poseKeypoints;
    auto face_kp = this->datum_processed_->at(0).faceKeypoints;
    auto hand_l_kp = this->datum_processed_->at(0).handKeypoints[0];
    auto hand_r_kp = this->datum_processed_->at(0).handKeypoints[1];
    this->appendFront(out_pt, pose_kp, th);
    this->appendFront(out_pt, face_kp, th);
    this->appendFront(out_pt, hand_l_kp, th);
    this->appendFront(out_pt, hand_r_kp, th);
  }
  return out_pt;
}

stdKeypoint api::opk::kpExtractor::getKeypoints(double th, const cv::Mat gt_img) {
  stdKeypoint out_pt;
  if (this->datum_processed_ != nullptr && !this->datum_processed_->empty()) {
    auto pose_kp = this->datum_processed_->at(0).poseKeypoints;
    auto face_kp = this->datum_processed_->at(0).faceKeypoints;
    auto hand_l_kp = this->datum_processed_->at(0).handKeypoints[0];
    auto hand_r_kp = this->datum_processed_->at(0).handKeypoints[1];
    this->appendFront(out_pt, pose_kp, th);
    this->appendFront(out_pt, face_kp, th);
    this->appendFront(out_pt, hand_l_kp, th);
    this->appendFront(out_pt, hand_r_kp, th);
  }
  // add background points
  std::vector<std::pair<int, int>> back_ground;
  std::pair<int, int> b_pt;
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution_x(0, gt_img.cols);
  std::uniform_int_distribution<int> distribution_y(0, gt_img.rows);
  for (int i = 0; i < 40; i++) {
    b_pt.first = distribution_x(generator);
    b_pt.second = distribution_y(generator);
    if (gt_img.at<cv::Vec4b>(b_pt.second, b_pt.first)[3]== 0) {
      back_ground.push_back(b_pt);
    }
  }
  out_pt.push_back(back_ground);
  return out_pt;
}

void api::opk::kpExtractor::appendFront(stdKeypoint &std_kp, const op::Array<float> &op_pk, double th) {
  int num_ppl = op_pk.getSize()[0];
  int num = op_pk.getSize()[1];
  int dim = op_pk.getSize()[2];
  static std::pair<int, int> xy;
  for (int i = 0; i < num_ppl; i++) {
    for (int j = num*i; j < num*i+num; j++) {
      int index = j*dim;
      if (op_pk.at(index+2) > th) {
        xy.first = op_pk.at(index);
        xy.second = op_pk.at(index+1);
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
}
