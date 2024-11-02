#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <stdio.h>

typedef struct Mesh
{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int nIndices;
} Mesh;

struct aiMaterialProperty* get_material_property(struct aiMaterial *material, const char* name) {
    for (int i = 0; i < material->mNumProperties; i++) {
        const char* propertyName = material->mProperties[i]->mKey.data;
        if (strcmp(name, propertyName) == 0) {
            return material->mProperties[i];
        }
    }
    return NULL;
}

Mesh *init_mesh(struct aiMesh *mesh_in, struct aiMaterial *material_in)
{
    printf("%s\n", mesh_in->mName.data);
    for (int i = 0; i < material_in->mNumProperties; i++) {
        printf("\t%s, %d, %d\n", material_in->mProperties[i]->mKey.data, (int) material_in->mProperties[i]->mType, material_in->mProperties[i]->mDataLength);
    }

    Mesh* mesh = (Mesh*) malloc(sizeof(Mesh));

    const int attributesPerVertex = 12;
    unsigned int indicesPerFace = mesh_in->mFaces[0].mNumIndices;

    unsigned int nVertices = mesh_in->mNumVertices * attributesPerVertex;
    unsigned int nIndices = mesh_in->mNumFaces * indicesPerFace;

    float *vertices = (float *)calloc(nVertices, sizeof(float));
    if (vertices == NULL)
    {
        fprintf(stderr, "Failed to load vertex data\n");
    }

    unsigned int *indices = (unsigned int *)calloc(nIndices, sizeof(unsigned int));
    if (indices == NULL)
    {
        fprintf(stderr, "Failed to load vertex data\n");
    }

    for (unsigned int i = 0; i < mesh_in->mNumVertices; i++)
    {
        struct aiVector3D vertex = mesh_in->mVertices[i];
        struct aiVector3D normal = mesh_in->mNormals[i];

        struct aiMaterialProperty* diffuse = get_material_property(material_in, "$clr.diffuse");
        if (diffuse == NULL) {
            fprintf(stderr, "Failed to load colors\n");
        }

        float* diffuseColors = (float*) diffuse->mData;
        struct aiVector3D texCoord = mesh_in->mTextureCoords[0] ? mesh_in->mTextureCoords[0][i] : (struct aiVector3D){0.0f, 0.0f, 0.0f};

        vertices[attributesPerVertex * i + 0] = vertex.x;
        vertices[attributesPerVertex * i + 1] = vertex.y;
        vertices[attributesPerVertex * i + 2] = vertex.z;

        vertices[attributesPerVertex * i + 3] = normal.x;
        vertices[attributesPerVertex * i + 4] = normal.y;
        vertices[attributesPerVertex * i + 5] = normal.z;

        vertices[attributesPerVertex * i + 6] = diffuseColors[0];
        vertices[attributesPerVertex * i + 7] = diffuseColors[1];
        vertices[attributesPerVertex * i + 8] = diffuseColors[2];
        vertices[attributesPerVertex * i + 9] = diffuseColors[3];

        vertices[attributesPerVertex * i + 10] = texCoord.x;
        vertices[attributesPerVertex * i + 11] = texCoord.y;

        // printf("#%d (%f, %f, %f), (%f, %f, %f), (%f, %f)\n", i,
        //        vertex.x,
        //        vertex.y,
        //        vertex.z,
        //        normal.x,
        //        normal.y,
        //        normal.z,
        //        texCoord.x,
        //        texCoord.y);
    }

    for (unsigned int i = 0; i < mesh_in->mNumFaces; i++)
    {
        struct aiFace face = mesh_in->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices[i * indicesPerFace + j] = face.mIndices[j];
        }
    }

    unsigned int VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nVertices, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * nIndices, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attributesPerVertex * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, attributesPerVertex * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, attributesPerVertex * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, attributesPerVertex * sizeof(float), (void *)(10 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    free(vertices);
    free(indices);

    mesh->VAO = VAO;
    mesh->VBO = VBO;
    mesh->EBO = EBO;
    mesh->nIndices = nIndices;

    return mesh;
}

void draw_mesh(Mesh *mesh)
{
    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->nIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void destroy_mesh(Mesh *mesh)
{
    glDeleteVertexArrays(1, &mesh->VAO);
    glDeleteBuffers(1, &mesh->VBO);
    free(mesh);
}

typedef struct Model
{
    Mesh **meshes;
    unsigned int nMeshes;
    mat4 *transform;
} Model;

Model *init_model(const char *path, float x, float y, float z)
{
    Model *model = (Model *)malloc(sizeof(Model));

    model->transform = (mat4 *)malloc(sizeof(mat4));
    glm_mat4_identity((vec4 *)model->transform);

    vec3 offset = {x, y, z};
    glm_translate((vec4 *)model->transform, offset);

    const struct aiScene *scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (scene == NULL)
    {
        fprintf(stderr, "Failed to load model from file %s\n", path);
        return NULL;
    }

    if (scene->mNumMeshes > 0)
    {
        model->nMeshes = scene->mNumMeshes;
        model->meshes = (Mesh**) malloc(sizeof(Mesh*) * scene->mNumMeshes);

        for (int i = 0; i < scene->mNumMeshes; i++) {
            struct aiMesh *mesh = scene->mMeshes[i];
            struct aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            model->meshes[i] = init_mesh(mesh, material);
        }
    } else {
        fprintf(stderr, "Model %s has no mesh", path);
    }

    aiReleaseImport(scene);

    return model;
}

void draw_model(Model* model)
{
    for (int i = 0; i < model->nMeshes; i++) {
        draw_mesh(model->meshes[i]);
    }
}

void destroy_model(Model* model) {
    for (int i = 0; i < model->nMeshes; i++) {
        destroy_mesh(model->meshes[i]);
    }

    free(model->meshes);
    free(model->transform);
    free(model);
}
