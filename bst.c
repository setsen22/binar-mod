#include "DS.h"

/*
  `ptree`-ийн зааж байгаа модонд `x` утгыг оруулна.
  Оруулахдаа хоёртын хайлтын модны зарчмаар оруулах бөгөөд оруулсан
  байрлалаас дээшхи өвөг эцгийн `len`, `height` утгууд өөрчлөгдөнө.
  Мод хоосон байсан бол `ptree->root` хаяг өөрчлөгдөж шинээр орсон оройг заана.
 */
void bs_put(BST *ptree, int x)
{
        Elm **p = &ptree->root;

        while (*p != NULL) {
                if (x < (*p)->x)
                        p = &(*p)->L;
                else if (x > (*p)->x)
                        p = &(*p)->R;
                else
                        return;
        }

        *p = (Elm *)malloc(sizeof(Elm));
        (*p)->x = x;
        (*p)->height = 1;
        (*p)->len = 1;
        (*p)->L = NULL;
        (*p)->R = NULL;

        Elm *path[10000];
        int n = 0;
        Elm *q = ptree->root;

        while (q != NULL) {
                path[n++] = q;

                if (x < q->x)
                        q = q->L;
                else if (x > q->x)
                        q = q->R;
                else
                        break;
        }

        for (int i = n - 1; i >= 0; i--) {
                q = path[i];

                int lh = q->L ? q->L->height : 0;
                int rh = q->R ? q->R->height : 0;

                q->height = (lh > rh ? lh : rh) + 1;
                q->len = 1 + (q->L ? q->L->len : 0)
                           + (q->R ? q->R->len : 0);
        }
}

Elm *bs_get(const BST *ptree, int x)
{
        Elm *p = ptree->root;

        while (p != NULL) {
                if (x == p->x)
                        return p;
                else if (x < p->x)
                        p = p->L;
                else
                        p = p->R;
        }

        return NULL;
        // Функцийг хэрэгжүүлнэ үү
}

/*
  `ptree`-ийн зааж байгаа модноос `x` утгыг хайн олдсон оройн `Elm*` хаягийг буцаана.
  Олдохгүй бол `NULL` хаягийг буцаана.
  Мод дандаа ялгаатай элементүүд хадгална гэж үзэж болно.
 */
Elm *bs_get(const BST *ptree, int x)
{
Elm *p = ptree->root;

        while (p != NULL) {
                if (x == p->x)
                        return p;
                else if (x < p->x)
                        p = p->L;
                else
                        p = p->R;
        }

        return NULL;       // Функцийг хэрэгжүүлнэ үү
}

/*
  Устгах функц: ХХМноос `x` утгыг хайж олоод устгана.
  Олдохгүй бол юу ч хийхгүй.
 */
void bs_del(BST *ptree, int x)
{
Elm **p = &ptree->root;
        Elm *path[10000];
        int n = 0;

        while (*p != NULL && (*p)->x != x) {
                path[n++] = *p;

                if (x < (*p)->x)
                        p = &(*p)->L;
                else
                        p = &(*p)->R;
        }

        if (*p == NULL)
                return;

        Elm *q = *p;

        if (q->L == NULL) {
                *p = q->R;
                free(q);
        }
        else if (q->R == NULL) {
                *p = q->L;
                free(q);
        }
        else {
                Elm **s = &q->R;
                Elm *successorPath[10000];
                int m = 0;

                while ((*s)->L != NULL) {
                        successorPath[m++] = *s;
                        s = &(*s)->L;
                }

                Elm *min = *s;
                q->x = min->x;
                *s = min->R;
                free(min);

                for (int i = m - 1; i >= 0; i--) {
                        Elm *v = successorPath[i];
                        int lh = v->L ? v->L->height : 0;
                        int rh = v->R ? v->R->height : 0;
                        v->height = (lh > rh ? lh : rh) + 1;
                        v->len = 1 + (v->L ? v->L->len : 0)
                                   + (v->R ? v->R->len : 0);
                }
        }

        for (int i = n - 1; i >= 0; i--) {
                Elm *v = path[i];
                int lh = v->L ? v->L->height : 0;
                int rh = v->R ? v->R->height : 0;

                v->height = (lh > rh ? lh : rh) + 1;
                v->len = 1 + (v->L ? v->L->len : 0)
                           + (v->R ? v->R->len : 0);
        }

        if (q->L != NULL || q->R != NULL) {
                int lh = q->L ? q->L->height : 0;
                int rh = q->R ? q->R->height : 0;

                q->height = (lh > rh ? lh : rh) + 1;
                q->len = 1 + (q->L ? q->L->len : 0)
                           + (q->R ? q->R->len : 0);
        }     // Функцийг хэрэгжүүлнэ үү
}

