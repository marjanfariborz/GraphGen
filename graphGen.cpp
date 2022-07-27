#include <iostream>
#include <random>

int main(int argc, char** argv) {
    const float A = 0.57f, B = 0.19f, C = 0.19f;
    static const int64_t kRandSeed = 27491095;
    int scale_ = atoi(argv[1]);
    int64_t num_nodes_ = 1l << scale_;
    int64_t num_edges_ = num_nodes_ * atoi(argv[2]);
    static const int64_t block_size = 1<<18;
    #pragma omp parallel
    {
      std::mt19937 rng;
      std::uniform_real_distribution<float> udist(0, 1.0f);
      #pragma omp for
      for (int64_t block=0; block < num_edges_; block+=block_size) {
        rng.seed(kRandSeed + block/block_size);
        for (int64_t e=block; e < std::min(block+block_size, num_edges_); e++) {
          int src = 0, dst = 0;
          for (int depth=0; depth < scale_; depth++) {
            float rand_point = udist(rng);
            src = src << 1;
            dst = dst << 1;
            if (rand_point < A+B) {
              if (rand_point > A)
                dst++;
            } else {
              src++;
              if (rand_point > A+B+C)
                dst++;
            }

          }
          std::cout<< src<<"\t"<<dst<<"\n";
        }
      }
    }
    return 0;
  }