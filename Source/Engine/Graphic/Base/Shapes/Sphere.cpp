#include "Sphere.hpp"

Sphere::Sphere(float radius, int smoothness)
{
    const int subdivision = smoothness;
    const float S_STEP = 186 / 2048.0f;     // horizontal texture step
    const float T_STEP = 322 / 1024.0f;     // vertical texture step
    std::vector<float> tmpVertices(12 * 3);
    {
        const float PI = 3.1415926f;
        const float H_ANGLE = PI / 180 * 72;    // 72 degree = 360 / 5
        const float V_ANGLE = atanf(1.0f / 2);  // elevation = 26.565 degree

        int i1, i2;                             // indices
        float z, xy;                            // coords
        float hAngle1 = -PI / 2 - H_ANGLE / 2;  // start from -126 deg at 2nd row
        float hAngle2 = -PI / 2;                // start from -90 deg at 3rd row

        // the first top vertex (0, 0, r)
        tmpVertices[0] = 0;
        tmpVertices[1] = 0;
        tmpVertices[2] = radius;

        // 10 vertices at 2nd and 3rd rows
        for (int i = 1; i <= 5; ++i)
        {
            i1 = i * 3;         // for 2nd row
            i2 = (i + 5) * 3;   // for 3rd row

            z = radius * sinf(V_ANGLE);
            xy = radius * cosf(V_ANGLE);

            tmpVertices[i1 + 0] = xy * cosf(hAngle1);
            tmpVertices[i2 + 0] = xy * cosf(hAngle2);
            tmpVertices[i1 + 1] = xy * sinf(hAngle1);
            tmpVertices[i2 + 1] = xy * sinf(hAngle2);
            tmpVertices[i1 + 2] = +z;
            tmpVertices[i2 + 2] = -z;

            // next horizontal angles
            hAngle1 += H_ANGLE;
            hAngle2 += H_ANGLE;
        }

        // the last bottom vertex (0, 0, -r)
        i1 = 11 * 3;
        tmpVertices[i1] = 0;
        tmpVertices[i1 + 1] = 0;
        tmpVertices[i1 + 2] = -radius;
    }
    
    float v[3];                             // vertex
    float n[3];                             // normal
    float scale;                            // scale factor for normalization

    // smooth icosahedron has 14 non-shared (0 to 13) and
    // 8 shared vertices (14 to 21) (total 22 vertices)
    //  00  01  02  03  04          //
    //  /\  /\  /\  /\  /\          //
    // /  \/  \/  \/  \/  \         //
    //10--14--15--16--17--11        //
    // \  /\  /\  /\  /\  /\        //
    //  \/  \/  \/  \/  \/  \       //
    //  12--18--19--20--21--13      //
    //   \  /\  /\  /\  /\  /       //
    //    \/  \/  \/  \/  \/        //
    //    05  06  07  08  09        //
    // add 14 non-shared vertices first (index from 0 to 13)
    _addVertex(tmpVertices[0], tmpVertices[1], tmpVertices[2]);      // v0 (top)
    _addNormal(0, 0, 1);
    _addTexCoord(S_STEP, 0);

    _addVertex(tmpVertices[0], tmpVertices[1], tmpVertices[2]);      // v1
    _addNormal(0, 0, 1);
    _addTexCoord(S_STEP * 3, 0);

    _addVertex(tmpVertices[0], tmpVertices[1], tmpVertices[2]);      // v2
    _addNormal(0, 0, 1);
    _addTexCoord(S_STEP * 5, 0);

    _addVertex(tmpVertices[0], tmpVertices[1], tmpVertices[2]);      // v3
    _addNormal(0, 0, 1);
    _addTexCoord(S_STEP * 7, 0);

    _addVertex(tmpVertices[0], tmpVertices[1], tmpVertices[2]);      // v4
    _addNormal(0, 0, 1);
    _addTexCoord(S_STEP * 9, 0);

    _addVertex(tmpVertices[33], tmpVertices[34], tmpVertices[35]);   // v5 (bottom)
    _addNormal(0, 0, -1);
    _addTexCoord(S_STEP * 2, T_STEP * 3);

    _addVertex(tmpVertices[33], tmpVertices[34], tmpVertices[35]);   // v6
    _addNormal(0, 0, -1);
    _addTexCoord(S_STEP * 4, T_STEP * 3);

    _addVertex(tmpVertices[33], tmpVertices[34], tmpVertices[35]);   // v7
    _addNormal(0, 0, -1);
    _addTexCoord(S_STEP * 6, T_STEP * 3);

    _addVertex(tmpVertices[33], tmpVertices[34], tmpVertices[35]);   // v8
    _addNormal(0, 0, -1);
    _addTexCoord(S_STEP * 8, T_STEP * 3);

    _addVertex(tmpVertices[33], tmpVertices[34], tmpVertices[35]);   // v9
    _addNormal(0, 0, -1);
    _addTexCoord(S_STEP * 10, T_STEP * 3);

    v[0] = tmpVertices[3];  v[1] = tmpVertices[4];  v[2] = tmpVertices[5];  // v10 (left)
    _computeVertexNormal(v, n);
    _addVertex(v[0], v[1], v[2]);
    _addNormal(n[0], n[1], n[2]);
    _addTexCoord(0, T_STEP);

    _addVertex(v[0], v[1], v[2]);                                            // v11 (right)
    _addNormal(n[0], n[1], n[2]);
    _addTexCoord(S_STEP * 10, T_STEP);

    v[0] = tmpVertices[18]; v[1] = tmpVertices[19]; v[2] = tmpVertices[20]; // v12 (left)
    _computeVertexNormal(v, n);
    _addVertex(v[0], v[1], v[2]);
    _addNormal(n[0], n[1], n[2]);
    _addTexCoord(S_STEP, T_STEP * 2);

    _addVertex(v[0], v[1], v[2]);                                            // v13 (right)
    _addNormal(n[0], n[1], n[2]);
    _addTexCoord(S_STEP * 11, T_STEP * 2);

    // add 8 shared vertices to array (index from 14 to 21)
    v[0] = tmpVertices[6];  v[1] = tmpVertices[7];  v[2] = tmpVertices[8];  // v14 (shared)
    _computeVertexNormal(v, n);
    _addVertex(v[0], v[1], v[2]);
    _addNormal(n[0], n[1], n[2]);
    _addTexCoord(S_STEP * 2, T_STEP);
    _sharedIndices[std::make_pair(S_STEP * 2, T_STEP)] = (unsigned int)m_uvs.size() / 2 - 1;

    v[0] = tmpVertices[9];  v[1] = tmpVertices[10]; v[2] = tmpVertices[11]; // v15 (shared)
    _computeVertexNormal(v, n);
    _addVertex(v[0], v[1], v[2]);
    _addNormal(n[0], n[1], n[2]);
    _addTexCoord(S_STEP * 4, T_STEP);
    _sharedIndices[std::make_pair(S_STEP * 4, T_STEP)] = (unsigned int)m_uvs.size() / 2 - 1;

    v[0] = tmpVertices[12]; v[1] = tmpVertices[13]; v[2] = tmpVertices[14]; // v16 (shared)
    scale = _computeScaleForLength(v, 1.0f);

    n[0] = v[0] * scale;    n[1] = v[1] * scale;    n[2] = v[2] * scale;
    _addVertex(v[0], v[1], v[2]);
    _addNormal(n[0], n[1], n[2]);
    _addTexCoord(S_STEP * 6, T_STEP);
    _sharedIndices[std::make_pair(S_STEP * 6, T_STEP)] = (unsigned int)m_uvs.size() / 2 - 1;

    v[0] = tmpVertices[15]; v[1] = tmpVertices[16]; v[2] = tmpVertices[17]; // v17 (shared)
    _computeVertexNormal(v, n);
    _addVertex(v[0], v[1], v[2]);
    _addNormal(n[0], n[1], n[2]);
    _addTexCoord(S_STEP * 8, T_STEP);
    _sharedIndices[std::make_pair(S_STEP * 8, T_STEP)] = (unsigned int)m_uvs.size() / 2 - 1;

    v[0] = tmpVertices[21]; v[1] = tmpVertices[22]; v[2] = tmpVertices[23]; // v18 (shared)
    _computeVertexNormal(v, n);
    _addVertex(v[0], v[1], v[2]);
    _addNormal(n[0], n[1], n[2]);
    _addTexCoord(S_STEP * 3, T_STEP * 2);
    _sharedIndices[std::make_pair(S_STEP * 3, T_STEP * 2)] = (unsigned int)m_uvs.size() / 2 - 1;

    v[0] = tmpVertices[24]; v[1] = tmpVertices[25]; v[2] = tmpVertices[26]; // v19 (shared)
    _computeVertexNormal(v, n);
    _addVertex(v[0], v[1], v[2]);
    _addNormal(n[0], n[1], n[2]);
    _addTexCoord(S_STEP * 5, T_STEP * 2);
    _sharedIndices[std::make_pair(S_STEP * 5, T_STEP * 2)] = (unsigned int)m_uvs.size() / 2 - 1;

    v[0] = tmpVertices[27]; v[1] = tmpVertices[28]; v[2] = tmpVertices[29]; // v20 (shared)
    _computeVertexNormal(v, n);
    _addVertex(v[0], v[1], v[2]);
    _addNormal(n[0], n[1], n[2]);
    _addTexCoord(S_STEP * 7, T_STEP * 2);
    _sharedIndices[std::make_pair(S_STEP * 7, T_STEP * 2)] = (unsigned int)m_uvs.size() / 2 - 1;

    v[0] = tmpVertices[30]; v[1] = tmpVertices[31]; v[2] = tmpVertices[32]; // v21 (shared)
    _computeVertexNormal(v, n);
    _addVertex(v[0], v[1], v[2]);
    _addNormal(n[0], n[1], n[2]);
    _addTexCoord(S_STEP * 9, T_STEP * 2);
    _sharedIndices[std::make_pair(S_STEP * 9, T_STEP * 2)] = (unsigned int)m_uvs.size() / 2 - 1;

    // build index list for icosahedron (20 triangles)
    _addIndices(0, 10, 14);      // 1st row (5 tris)
    _addIndices(1, 14, 15);
    _addIndices(2, 15, 16);
    _addIndices(3, 16, 17);
    _addIndices(4, 17, 11);
    _addIndices(10, 12, 14);      // 2nd row (10 tris)
    _addIndices(12, 18, 14);
    _addIndices(14, 18, 15);
    _addIndices(18, 19, 15);
    _addIndices(15, 19, 16);
    _addIndices(19, 20, 16);
    _addIndices(16, 20, 17);
    _addIndices(20, 21, 17);
    _addIndices(17, 21, 11);
    _addIndices(21, 13, 11);
    _addIndices(5, 18, 12);      // 3rd row (5 tris)
    _addIndices(6, 19, 18);
    _addIndices(7, 20, 19);
    _addIndices(8, 21, 20);
    _addIndices(9, 13, 21);

    // subdivide icosahedron
    {
        std::vector<unsigned int> tmpIndices;
        int indexCount;
        unsigned int i1, i2, i3;            // indices from original triangle
        const float* v1, * v2, * v3;          // ptr to original vertices of a triangle
        const float* t1, * t2, * t3;          // ptr to original texcoords of a triangle
        float newV1[3], newV2[3], newV3[3]; // new subdivided vertex positions
        float newN1[3], newN2[3], newN3[3]; // new subdivided normals
        float newT1[2], newT2[2], newT3[2]; // new subdivided texture coords
        unsigned int newI1, newI2, newI3;   // new subdivided indices
        int i, j;

        // iteration for subdivision
        for (i = 1; i <= subdivision; ++i)
        {
            // copy prev indices
            tmpIndices = m_indices;

            // clear prev arrays
            m_indices.clear();

            indexCount = (int)tmpIndices.size();
            for (j = 0; j < indexCount; j += 3)
            {
                // get 3 indices of each triangle
                i1 = tmpIndices[j];
                i2 = tmpIndices[j + 1];
                i3 = tmpIndices[j + 2];

                // get 3 vertex attribs from prev triangle
                v1 = &m_vertices[i1 * 3];
                v2 = &m_vertices[i2 * 3];
                v3 = &m_vertices[i3 * 3];
                t1 = &m_uvs[i1 * 2];
                t2 = &m_uvs[i2 * 2];
                t3 = &m_uvs[i3 * 2];

                // get 3 new vertex attribs by spliting half on each edge
                _computeHalfVertex(v1, v2, radius, newV1);
                _computeHalfVertex(v2, v3, radius, newV2);
                _computeHalfVertex(v1, v3, radius, newV3);

                _computeHalfTexCoord(t1, t2, newT1);
                _computeHalfTexCoord(t2, t3, newT2);
                _computeHalfTexCoord(t1, t3, newT3);

                _computeVertexNormal(newV1, newN1);
                _computeVertexNormal(newV2, newN2);
                _computeVertexNormal(newV3, newN3);

                // add new vertices/normals/texcoords to arrays
                // It will check if it is shared/non-shared and return index
                newI1 = _addSubVertexAttribs(newV1, newN1, newT1);
                newI2 = _addSubVertexAttribs(newV2, newN2, newT2);
                newI3 = _addSubVertexAttribs(newV3, newN3, newT3);

                // add 4 new triangle indices
                _addIndices(i1, newI1, newI3);
                _addIndices(newI1, i2, newI2);
                _addIndices(newI1, newI2, newI3);
                _addIndices(newI3, newI2, i3);
            }
        }
    }

    // Bind
    _bindArray();
}

