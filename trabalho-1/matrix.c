#include <stdio.h>
#include <stdlib.h>

struct a{
    int a;
    char c;
};

typedef struct a *a;

int main(void) {

    int **m = (int **) malloc (10*sizeof(int *));
    m[0] = (int *) malloc (100*sizeof(int));

    for (int i=1; i<10; i++)
        m[i] = m[i-1] + 10;
    printf("ha\n");
    for (int i=0; i<10; i++) m[0][i]=i;
    for (int i=0; i<10; i++) printf("%d\n",m[0][i]);


    a *v = (a *) malloc (10*sizeof(a *));
    *v = (a) malloc (10*sizeof(struct a));
    for (int i=1; i<10; i++)
        v[i] = v[i-1] + sizeof(struct a);
    printf("ha\n");
    for (int i=0; i<10; i++) v[i]->a=i;
    printf("ha\n");
    for (int i=0; i<10; i++) printf("%d\n",v[i]->a);

    return 0;
}
