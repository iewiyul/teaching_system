/*
负责人：成员 E
*/
#include "Heap.h"

//判断谁应该在上面should_go_up，Heap.c私有函数
static bool should_go_up(Student a,Student b,bool isMinHeap){
    return isMinHeap ? (a.gpa < b.gpa):(a.gpa>b.gpa);//ture代表是小顶堆，a的绩点低，更靠上；
   // flase代表大顶堆，a的绩点高，靠上
}

//交换两个学生swap_stu,Heap.c私有函数
static bool swap_stu(Student* a,Student* b){
    Student t = *a;
    *a = *b;
    *b = t;
}

//上浮（堆插入的时候用）

static void sift_up(Heap*h,int i){
    while(i>0){
        int parent = (i-1)/2;//计算父亲的下标
        if (should_go_up(h ->data[i],h ->data[parent],h->isMinHeap))//判断是否是小顶堆
        {
           swap_stu(&h ->data[i],&h ->data[parent]);//如果是，上浮
           i = parent;
        }else
        {
            break;//不是
        }  
    }//n个节点最多上浮logn次，所以是O(logn)
}

//下沉（堆弹顶/排序用）

static void sift_down(Heap*h,int i,int n){
    while (1){
        int l=2*i+1;//左孩子下标
        int r=2*i+2;//右孩子下标
        int best = i;//假设此时 i 最应该在上面
        if (l<n && should_go_up(h->data[l],h->data[best],h->isMinHeap))//若左孩子存在，将左孩子与父亲作比较
        {
            best = l;
        }
        if (r<n && should_go_up(h->data[r],h->data[best],h->isMinHeap))//若右孩子存在，将右孩子与父亲作比较
        {
            best = r;
        }
        if(best==i){
            break;
        }
        swap_stu(&h ->data[i],&h ->data[best]);//和更优先交换位置
        i=best;//继续下沉
    }
    
}

//创建空堆

Heap*    heap_create(bool isMinHeap) { 
    Heap* h = (Heap*)malloc(sizeof(Heap));//申请内存
    if(!h)return NULL;
    h->capacity=16;
    h->size=0;
    h->isMinHeap=isMinHeap; //判断是小顶堆吗
    h->data=(Student*)malloc(sizeof(Student)*h->capacity);//分配内存
    if (!h->data)
    {
        free(h);
        return NULL;//内存不够，重新分配
    }
    return h;
    //(void)isMinHeap; return NULL;
 }
 
 //释放堆内存

void     heap_destroy(Heap* h) {
    if(!h)return;
    free(h->data);
    free(h);
    //(void)h; 
    }

 //堆的插入

bool     heap_insert(Heap* h, Student s) { 
    if (h->size == h->capacity)//如果容量不够就翻倍扩容
    {
        int nc = h->capacity*2;//定义新容量nc
        Student* nd =(Student*)realloc(h->data,sizeof(Student)*nc);//重新分配内存
        if(!nd)return false;
        h->data=nd;
        h->capacity=nc;
    }
    h->data[h->size]=s;//把新学生放在数组末尾
    sift_up(h,h->size);
    h->size++;
    return true;
    //(void)h; (void)s; return false;
 }

 //弹出堆顶

Student  heap_extractTop(Heap* h)  { 
    Student empty;
    memset(&empty,0,sizeof(empty));//创建一个全0的Student
    if (h->size == 0)return empty;//空堆返回
    Student top =h->data[0];      //堆顶
    h->data[0]=h->data[h->size-1];//末尾搬到根的位置
    h->size--;
    if(h->size > 0)sift_down(h,0,h->size);//下沉
    return top;
    //(void)h; Student z = {0}; return z; 
}

//查看堆顶

Student  heap_peek(Heap* h) { 
    Student empty;
    memset(&empty,0,sizeof(empty)); 
    if(h->size == 0)return empty;
    return h->data[0];
    //(void)h; Student z = {0}; return z; 
}

//查看当前堆的元素个数

int      heap_size(Heap* h)  { 
    return h ? h->size : 0;
    //(void)h; return 0;
 }

 //判断堆是否是空堆

bool     heap_isEmpty(Heap* h)  { 
    return !h || h->size == 0; 
    //(void)h; return true;
 }

//堆排序
//1.建堆
//2.反复取堆顶
void     heap_sort(Student* arr, int n, bool isMinHeap)  { 
    if(!arr||n<=1)return;
    Heap tmp;
    tmp.data = arr;
    tmp.size = n;
    tmp.capacity = n;
    tmp.isMinHeap = isMinHeap;
    for (int i =n/2-1;i>=0;i--)sift_down(&tmp,i,n);//建堆
    for (int i = n-1; i >0; i--){                  //排序
        swap_stu(&arr[0],&arr[i]);
        sift_down(&tmp,0,i);
    }                                              
    
    //(void)arr; (void)n; (void)isMinHeap;
 }
 
 //找前K大
 //1.建小顶堆2.遍历

void     heap_topK_max(Student* all, int n, int k, Student* result)   { 
    if(!all||!result||n<=0||k<=0)return;
    if(k>n)k=n;
    Heap*h = heap_create(true);
    for (int i=0;i<n;i++){
        if (h->size<k){
            heap_insert(h,all[i]);//未满直接入堆
        }else if (all[i].gpa > h->data[0].gpa){
            h->data[0] = all[i];
            sift_down(h,0,h->size);//满了，与堆顶替换，下沉
        }
        
    }
    for (int i=k-1;i>=0;i--)result[i]=heap_extractTop(h);//倒序写入result
    heap_destroy(h);


    //(void)all; (void)n; (void)k; (void)result; 
}