void Sphere::draw(const Camera& camera, const glm::mat4& model, const std::vector<Light>& lights) {
    for (auto& recipe : m_shaders) {
        // Setup uniforms
        recipe.second->
            use().
            set("Model", model).
            set("View", camera.modelview).
            set("Projection", camera.projection);

        switch (recipe.first)
        {
        case CookType::Solid:
            recipe.second->
                set("CameraPos", camera.position).
                set("LightNumber", (int)lights.size());

            for (int iLight = 0; iLight < (int)lights.size(); iLight++) {
                recipe.second->
                    set("LightPos_" + std::to_string(iLight), lights[iLight].position).
                    set("LightColor_" + std::to_string(iLight), lights[iLight].color);
            }
            break;

        case CookType::Shadow:
            recipe.second->
                set("viewPos", camera.position).
                set("far_plane", camera.far_plane).
                set("lightPos", lights[0].position).
                set("lightColor", 0.3f * lights[0].color).
                set("depthMap", 1);

            break;
        }

        // Draw
        drawElements(*recipe.second);
    }
}

void Sphere::draw(const Camera& camera, const glm::vec3& position, const glm::vec3& orientation, const std::vector<Light>& lights) {
    glm::mat4 model(1.0f);

    model = glm::translate(model, position);
    model = glm::rotate(model, orientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, orientation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    draw(camera, model, lights);
}

void Sphere::drawBatch(size_t amount, const Camera& camera) {
    for (auto& recipe : m_shaders) {
        // Setup uniforms
        recipe.second->
            use().
            set("View", camera.modelview).
            set("Projection", camera.projection).
            set("CameraPos", camera.position);

        // Draw
        bind();
        glDrawElementsInstanced(GL_TRIANGLES, (int)m_indices.size(), GL_UNSIGNED_INT, 0, (GLsizei)amount);
        unbind();
    }
}

void Sphere::drawElements(Shader& sh) {
    if (sh.has("LocalModel"))
        sh.use().set("LocalModel", glm::mat4(1.0f));

    bind();
    glDrawElements(GL_TRIANGLES, (int)m_indices.size(), GL_UNSIGNED_INT, 0);
    unbind();
}


// Private
void Sphere::_addVertex(float x, float y, float z)
{
    m_vertices.push_back(x);
    m_vertices.push_back(y);
    m_vertices.push_back(z);
}


void Sphere::_addNormal(float nx, float ny, float nz)
{
    m_normals.push_back(nx);
    m_normals.push_back(ny);
    m_normals.push_back(nz);
}

void Sphere::_addTexCoord(float s, float t)
{
    m_uvs.push_back(s);
    m_uvs.push_back(t);
}

void Sphere::_addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    m_indices.push_back(i1);
    m_indices.push_back(i2);
    m_indices.push_back(i3);
}


