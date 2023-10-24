#include "opencv2/core/hal/interface.h"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <cstdio>
#include <iostream>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <torch/csrc/autograd/variable.h>
#include <torch/script.h>
#include <torch/torch.h>

void add(std::string m, std::string img, std::string labelfile) {
  std::shared_ptr<torch::jit::script::Module> module =
      std::make_shared<torch::jit::script::Module>(torch::jit::load(m));
  if (module == nullptr) {
    std::cout << "not found this module" << std::endl;
    return;
  }

  std::vector<torch::jit::IValue> inputs;
  inputs.emplace_back(torch::rand({64, 3, 224, 224}));
  auto t = (double)cv::getTickCount();
  module->forward(inputs).toTensor();
  t = (double)cv::getTickCount() - t;
  printf("execution time = %gs\n", t / cv::getTickFrequency());
  inputs.pop_back();

  cv::Mat image;
  image = cv::imread(img);
  cv::cvtColor(image, image, cv::COLOR_BGR2BGRA);
  cv::Mat img_float;
  image.convertTo(img_float, CV_32F, 1.0 / 255);
  cv::resize(img_float, img_float, cv::Size(224, 224));
  auto img_tensor = torch::from_blob(img_float.data, {1, 224, 224, 3}).cpu();
  img_tensor = img_tensor.permute({0, 3, 1, 2});
  img_tensor[0][0] = img_tensor[0][0].sub_(0.485).div_(0.229);
  img_tensor[0][1] = img_tensor[0][1].sub_(0.456).div_(0.224);
  img_tensor[0][2] = img_tensor[0][2].sub_(0.406).div_(0.225);
  auto img_var = torch::autograd::make_variable(img_tensor, false);
  inputs.emplace_back(img_var);
  torch::Tensor out_tensor = module->forward(inputs).toTensor();

  std::ifstream rf(labelfile.c_str());
  std::string line;
  std::vector<std::string> labels;
  while (std::getline(rf, line))
    labels.push_back(line);

  std::tuple<torch::Tensor, torch::Tensor> result = out_tensor.sort(-1, true);
  torch::Tensor top_scores = std::get<0>(result)[0];
  torch::Tensor top_idxs = std::get<1>(result)[0].toType(torch::kInt32);

  auto top_scores_a = top_scores.accessor<float, 1>();
  auto top_idxs_a = top_idxs.accessor<int, 1>();
  for (int i = 0; i < 5; ++i) {
    std::cout << "score: " << top_scores_a[i];
    std::cout << "   label: " << labels[top_idxs_a[i]] << std::endl;
  }
}