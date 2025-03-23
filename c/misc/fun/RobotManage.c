#include <stdio.h>
#include <stdlib.h>

typedef struct Command {
    int time;
    char letter;
    int team;
    int id;
    int arg;
} Command;
typedef struct Robot {
    int team;
    int id;
    int type;
    int level;
    int maxHP;
    int maxHT;
    int HP;
    int HT;
    struct Robot *next;
} Robot;

Robot theAlive, theBroken;
Robot *AliveRobots = &theAlive;
Robot *BrokenRobots = &theBroken;


void GetCommand(Command *C) {
    scanf("%d %c %d %d %d", &C->time, &C->letter, &C->team, &C->id, &C->arg);
}

Robot *CreateRobot(int team, int id, int type) {
    Robot *R = malloc(sizeof(struct Robot));
    R->team = team;
    R->id = id;
    R->type = type;
    R->level = 1;
    R->next = NULL;
    R->HT = 0;
    if (type == 0) {
        R->maxHP = R->HP = 100;
        R->maxHT = 100;
    } else {
        R->maxHP = R->HP = 300;
        R->maxHT = 0;
    }
    return R;
}

void AddRobot(Robot *addTo, Robot *addRobot) {
    addRobot->next = addTo->next;
    addTo->next = addRobot;
}

void MoveRobot(Robot *moveTo, Robot *preRobot) {
    Robot *R = preRobot->next;
    preRobot->next = R->next;
    R->next = moveTo->next;
    moveTo->next = R;
}

Robot *FindPre(Robot *Robots, int team, int id) {
    Robot *R;
    for (R = Robots; R->next && (R->next->team != team || R->next->id != id); R = R->next);
    return R;
}

void DoA(int team, int id, int type) {
    Robot *P, *R;
    P = FindPre(BrokenRobots, team, id);
    R = P->next;
    if (R != NULL) {
        R->HP = R->maxHP;
        R->HT = R->maxHT;
        MoveRobot(AliveRobots, P);
    } else {
        P = FindPre(AliveRobots, team, id);
        R = P->next;
        if (R == NULL) {
            R = CreateRobot(team, id, type);
            AddRobot(AliveRobots, R);
        }

    }
}

void DoF(int team, int id, int damage) {
    Robot *P, *R;
    P = FindPre(AliveRobots, team, id);
    R = P->next;
    if (R != NULL) {
        R->HP -= damage;
        if (R->HP <= 0) {
            MoveRobot(BrokenRobots, P);
            printf("D %d %d\n", R->team, R->id);
        }
    }

}

void DoH(int team, int id, int heat) {
    Robot *R;
    R = FindPre(AliveRobots, team, id)->next;
    if (R != NULL && R->type == 0)
        R->HT += heat;
}

void DoU(int team, int id, int level) {
    Robot *R;
    R = FindPre(AliveRobots, team, id)->next;
    if (R != NULL && R->type == 0) {
        if (R->level < level) {
            if (level == 2) {
                R->level = 2;
                R->maxHP = R->HP = 150;
                R->maxHT = R->HT = 200;
            } else if (level == 3) {
                R->level = 3;
                R->maxHP = R->HP = 250;
                R->maxHT = R->HT = 300;
            }
        }
    }
}

void DoCommand(Command *C) {
    switch (C->letter) {
        case 'A':
            DoA(C->team, C->id, C->arg);
            break;
        case 'F':
            DoF(C->team, C->id, C->arg);
            break;
        case 'H':
            DoH(C->team, C->id, C->arg);
            break;
        case 'U':
            DoU(C->team, C->id, C->arg);
    }
}

void Update() {
    Robot *P, *R;
    for (P = AliveRobots, R = P->next; R; P = R, R = R->next) {
        if (R->HT && --R->HT > R->maxHT) {
            R->HP -= R->HT - R->maxHT;
            if (R->HP <= 0) {
                MoveRobot(BrokenRobots, P);
                printf("D %d %d\n", R->team, R->id);
            }
        }
    }
}

void Quit() {
    Robot *R;
    for (R = AliveRobots->next; R != NULL; R = AliveRobots->next) {
        AliveRobots->next = R->next;
        free(R);
    }
    for (R = BrokenRobots->next; R != NULL; R = BrokenRobots->next) {
        BrokenRobots->next = R->next;
        free(R);
    }
}

int main() {
    int N, t;
    scanf("%d", &N);
    Command thisCommand;
    t = 0;
    GetCommand(&thisCommand);
    do {
        while (t == thisCommand.time) {
            DoCommand(&thisCommand);
            if (--N)
                GetCommand(&thisCommand);
            else
                thisCommand.time = 65536;
        }
        Update();
    } while (++t < 65536);
    Quit();
    return 0;
}