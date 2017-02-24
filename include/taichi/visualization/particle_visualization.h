#pragma once

#include <taichi/common/meta.h>
#include <taichi/visualization/image_buffer.h>
#include <taichi/visual/camera.h>
#include <taichi/visual/texture.h>

TC_NAMESPACE_BEGIN

struct RenderParticle {
    Vector3 position;
    Vector4 color;
    RenderParticle() {}
    RenderParticle(const Vector3 &position, const Vector4 &color) : position(position), color(color) {}
    RenderParticle(const Vector3 &position, const Vector3 &color) : position(position), color(color.x, color.y, color.z, 1.0f) {}
    bool operator == (const RenderParticle &p) const {
        // For boost::python vector_indexing_suite
        return false;
    }
};

class ParticleRenderer {
protected:
    std::shared_ptr<Camera> camera;

public:
    void set_camera(std::shared_ptr<Camera> camera) {
        this->camera = camera;
    }
    virtual void initialize(const Config &config) {};
    virtual void render(Array2D<Vector3> &buffer, const std::vector<RenderParticle> &particles) const {}
};

std::shared_ptr<Texture> rasterize_render_particles(const Config &config, const std::vector<RenderParticle> &particles);

TC_INTERFACE(ParticleRenderer)

TC_NAMESPACE_END

