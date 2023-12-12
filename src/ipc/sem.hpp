#pragma once
#include <string>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <memory.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>

constexpr size_t SHM_SIZE = 20;
const char *SHM_FILE = "./shm_file";
const char *A_WRITE_FILE = "./A_write";
const char *A_READ_FILE = "./A_read";
constexpr char PROJ_ID = 'B';

// 获取并初始化信号量
int CreateSem(const char *fileName, int val)
{
    // 获取信号量
    int semId = semget(ftok(fileName, PROJ_ID), 1, IPC_CREAT | IPC_EXCL | 0666);
    if (semId == -1) {
        perror("semget: ");
        return semId;
    }

    // 设置初始值
    union semun {
        int val;
    } semVal;
    semVal.val = val;
    int res = semctl(semId, 0, SETVAL, semVal);
    if (res == -1) {
        return res;
    }
    return semId;
}

// 获取已经存在的信号量
int GetSem(const char *fileName)
{
    // 获取信号量
    int semId = semget(ftok(fileName, PROJ_ID), 1, 0666);
    if (semId == -1) {
        perror("semget: ");
    }
    return semId;
}

// 进行P操作
int SemP(int semId)
{
    struct sembuf buf = {
        .sem_num = 0,
        .sem_op = -1,
        .sem_flg = 0};
    return semop(semId, &buf, 1);
}

// 定时器P操作
int SemTimerP(int semId, int time)
{
    struct sembuf buf = {
        .sem_num = 0,
        .sem_op = -1,
        .sem_flg = 0};

    struct timespec timesp = {
        .tv_sec = time,
        .tv_nsec = 0,
    };
    return semtimedop(semId, &buf, 1, &timesp);
}

// 进行V操作
int SemV(int semId)
{
    struct sembuf buf = {0, 1, 0};
    return semop(semId, &buf, 1);
}