#include "terrain.hpp"
#include "SimplexNoise.hpp"

void Terrain::generate(uint nvertices) {
    // already generated
    if (gobj.mesh_count() > 0) {
        gobj.meshes.front().delete_buffers();
        gobj.meshes.clear();
    }

    uint f1 = (uint) glm::sqrt(nvertices);
    while (nvertices % f1 != 0) {
        f1--;
    }
    uint f2 = nvertices / f1;

    uint nrows = 0;
    uint ncols = 0;

    if (gobj.transform.scale.x > gobj.transform.scale.z) {
        nrows = f1 - 1;
        ncols = f2 - 1;
    }
    else {
        nrows = f2 - 1;
        ncols = f1 - 1;
    }

    Transform vert_t;
    vert_t.scale.x = gobj.transform.scale.x / ncols;
    vert_t.scale.z = gobj.transform.scale.z / nrows;

    vert_t.position.x = gobj.transform.scale.x + gobj.transform.position.x;
    vert_t.position.x += vert_t.scale.x / 2;

    vert_t.position.z = gobj.transform.scale.x + gobj.transform.position.z;
    vert_t.position.z -= vert_t.scale.z / 2;

    float originalx = vert_t.position.x;

    constexpr float scale = 0.007f;
    SimplexNoise n(
        scale,
        1,
        2,
        0.5f
    );

    Mesh& mesh = gobj.create_mesh();

    for (size_t i = 0; i < nrows; i++) {
        for (size_t j = 0; j < ncols; j++) {
            /*if (i > 500) {*/
            /*    if (i - 500 > 20) {*/
            /*        vert_t.position.y = glm::simplex(glm::vec2(vert_t.position.x, vert_t.position.z));*/
            /*        // vert_t.position.y = glm::sphericalRand(vert_t.position.x + vert_t.position.z).y;*/
            /*        vert_t.position.y = Utils::random_float(vert_t.position.x, vert_t.position.z);*/
            /*    }*/
            /*}*/
            /*else if (j < 500){*/
            /*    vert_t.position.y = Utils::noise(vert_t.position.x, vert_t.position.z);*/
            /*}*/
            /*else {*/
            /*    vert_t.position.y = glm::perlin(glm::vec2(vert_t.position.x / 2, vert_t.position.z / 2));*/
            /*}*/
            vert_t.position.y = n.fractal(16, j, i) * 2;

            Color color;
            if (vert_t.position.y < color_boundary) {
                color = color_low;
            }
            else {
                color = color_high;
            }
            mesh.vertices.emplace_back(vert_t.position, glm::vec3(0), glm::vec2(0));
            vert_t.position.x += vert_t.scale.x;
        }
        vert_t.position.x = originalx;
        vert_t.position.z -= vert_t.scale.z;
    }

    for (size_t j = 0; j < nrows - 1; j++) {
        for (size_t i = 0; i < ncols - 1; i++) {
            size_t top_left = j * ncols + i;
            size_t top_right = top_left + 1;
            size_t bottom_left = top_left + ncols;
            size_t bottom_right = bottom_left + 1;

            mesh.indices.push_back(top_left);
            mesh.indices.push_back(top_right);
            mesh.indices.push_back(bottom_left);

            mesh.indices.push_back(bottom_left);
            mesh.indices.push_back(top_right);
            mesh.indices.push_back(bottom_right);

            // Calculate normals
            /*glm::vec3 normal = glm::normalize(*/
            /*    glm::cross(vertices[top_right].position - vertices[top_left].position,*/
            /*             vertices[bottom_left].position - vertices[top_left].position));*/
            glm::vec3 normal =
                glm::cross(mesh.vertices[top_right].position - mesh.vertices[top_left].position,
                         mesh.vertices[bottom_left].position - mesh.vertices[top_left].position);
            mesh.vertices[top_left].normal = normal;
            mesh.vertices[top_right].normal = normal;
            mesh.vertices[bottom_left].normal = normal;
            /*vertices[top_left].normal = normal;*/
            /*vertices[top_right].normal = normal;*/
            /*vertices[bottom_left].normal = normal;*/
            mesh.vertices[bottom_right].normal = normal;
        }
    }

    mesh.create_buffers();
}

