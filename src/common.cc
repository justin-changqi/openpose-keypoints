// Copyright 2018 Justin Zhang
#include "api/opk/common.hpp"

cv::Mat api::opk::common::stdToCvimg(const stdImage &std_img) {
  std::pair<int, int> size = std::get<0>(std_img);
  std::vector<std::array<std::uint8_t, 4>> data = std::get<1>(std_img);
  int xn = size.first;
  int yn = size.second;
  cv::Mat out_img(xn, yn, CV_8UC4);
  for (int i = 0; i < xn; i++) {
    for (int j = 0; j < yn; j++) {
      enum { cv_blue = 0, cv_green, cv_red, cv_alpha};
      enum { red = 0, green, blue, alpha};
      int ai = xn*j+i;
      out_img.at<cv::Vec3b>(j, i)[cv_red] = data[ai][red];
      out_img.at<cv::Vec3b>(j, i)[cv_green] = data[ai][green];
      out_img.at<cv::Vec3b>(j, i)[cv_blue] = data[ai][blue];
      out_img.at<cv::Vec3b>(j, i)[cv_alpha] = data[ai][cv_alpha];
    }
  }
  return out_img;
}
