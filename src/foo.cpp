#include <torch/torch.h>
#include <iostream>

void add() {
  torch::Tensor tensor = torch::rand({2, 3});
  std::cout << tensor << std::endl;
}