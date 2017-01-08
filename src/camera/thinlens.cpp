#include <taichi/visual/camera.h>

TC_NAMESPACE_BEGIN

class ThinLensCamera : public Camera {
public:
    ThinLensCamera() {}

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
        if (config.has_key("focus"))
            this->focus = config.get_vec3("focus");
        else
            this->focus = look_at;
        this->aperture = config.get_real("aperture");
    }

    real get_pixel_scaling() override {
        return sqr(tan_half_fov) * aspect_ratio;
    }

    Vector2 sample_lens(const Vector2 &uv) {
        real r = std::sqrt(uv[0]);
        real theta = pi * 2 * uv[1];
        return Vector2(cos(theta), sin(theta)) * r;
    }

    virtual Ray sample(Vector2 offset, Vector2 size, StateSequence &rand) override {
        Vector2 rand_offset = random_offset(offset, size, rand(), rand());
        Vector3 local_dir = normalize(
            dir + rand_offset.x * tan_half_fov * right * aspect_ratio + rand_offset.y * tan_half_fov * up);
        Vector3 world_dir = normalized(multiply_matrix4(transform, local_dir, 0)); //TODO: why normalize here???
        Vector3 focus_point = get_origin() + world_dir *
            (dot(focus - get_origin(), get_dir()) / dot(get_dir(), world_dir));
        Vector2 uv = sample_lens(rand.next2());
        Vector3 orig = get_origin() + aperture * (uv[0] * right + uv[1] * up);
        return Ray(orig, normalized(focus_point - orig));
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
    real aperture;
    Vector3 focus;
};

TC_IMPLEMENTATION(Camera, ThinLensCamera, "thinlens");

TC_NAMESPACE_END

