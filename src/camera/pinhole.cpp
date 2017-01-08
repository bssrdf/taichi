#include <taichi/visual/camera.h>

TC_NAMESPACE_BEGIN

class PinholeCamera : public Camera {
public:
    PinholeCamera() {}

    virtual void initialize(const Config &config) override {
        fov = config.get_real("fov") / 180.0f * pi;
        this->origin = config.get_vec3("origin");
        this->look_at = config.get_vec3("look_at");
        this->up = config.get_vec3("up");
        this->width = config.get_int("width");
        this->height = config.get_int("height");
        set_dir_and_right();
        tan_half_fov = tan(fov / 2);
        this->aspect_ratio = (real)width / height;
        this->transform = Matrix4(1.0f);
    }

    real get_pixel_scaling() override {
        return sqr(tan_half_fov) * aspect_ratio;
    }

    virtual Ray sample(Vector2 offset, Vector2 size, StateSequence &rand) override {
        Vector2 rand_offset = random_offset(offset, size, rand(), rand());
        Vector3 local_dir = normalize(
            dir + rand_offset.x * tan_half_fov * right * aspect_ratio + rand_offset.y * tan_half_fov * up);
        Vector3 world_orig = multiply_matrix4(transform, origin, 1);
        Vector3 world_dir = normalized(multiply_matrix4(transform, local_dir, 0)); //TODO: why normalize here???
        return Ray(world_orig, world_dir, 0);
    }

    void get_pixel_coordinate(Vector3 ray_dir, real &u, real &v) override {
        auto inv_transform = glm::inverse(transform);
        auto local_ray_dir = multiply_matrix4(inv_transform, ray_dir, 0);
        u = dot(local_ray_dir, right) / dot(local_ray_dir, dir) / tan_half_fov / aspect_ratio + 0.5f;
        v = dot(local_ray_dir, up) / dot(local_ray_dir, dir) / tan_half_fov + 0.5f;
    }

private:
    real fov;
    real tan_half_fov;
    real aspect_ratio;
};

TC_IMPLEMENTATION(Camera, PinholeCamera, "pinhole");

TC_NAMESPACE_END

