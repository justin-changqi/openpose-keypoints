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

cv::Mat api::opk::kpExtractor::getResultImg() {
  if (this->datum_processed_ != nullptr && !this->datum_processed_->empty()) {
    return this->datum_processed_->at(0).cvOutputData.clone();
  }
}

