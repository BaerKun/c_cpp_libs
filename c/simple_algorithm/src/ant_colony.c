#include <stdlib.h>
#include <time.h>
#include <random.h>
#include <node.h>

#define NUM_OF_PLACES 222   // 地点数量
#define MAX_WORK_TIME 8.5   // 最大工作时间
#define INIT_PHEROMONE 0.1f // 初始信息素

typedef Node Path;

float dency = 0.1f; // 挥发率
float alpha = 1.0f;
int nplaces = NUM_OF_PLACES;
float moveTime[NUM_OF_PLACES][NUM_OF_PLACES]; // 两地移动所需时间
float pheromone[NUM_OF_PLACES][NUM_OF_PLACES]; // 信息素
float workTime[NUM_OF_PLACES]; // 工作所需时间
float trendency[NUM_OF_PLACES][NUM_OF_PLACES]; // alpha * 信息素 / 移动时间

void init() {
    int i, j;
    float *_pheromone, *_trendency, *_moveTime;

    for (i = 0; i < nplaces; ++i) {
        _pheromone = pheromone[i];
        _trendency = trendency[i];
        _moveTime = moveTime[i];
        for (j = 0; j < nplaces; ++j) {
            _pheromone[j] = INIT_PHEROMONE;
            _trendency[j] = alpha * INIT_PHEROMONE * _moveTime[j];
        }
    }
}

int haveVisited(Path *head, int place) {
    while (head != NULL) {
        if (head->element == place)
            return 1;
        head = head->next;
    }
    return 0;
}

int chooseTarget(Path *head, int source, float totalTime, char *mask) {
    float sum = 0.f,
            *_moveTime = moveTime[source],
            *_trendency = trendency[source];
    int i;

    // 筛选所有可行路径
    for (i = 0; i < nplaces; ++i) {
        if (totalTime + _moveTime[i] + workTime[i] > MAX_WORK_TIME || haveVisited(head, i)) {
            mask[i] = 0;
            continue;
        }
        mask[i] = 1;
        sum += _trendency[i];
    }

    if (sum == 0.f)
        return -1;

    // 随机选择，左闭右开
    float random = randfloat(0, sum);
    for (i = 0; i < nplaces; ++i) {
        if (mask[i] == 0)
            continue;
        random -= _trendency[i];
        if (random < 0.f)
            return i;
    }

    return nplaces - 1;
}

void update(Path *head, float totalTime) {
    Path *source = head,
            *target = head->next;

    while (target != NULL) {
        pheromone[source->element][target->element] *= 1.f - dency;
        pheromone[source->element][target->element] += 1.f / totalTime;
        trendency[source->element][target->element] =
                alpha * pheromone[source->element][target->element] / moveTime[source->element][target->element];
        source = target;
        target = target->next;
    }
}

void antColony(int nants) {
    float totalTime;
    int source, target;
    Path head, *path;
    char *mask = malloc(nplaces);

    while (nants-- > 0) {
        // 随机出发点
        source = randint(0, nplaces);

        head.element = source;
        head.next = NULL;
        path = &head;

        totalTime = workTime[source];

        while (1) {
            // 选择下一个点
            target = chooseTarget(&head, source, totalTime, mask);

            // 没有可行路径
            if (target == -1)
                break;

            totalTime += moveTime[source][target] + workTime[target];

            // 链接路径
            nodeInsert(&path->next, target);

            source = target;
        }
        path->next = NULL;

        // 更新信息素和趋势
        update(&head, totalTime);
        // 清空路径
        nodeClear(&head.next);
    }
    free(mask);
}

int main() {
    /*
     * 换算、导入数据 moveTime/workTime
     * 设置dency/alpha ...
     */

    init();
    antColony(666);

    // 后续筛选...
}
