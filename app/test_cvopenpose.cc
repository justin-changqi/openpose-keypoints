// Copyright 2018 Justin Zhang
#include <iostream>
#include <chrono>
#include <thread>
#include "api/opk/kp_extractor.hpp"

constexpr char const* FOLDER_IMG = "../img/test_src/";
constexpr char const* FOLDER_IMG_OUT = "../img/test_out/";
constexpr char const* FOLDER_IMG_KP = "../img/test_kp/";

void saveImage(cv::Mat &img, const std::string folder, std::string file_name)
{
  std::string save_file = folder + file_name + ".png";
  cv::imwrite(save_file, img);
}

void drawPoints(stdKeypoint pt, cv::Mat &img) {
  for (int i = 0; i < pt.size(); i++) {
    // front:0, back: 1
    for (int j = 0; j < pt[i].size(); j++) {
      cv::Point cvpt =  cv::Point(pt[i][j].first, pt[i][j].second);
      cv::circle(img, cvpt, 2, cv::Scalar(255, 0, 0), 2, 8, 0);
    }
  }
}

int main(int argc, char const *argv[]) {
  // gflags::ParseCommandLineFlags(&argc, &argv, true);
  api::opk::kpExtractor kp;
  // const auto imageToProcess = cv::imread("../img/test_src/29.jpeg");
  // kp.computeKp(imageToProcess);
  // stdKeypoint my_kp = kp.getKeypoints(0.75);
  // cv::Mat draw = imageToProcess.clone();
  // drawPoints(my_kp, draw);
  // cv::imshow("User worker GUI", draw);
  // cv::waitKey(0);
  for (int i = 1; i <= 29; i++) {
    std::cout << "Loading image " << FOLDER_IMG + std::to_string(i) << ".jpeg ..." << std::endl;
    const auto imageToProcess = cv::imread(FOLDER_IMG + std::to_string(i)+".jpeg");
    kp.computeKp(imageToProcess);
    cv::Mat result = kp.getResultImg();
    stdKeypoint my_kp = kp.getKeypoints(0.1);
    cv::Mat draw = imageToProcess.clone();
    drawPoints(my_kp, draw);
    saveImage(result, FOLDER_IMG_OUT,  std::to_string(i));
    saveImage(draw, FOLDER_IMG_KP,  std::to_string(i));
  }
  return 0;
}


