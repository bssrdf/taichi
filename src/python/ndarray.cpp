#include <taichi/python/export.h>

#include <taichi/math/array_2d.h>
#include <taichi/levelset/levelset2d.h>
#include <taichi/visualization/image_buffer.h>

TC_NAMESPACE_BEGIN

template<typename T>
void array2d_to_ndarray(T *arr, long long output) // 'output' is actually a pointer...
{
    int width = arr->get_width(), height = arr->get_height();
    for (auto &ind : arr->get_region()) {
        reinterpret_cast<float *>(output)[ind.i + ind.j * width] = (*arr)[ind];
    }
}

template<typename T, int channels>
void image_buffer_to_ndarray(T *arr, long long output) // 'output' is actually a pointer...
{
    int width = arr->get_width(), height = arr->get_height();
    for (auto &ind : arr->get_region()) {
        for (int i = 0; i < channels; i++) {
            reinterpret_cast<float *>(output)[ind.i * channels + ind.j * width * channels + i] = (*arr)[ind][i];
        }
    }
}

template void array2d_to_ndarray(LevelSet2D *arr, long long);
template void array2d_to_ndarray(Array2D<float> *arr, long long);

template void image_buffer_to_ndarray<ImageBuffer<Vector3>, 3>(ImageBuffer<Vector3> *arr, long long);
template void image_buffer_to_ndarray<ImageBuffer<Vector4>, 4>(ImageBuffer<Vector4> *arr, long long);

TC_NAMESPACE_END

