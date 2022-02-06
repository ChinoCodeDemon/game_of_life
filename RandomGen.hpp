#include<random>

namespace ldc{
    class RandomGen {
    public:
        static float generate(float min, float max){
            static std::mt19937 e(std::random_device{}());
            std::uniform_real_distribution<float> uniform_dist(min, max);
            return uniform_dist(e);
        }

        static double generate64(double min, double max){
            static std::mt19937_64 e(std::random_device{}());
            std::uniform_real_distribution<double> uniform_dist(min, max);
            return uniform_dist(e);
        }
    };
}