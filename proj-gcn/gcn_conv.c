#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main() {
    int i, j, k;
    FILE *ifp, *ofp;
    char file_name[100];
    char *line = NULL;
    size_t len = 0;
    const char *s = " ";
    char *token = NULL;

    static float x[2708][1433];
    static float weight[1433][16];
    static int edge_index[2][13264];
    static float edge_weight[13264];

    static float x_mul[2708][16];
    static float deg[2708];
    static float deg_inv_sqrt[2708];
    static float norm[13264];
    static float out[13264][16];
    static float result[2708][16];

    // Load x
    sprintf(file_name, "x.txt");
    if (!(ifp = fopen(file_name, "r"))) {
        printf("File x.txt cannot be opened for read.\n");
        return -1;
    }
    for (j = 0; j < 2708; ++j) {
        getline(&line, &len, ifp);
        token = strtok(line, s);
        x[j][0] = atof(token);
        for (i = 1; i < 1433; ++i) {
            token = strtok(NULL, s);
            x[j][i] = atof(token);
        }
    }
    fclose(ifp);

    // Load weights
    sprintf(file_name, "weight_conv1.txt");
    if (!(ifp = fopen(file_name, "r"))) {
        printf("File weight_conv1.txt cannot be opened for read.\n");
        return -1;
    }
    for (j = 0; j < 1433; ++j) {
        getline(&line, &len, ifp);
        token = strtok(line, s);
        weight[j][0] = atof(token);
        for (i = 1; i < 16; ++i) {
            token = strtok(NULL, s);
            weight[j][i] = atof(token);
        }
    }
    fclose(ifp);

    // Load edge indices
    sprintf(file_name, "edge_index.txt");
    if (!(ifp = fopen(file_name, "r"))) {
        printf("File edge_index.txt cannot be opened for read.\n");
        return -1;
    }
    for (j = 0; j < 2; ++j) {
        getline(&line, &len, ifp);
        token = strtok(line, s);
        edge_index[j][0] = atof(token);
        for (i = 1; i < 10556; ++i) {
            token = strtok(NULL, s);
            edge_index[j][i] = atof(token);
        }
    }
    fclose(ifp);

    for (j = 0; j < 2708; ++j) {
        for (i = 0; i < 16; ++i) {
            x_mul[j][i] = 0;
            for (k = 0; k < 1433; ++k) {
                x_mul[j][i] += (x[j][k] * weight[k][i]);
            }
        }
    }

    for (j = 0; j < 2; ++j) {
        for (i = 0; i < 2708; ++i) {
            edge_index[j][10556+i] = i;
        }
    }

    for (i = 0; i < 13264; ++i) {
        edge_weight[i] = 1;
    }

    for (i = 0; i < 13264; ++i) {
        deg[edge_index[0][i]] = deg[edge_index[0][i]] + edge_weight[i];
    }

    for (i = 0; i < 2708; ++i) {
        deg_inv_sqrt[i] = 1 / sqrtf(deg[i]);
    }

    for (i = 0; i < 13264; ++i) {
        norm[i] = deg_inv_sqrt[edge_index[0][i]] * edge_weight[i] * deg_inv_sqrt[edge_index[1][i]];
    }

    for (j = 0; j < 13264; ++j) {
        for (i = 0; i < 16; ++i) {
            out[j][i] = norm[j] * x_mul[edge_index[0][j]][i];
        }
    } 

    for (j = 0; j < 16; ++j) {
        for (i = 0; i < 13264; ++i) {
            result[edge_index[1][i]][j] = result[edge_index[1][i]][j] + out[i][j];
        }
    }

    sprintf(file_name, "result.txt");
    if (!(ofp = fopen(file_name, "w"))) {
        printf("File result.txt cannot be opened for write.\n");
        return -1;
    }
    for (j = 0; j < 2708; ++j) {
        for (i = 0; i < 16; ++i) {
            fprintf(ofp, "%f ", result[j][i]);
        }
        fprintf(ofp, "\n");
    }
    fclose(ofp);

    printf("Processing complete.\n");
    return 0;
}
