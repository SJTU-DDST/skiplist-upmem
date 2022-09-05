#include <dpu.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#ifndef DPU_BINARY
#define DPU_BINARY "./dpu_task"
#endif

#define BATCH_SIZE 8
#define NR_DPUS 4
typedef struct request
{
    int operate; // 0:insert (score,ele),1:find (score,ele),2:delete (score,ele)
    double score;
    int ele;
} request;
typedef struct response
{
    int response; // 0:OK,1:ERROR
} response;

int main()
{
    struct dpu_set_t set, dpu;
    uint32_t nr_dpus = NR_DPUS;
    uint32_t each_dpu;
    request rqst[BATCH_SIZE * NR_DPUS] = {
        {0, 5, 8}, {0, 2, 7}, {0, 4, 5}, {0, 4, 6}, {0, 1, 9}, {1, 4, 5}, {2, 4, 5}, {1, 4, 5},
        {0, 5, 8}, {0, 2, 7}, {0, 4, 5}, {0, 4, 6}, {0, 1, 9}, {1, 4, 5}, {2, 4, 5}, {1, 4, 5},
        {0, 5, 8}, {0, 2, 7}, {0, 4, 5}, {0, 4, 6}, {0, 1, 9}, {1, 4, 5}, {2, 4, 5}, {1, 4, 5},
        {0, 5, 8}, {0, 2, 7}, {0, 4, 5}, {0, 4, 6}, {0, 1, 9}, {1, 4, 5}, {2, 4, 5}, {1, 4, 5}};
    response rpse[BATCH_SIZE * NR_DPUS] = {
        {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1},
        {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1},
        {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1},
        {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}};
    DPU_ASSERT(dpu_alloc(nr_dpus, NULL, &set));
    DPU_ASSERT(dpu_load(set, DPU_BINARY, NULL));
    DPU_FOREACH(set, dpu, each_dpu)
    {
        DPU_ASSERT(dpu_prepare_xfer(dpu, &rqst[each_dpu * BATCH_SIZE]));
    }
    DPU_ASSERT(dpu_push_xfer(set, DPU_XFER_TO_DPU, "rqst", 0, sizeof(request) * BATCH_SIZE, DPU_XFER_DEFAULT));
    // DPU_ASSERT(dpu_broadcast_to(set, "rqst", 0, rqst, sizeof(request) * BATCH_SIZE, DPU_XFER_DEFAULT));
    DPU_ASSERT(dpu_launch(set, DPU_SYNCHRONOUS));
    DPU_FOREACH(set, dpu, each_dpu)
    {
        DPU_ASSERT(dpu_prepare_xfer(dpu, &rpse[each_dpu * BATCH_SIZE]));
    }
    DPU_ASSERT(dpu_push_xfer(set, DPU_XFER_FROM_DPU, "rpse", 0, sizeof(response) * BATCH_SIZE, DPU_XFER_DEFAULT));
    DPU_FOREACH(set, dpu, each_dpu)
    {
        printf("dpu:[%u]\n", each_dpu);
        for (int i = each_dpu * BATCH_SIZE; i < (each_dpu + 1) * BATCH_SIZE; i++)
        {
            if (!rpse[i].response)
            {
                printf("ok!\n");
            }
            else
            {
                printf("error!\n");
            }
        }
    }
    DPU_ASSERT(dpu_free(set));
    return 0;
}