#include <mram.h>
#include <alloc.h>
#include <stdint.h>
#include <stdio.h>
#include "mram_alloc.h"
#include "skiplist.h"
#define BATCH_SIZE 8
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

/* Buffer in MRAM. */
__mram_noinit request rqst[BATCH_SIZE];
__mram_noinit response rpse[BATCH_SIZE];

__host mram_allocator *allocator = NULL;
__host zskiplist *zsl = NULL;

int main()
{
    if (!(allocator && zsl))
    {
        allocator = mem_alloc(sizeof(mram_allocator));
        zsl = mem_alloc(sizeof(zskiplist));
        mram_allocator_initial(allocator);
        zslCreate(zsl, allocator);
    }
    __mram_ptr zskiplistNode *node;
    int ret;
    // zslInsert(zsl, 5, 8);
    // zslInsert(zsl, 2, 7);
    // zslInsert(zsl, 4, 5);
    // zslInsert(zsl, 4, 6);
    // zslInsert(zsl, 1, 9);
    // zrangespec spec={4,4,0,0};
    // node = zslFirstInRange(zsl,&spec);
    // if(node){
    //     printf("0\n");
    //     printf("%d\n",node->ele);
    // }
    // else
    //     printf("1\n");
    // node = zslLastInRange(zsl,&spec);
    // if(node){
    //     printf("0\n");
    //     printf("%d\n",node->ele);
    // }
    // else
    //     printf("1\n");
    // ret = zslDelete(zsl, 4, 5);
    // if(ret)
    //     printf("0\n");
    // else
    //     printf("1\n");
    // node = zslFind(zsl, 4, 5);
    // if(node)
    //     printf("0\n");
    // else
    //     printf("1\n");
    // node = zslFind(zsl, 4, 5);
    // if(node)
    //     printf("0\n");
    // else
    //     printf("1\n");

    for (int i = 0; i < BATCH_SIZE; i++)
    {
        switch (rqst[i].operate)
        {
        case 0:
            node = zslInsert(zsl, rqst[i].score, rqst[i].ele);
            if (node)
                rpse[i].response = 0;
            else
                rpse[i].response = 1;
            break;
        case 1:
            node = zslFind(zsl, rqst[i].score, rqst[i].ele);
            if (node)
                rpse[i].response = 0;
            else
                rpse[i].response = 1;
            break;
        case 2:
            ret = zslDelete(zsl, rqst[i].score, rqst[i].ele);
            if (ret)
                rpse[i].response = 0;
            else
                rpse[i].response = 1;
            break;
        default:
            rpse[i].response = 1;
        }
    }
    return 0;
}