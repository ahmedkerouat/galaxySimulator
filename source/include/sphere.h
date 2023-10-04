const float PI = 3.14159265359f;

class Sphere {
public:
    Sphere(float radius, int sectorCount, int stackCount) :
        radius(radius), sectorCount(sectorCount), stackCount(stackCount) {
        generateSphere();
    }

    const std::vector<float>& getVertices() const { return vertices; }
    const std::vector<unsigned int>& getIndices() const { return indices; }

private:
    float radius;
    int sectorCount;
    int stackCount;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    void generateSphere() {
        std::vector<float>().swap(vertices);
        std::vector<unsigned int>().swap(indices);

        float x, y, z, xy;
        float nx, ny, nz, lengthInv = 1.0f / radius;
        float s, t;
        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i) {
            stackAngle = PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
            xy = radius * cosf(stackAngle);      // r * cos(u)
            z = radius * sinf(stackAngle);       // r * sin(u)

            for (int j = 0; j <= sectorCount; ++j) {
                sectorAngle = j * sectorStep; // starting from 0 to 2pi

                x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)

                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                vertices.push_back(nx);
                vertices.push_back(ny);
                vertices.push_back(nz);

                s = (float)j / sectorCount;
                t = (float)i / stackCount;
                vertices.push_back(s);
                vertices.push_back(t);
            }
        }

        // Generate indices
        for (int i = 0; i < stackCount; ++i) {
            int k1 = i * (sectorCount + 1);
            int k2 = k1 + sectorCount + 1;

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                if (i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (i != (stackCount - 1)) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
    }
};