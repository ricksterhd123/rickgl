#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <stdio.h>

typedef struct Model
{
    struct aiScene *scene;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int nIndices;

    mat4 *transform;
} Model;

Model *init_model(const char *path, float x, float y, float z)
{
    Model *model = (Model *)malloc(sizeof(Model));

    model->transform = (mat4 *)malloc(sizeof(mat4));
    glm_mat4_identity((vec4 *)model->transform);

    vec3 offset = {x, y, z};
    glm_translate((vec4*) model->transform, offset);

    const struct aiScene *scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (scene == NULL)
    {
        fprintf(stderr, "Failed to load file\n");
        return NULL;
    }

    const int attriubtesPerVertex = 8;

    if (scene->mNumMeshes > 0)
    {
        struct aiMesh *mesh = scene->mMeshes[0];

        unsigned int nVertices = mesh->mNumVertices * attriubtesPerVertex;
        unsigned int nIndices = mesh->mNumFaces * mesh->mFaces[0].mNumIndices;

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

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            struct aiVector3D vertex = mesh->mVertices[i];
            struct aiVector3D normal = mesh->mNormals[i];
            struct aiVector3D texCoord = mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][i] : (struct aiVector3D){0.0f, 0.0f, 0.0f};

            vertices[attriubtesPerVertex * i] = vertex.x;
            vertices[attriubtesPerVertex * i + 1] = vertex.y;
            vertices[attriubtesPerVertex * i + 2] = vertex.z;
            vertices[attriubtesPerVertex * i + 3] = normal.x;
            vertices[attriubtesPerVertex * i + 4] = normal.y;
            vertices[attriubtesPerVertex * i + 5] = normal.z;
            vertices[attriubtesPerVertex * i + 6] = texCoord.x;
            vertices[attriubtesPerVertex * i + 7] = texCoord.y;

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

        unsigned int curIndex = 0;
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            struct aiFace face = mesh->mFaces[i];
            // printf("face #%d - %d indices\n", i, face.mNumIndices);

            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices[curIndex++] = face.mIndices[j];
            }
        }

        unsigned int VBO, VAO, EBO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // printf("GOt %d, %d\n", nVertices, nIndices);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nVertices, vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * nIndices, indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attriubtesPerVertex * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, attriubtesPerVertex * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, attriubtesPerVertex * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        model->VAO = VAO;
        model->VBO = VBO;
        model->EBO = EBO;
        model->nIndices = nIndices;

        free(vertices);
        free(indices);
    }

    aiReleaseImport(scene);

    return model;
}

void draw_model(Model *model)
{
    glBindVertexArray(model->VAO);
    glDrawElements(GL_TRIANGLES, model->nIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void destroy_model(Model *model)
{
    glDeleteVertexArrays(1, &model->VAO);
    glDeleteBuffers(1, &model->VBO);
}
