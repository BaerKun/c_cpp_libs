#include "turnpike_reconstruction.h"
#include "binary_search_tree.h"
#include <stdio.h>
#define STACK_DATA_TYPE TreeNodePtr
#include "stack.h"

typedef struct {
    TreeNodePtr *tree;
    DistanceType *points;
    StackPtr stack;
    int npts;
} Package;

int RT_Delete(TreeNodePtr *const tree, const StackPtr stack, const DistanceType points[], const DistanceType point,
              const int left, const int right, const int end) {
    int i;
    TreeNodePtr node;
    for (i = 0; i < left; i++) {
        node = bstUnlinkWithData(tree, point - points[i]);
        if (node == NULL)
            return i;
        stackPush(stack, node);
    }

    for (i = right + 1; i < end; i++) {
        node = bstUnlinkWithData(tree, points[i] - point);
        if (node == NULL)
            return i;
        stackPush(stack, node);
    }

    return end;
}

void RT_Insert(TreeNodePtr *const tree, const StackPtr stack, int left, const int right, const int end) {
    int i;

    if (end < left)
        left = end;

    for (i = right + 1; i < end; i++) {
        bstInsertNode(tree, *stackTop(stack));
        stackPop(stack);
    }

    for (i = 0; i < left; i++) {
        bstInsertNode(tree, *stackTop(stack));
        stackPop(stack);
    }
}

int reconstructTurnpikeBody(Package *package, const int left, const int right) {
    int isSuccessful = 0;
    int end;

    if (left > right)
        return 1;

    DistanceType max = (*bstFindMax(package->tree))->data;

    if (package->npts ==
        (end = RT_Delete(package->tree, package->stack, package->points, max, left, right, package->npts))) {
        package->points[right] = max;
        isSuccessful = reconstructTurnpikeBody(package, left, right - 1);
    }

    if (!isSuccessful) {
        RT_Insert(package->tree, package->stack, left, right, end);

        if (package->npts ==
            (end = RT_Delete(package->tree, package->stack, package->points, max = package->points[package->npts - 1] - max,
                             left, right, package->npts))) {
            package->points[left] = max;
            isSuccessful = reconstructTurnpikeBody(package, left + 1, right);
        }

        if (!isSuccessful) {
            RT_Insert(package->tree, package->stack, left, right, end);
        }
    }

    return isSuccessful;
}

static TreeNodePtr buildBST(const DataType data[], const int len, TreeNode *const buffer) {
    TreeNodePtr tree = NULL;
    if (buffer == NULL) return NULL;

    for (int i = 0; i < len; i++) {
        buffer[i].data = data[i];
        bstInsertNode(&tree, buffer + i);
    }

    return tree;
}

void reconstructTurnpike(DistanceType distances[], DistanceType points[], const int npts) {
    const int numOfDistances = npts * (npts - 1) / 2;
    const TreeNodePtr buffer = malloc(sizeof(TreeNode) * numOfDistances);
    TreeNodePtr tree = buildBST(distances, numOfDistances, buffer);
    Stack stack;
    stackInit(&stack, numOfDistances);
    Package package = (Package){&tree, points, &stack, npts};

    points[0] = 0;
    points[npts - 1] = bstUnlinkMax(&tree)->data; // buffer，不用free单个节点
    points[npts - 2] = bstUnlinkMax(&tree)->data;

    if (bstUnlinkWithData(&tree, points[npts - 1] - points[npts - 2]) != NULL) {
        if (reconstructTurnpikeBody(&package, 1, npts - 3)) {
            puts("reconstruct successfully!");
            goto END;
        }
    }

    puts("reconstruct failed!");
END:
    stackFreeData(&stack);
    free(buffer);
    free(tree);
}