unsigned int Sphere::_addSubVertexAttribs(const float v[3], const float n[3], const float t[2])
{
    unsigned int index;     // return value;

    // check if is shared vertex or not first
    if (_isSharedTexCoord(t))
    {
        // find if it does already exist in sharedIndices map using (s,t) key
        // if not in the list, add the vertex attribs to arrays and return its index
        // if exists, return the current index
        std::pair<float, float> key = std::make_pair(t[0], t[1]);
        const auto iter = _sharedIndices.find(key);
        if (iter == _sharedIndices.cend())
        {
            _addVertex(v[0], v[1], v[2]);
            _addNormal(n[0], n[1], n[2]);
            _addTexCoord(t[0], t[1]);
            index = (unsigned int)m_uvs.size() / 2 - 1;
            _sharedIndices[key] = index;
        }
        else
        {
            index = iter->second;
        }
    }
    // not shared
    else
    {
        _addVertex(v[0], v[1], v[2]);
        _addNormal(n[0], n[1], n[2]);
        _addTexCoord(t[0], t[1]);
        index = (unsigned int)m_uvs.size() / 2 - 1;
    }

    return index;
}


// Stattics private
float Sphere::_computeScaleForLength(const float v[3], float length)
{
    // and normalize the vector then re-scale to new radius
    return length / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void Sphere::_computeVertexNormal(const float v[3], float normal[3])
{
    // normalize
    float scale = _computeScaleForLength(v, 1.0f);
    normal[0] = v[0] * scale;
    normal[1] = v[1] * scale;
    normal[2] = v[2] * scale;
}

void Sphere::_computeHalfVertex(const float v1[3], const float v2[3], float length, float newV[3])
{
    newV[0] = v1[0] + v2[0];
    newV[1] = v1[1] + v2[1];
    newV[2] = v1[2] + v2[2];

    float scale = _computeScaleForLength(newV, length);
    newV[0] *= scale;
    newV[1] *= scale;
    newV[2] *= scale;
}

void Sphere::_computeHalfTexCoord(const float t1[2], const float t2[2], float newT[2])
{
    newT[0] = (t1[0] + t2[0]) * 0.5f;
    newT[1] = (t1[1] + t2[1]) * 0.5f;
}


//

bool Sphere::_isSharedTexCoord(const float t[2])
{
    // 20 non-shared line segments
    const float S = 1.0f / 11;  // texture steps
    const float T = 1.0f / 3;
    static float segments[] = { S, 0,       0, T,       // 00 - 05
                                S, 0,       S * 2, T,     // 00 - 06
                                S * 3, 0,     S * 2, T,     // 01 - 06
                                S * 3, 0,     S * 4, T,     // 01 - 07
                                S * 5, 0,     S * 4, T,     // 02 - 07
                                S * 5, 0,     S * 6, T,     // 02 - 08
                                S * 7, 0,     S * 6, T,     // 03 - 08
                                S * 7, 0,     S * 8, T,     // 03 - 09
                                S * 9, 0,     S * 8, T,     // 04 - 09
                                S * 9, 0,     1, T * 2,     // 04 - 14
                                0, T,       S * 2, 1,     // 05 - 15
                                S * 3, T * 2,   S * 2, 1,     // 10 - 15
                                S * 3, T * 2,   S * 4, 1,     // 10 - 16
                                S * 5, T * 2,   S * 4, 1,     // 11 - 16
                                S * 5, T * 2,   S * 6, 1,     // 11 - 17
                                S * 7, T * 2,   S * 6, 1,     // 12 - 17
                                S * 7, T * 2,   S * 8, 1,     // 12 - 18
                                S * 9, T * 2,   S * 8, 1,     // 13 - 18
                                S * 9, T * 2,   S * 10, 1,    // 13 - 19
                                1, T * 2,     S * 10, 1 };  // 14 - 19

    // check the point with all 20 line segments
    // if it is on the line segment, it is non-shared
    int count = (int)(sizeof(segments) / sizeof(segments[0]));
    for (int i = 0, j = 2; i < count; i += 4, j += 4)
    {
        if (_isOnLineSegment(&segments[i], &segments[j], t))
            return false;   // not shared
    }

    return true;
}

bool Sphere::_isOnLineSegment(const float a[2], const float b[2], const float c[2])
{
    const float EPSILON = 0.0001f;

    // cross product must be 0 if c is on the line
    float cross = ((b[0] - a[0]) * (c[1] - a[1])) - ((b[1] - a[1]) * (c[0] - a[0]));
    if (cross > EPSILON || cross < -EPSILON)
        return false;

    // c must be within a-b
    if ((c[0] > a[0] && c[0] > b[0]) || (c[0] < a[0] && c[0] < b[0]))
        return false;
    if ((c[1] > a[1] && c[1] > b[1]) || (c[1] < a[1] && c[1] < b[1]))
        return false;

    return true;    // all passed, it is on the line segment
}
