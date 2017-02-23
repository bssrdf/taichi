#pragma once

#include <memory>
#include <taichi/math/linalg.h>
#include <taichi/math/array_2d.h>
#include <taichi/system/threading.h>
#include <stb_image.h>
#include <stb_image_write.h>

TC_NAMESPACE_BEGIN

template<typename T>
class ImageAccumulator {
public:
    std::vector<Spinlock> locks;
    ImageAccumulator() {}

    ImageAccumulator(int width, int height) : width(width), height(height),
        buffer(width, height), counter(width, height)
    {
        for (int i = 0; i < width * height; i++) {
            locks.push_back(Spinlock());
        }
    }

    Array2D<T> get_averaged(T default_value = T(0)) {
        Array2D<T> result(width, height);
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                if (counter[i][j] > 0) {
                    real inv = (real)1 / counter[i][j];
                    result[i][j] = inv * buffer[i][j];
                }
                else {
                    result[i][j] = default_value;
                }
            }
        }
        return result;
    }

    void accumulate(int x, int y, T val) {
        int lock_id = x * height + y;
        locks[lock_id].lock();
        counter[x][y] ++;
        buffer[x][y] += val;
        locks[lock_id].unlock();
    }

    void accumulate(ImageAccumulator<T> &other) {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                counter[i][j] += other.counter[i][j];
                buffer[i][j] += other.buffer[i][j];
            }
        }
    }


    int get_width() const {
        return width;
    }

    int get_height() const {
        return height;
    }

private:
    Array2D<T> buffer;
    Array2D<int> counter;
    int width, height;
};

TC_NAMESPACE_END