/*
  Хамгийн багыг устгах функц: ХХМноос хамгийг бага утгыг нь устгах функц.
  Устгасан утгыг буцаана.
 */
int bs_delMin(BST *ptree)
{
 Elm **p = &ptree->root;
        Elm *path[10000];
        int n = 0;

        while ((*p)->L != NULL) {
                path[n++] = *p;
                p = &(*p)->L;
        }

        Elm *q = *p;
        int x = q->x;

        *p = q->R;
        free(q);

        for (int i = n - 1; i >= 0; i--) {
                Elm *v = path[i];

                int lh = v->L ? v->L->height : 0;
                int rh = v->R ? v->R->height : 0;

                v->height = (lh > rh ? lh : rh) + 1;
                v->len = 1 + (v->L ? v->L->len : 0)
                           + (v->R ? v->R->len : 0);
        }

        return x;      // Функцийг хэрэгжүүлнэ үү
}

/*
  ХХМыг inorder дарааллаар, нэг мөрөнд нэг утга хэвлэнэ.
 */
void bs_inorder(const BST *ptree)
{

       if (ptree->root == NULL)
                return;

        Elm *stack[10000];
        int top = 0;
        Elm *p = ptree->root;

        while (p != NULL || top > 0) {
                while (p != NULL) {
                        stack[top++] = p;
                        p = p->L;
                }

                p = stack[--top];

                printf("%d %d %d\n", p->x, p->len, p->height);

                p = p->R;
        }
  // Функцийг хэрэгжүүлнэ үү
}

/*
  ХХМноос `x` утгаас эрс бага буюу тэнцүү байх хэдэн орой байгааг олж буцаана.
  Өөрөөр хэлбэл хоёртын хайлтын модны утгуудыг өсөх дарааллаар байрлуулбал
  `x`-ийг оролцуулаад өмнө хэдэн тоо байх вэ? гэсэн үг.
  `x` утга заавал модонд байх албагүй.
 */
int bs_rank(const BST *ptree, int x)
{
Elm *p = ptree->root;
        int count = 0;

        while (p != NULL) {
                if (x < p->x)
                        p = p->L;
                else {
                        count += 1;

                        if (p->L != NULL)
                                count += p->L->len;

                        p = p->R;
                }
        }

        return count;
       // Функцийг хэрэгжүүлнэ үү
}

/*
  ХХМноос `x` утгатай оройг хайж олоод, тухайн оройд суурилсан
  дэд модонд хэдэн орой байгааг олж буцаана.
  Олдохгүй бол -1-ийг буцаана.
 */
int bs_size(const BST *ptree, int x)
{
  Elm *p = bs_get(ptree, x);

        if (p == NULL)
                return -1;

        return p->len;       // Функцийг хэрэгжүүлнэ үү
}

/*
  XXMноос `x`-ээс бага буюу тэнцүү байх хамгийн их утгын `Elm *` хаягийг олж буцаана.
  Олдохгүй бол `NULL` хаягийг буцаана.
 */
Elm *bs_floor(const BST *ptree, int x)
{
 Elm *p = ptree->root;
        Elm *result = NULL;

        while (p != NULL) {
                if (p->x == x)
                        return p;

                if (x < p->x)
                        p = p->L;
                else {
                        result = p;
                        p = p->R;
                }
        }

        return result;
        // Функцийг хэрэгжүүлнэ үү
}

/*
  XXMноос `x`-ээс их буюу тэнцүү байх хамгийн бага утгын `Elm *` хаягийг олж буцаана.
  Олдохгүй бол `NULL` хаягийг буцаана.
 */
Elm *bs_ceiling(const BST *ptree, int x)
{
   Elm *p = ptree->root;
        Elm *result = NULL;

        while (p != NULL) {
                if (p->x == x)
                        return p;

                if (x > p->x)
                        p = p->R;
                else {
                        result = p;
                        p = p->L;
                }
        }

        return result;      // Функцийг хэрэгжүүлнэ үү
}

/*
  ХХМноос `x` утгатай оройг хайж олоод, тухайн оройд суурилсан
  дэд модны өндөр хэд байгааг олж буцаана. Олдохгүй бол -1-ийг буцаана.
 */
int bs_height(const BST *ptree, int x)
{
 Elm *p = bs_get(ptree, x);

        if (p == NULL)
                return -1;

        return p->height;       // Функцийг хэрэгжүүлнэ үү
}
