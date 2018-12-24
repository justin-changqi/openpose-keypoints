// Copyright 2018 Justin Zhang
#include <iostream>
#include <chrono>
#include <thread>
#include "api/opk/kp_extractor.hpp"

constexpr char const* FOLDER_GT_IMG = "../img/test_gt/gt";
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
      if (i == 0) {
        cv::circle(img, cvpt, 2, cv::Scalar(255, 0, 0), 2, 8, 0);
      } else {
        cv::circle(img, cvpt, 2, cv::Scalar(0, 255, 0), 2, 8, 0);
      }
    }
  }
}

void savePoints(stdKeypoint pt, std::string file) {
  std::ofstream myfile(file);
  myfile << "label,x,y" << std::endl;
  for (int i = 0; i < pt.size(); i++) {
    // front:0, back: 1
    for (int j = 0; j < pt[i].size(); j++) {
      if (i == 0) {
        myfile << "0";
      } else {
        myfile << "1";
      }
      myfile << "," << pt[i][j].first << "," << pt[i][j].second << std::endl;
    }
  }
  myfile.close();
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
  for (int i = 1; i <= 23; i++) {
    std::cout << "Loading image " << FOLDER_IMG + std::to_string(i) << ".jpeg ..." << std::endl;
    const auto imageToProcess = cv::imread(FOLDER_IMG + std::to_string(i)+".jpeg");
    const auto gt_img = cv::imread(FOLDER_GT_IMG + std::to_string(i)+".PNG", cv::IMREAD_UNCHANGED);
    kp.computeKp(imageToProcess);
    cv::Mat result = kp.getResultImg();
    stdKeypoint my_kp = kp.getKeypoints(0.1, gt_img);
    cv::Mat draw = imageToProcess.clone();
    drawPoints(my_kp, draw);
    savePoints(my_kp, "../test_data/std_keypoint" + std::to_string(i) + ".csv");
    saveImage(result, FOLDER_IMG_OUT,  std::to_string(i));
    saveImage(draw, FOLDER_IMG_KP,  std::to_string(i));
  }
  return 0;
}


