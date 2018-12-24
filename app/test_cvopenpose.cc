// Copyright 2018 Justin Zhang
#include <iostream>
#include <chrono>
#include <thread>
#include "api/opk/kp_extractor.hpp"

constexpr char const* FOLDER_IMG = "../img/test_src/";
constexpr char const* FOLDER_IMG_OUT = "../img/test_out/";

void saveImage(cv::Mat &img, const std::string folder, std::string file_name)
{
  std::string save_file = folder + file_name + ".png";
  cv::imwrite(save_file, img);
}

int main(int argc, char const *argv[]) {
  // gflags::ParseCommandLineFlags(&argc, &argv, true);
  api::opk::kpExtractor kp;
  for (int i = 1; i <= 29; i++) {
    std::cout << "Loading image " << FOLDER_IMG + std::to_string(i) << "..." << std::endl;
    const auto imageToProcess = cv::imread(FOLDER_IMG + std::to_string(i)+".jpeg");
    kp.computeKp(imageToProcess);
    cv::Mat result = kp.getResultImg();
    cv::imshow("User worker GUI", result);
    saveImage(result, FOLDER_IMG_OUT,  std::to_string(i));
    cv::waitKey(100);
  }
  return 0;
}


