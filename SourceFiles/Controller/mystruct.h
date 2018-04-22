#ifndef MYSTRUCT_H
#define MYSTRUCT_H

typedef struct  {
    bool valid, changed;
    int pwm, time;
    QString dj_single_s;
}DJ_single;

typedef struct  {
    QString dj_group_s;
}DJ_group;

#endif // MYSTRUCT_H
