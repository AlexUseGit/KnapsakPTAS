#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <vector>
#include <io.h>

     
#define random(x)    (rand() % (x))

#define DET(a1, a2, b1, b2)        ((a1) * (long) (b2) - (a2) * (long) (b1))
#define SWAP(a, b)             { register item t; t = *a; *a = *b; *b = t; }
#define TRUE  1
#define FALSE 0
#define SORTSTACK 200


/* ======================================================================
				 type declarations
   ====================================================================== */

typedef int   boolean; /* boolean variables */
typedef short itype;   /* item profits and weights */
typedef long  stype;   /* sum of pofit or weight */

/* item as seen externally */
typedef struct {
  itype   p;     /* profit */
  itype   w;     /* weight */
  boolean x;     /* solution variable */
} exitem;

/* item as seen internally */
typedef struct {
  itype   p;     /* profit */
  itype   w;     /* weight */
  boolean *x;    /* pointer to original solution variable */
} item;

/* interval stack */
typedef struct istk {
  item    *f;
  item    *l;
  stype   ws;
} istack;


/* ======================================================================
				 global variables
   ====================================================================== */

item    *br;        /* break item */
stype   wsb;        /* profit and wight sum up to break item */
stype   psb;
stype   c;          /* total capacity */
itype   z;          /* lower bound */
item    *fsort;
item    *lsort;
istack  s;          /* sorted interval returned by partsort */

istack  *ihead1, *stack1, *iend1;  /* interval stack, i < br */
istack  *ihead2, *stack2, *iend2;  /* interval stack, i > br */
boolean **ehead, **estack;         /* exception stack */


/* ======================================================================
          expknap algorithm
====================================================================== */

void * palloc(size_t no, size_t each)
{
  long size;
  long *p;

  size = no * (long) each;
  if (size == 0) size = 1;
  p = (long*) malloc(size);
  return p;
}

stype maketest(exitem *f, exitem *l, int r)
{
  register exitem *j;
  register stype sum;
  stype c;
  itype r1;

  sum = 0; r1 = r/10;
  for (j = f; j <= l; j++) {
    j->w = random(r) + 1;
    j->p = random(r) + 1;
    sum += j->w;
  }
  c = sum / 2;
  return c;
}

void testinstance(exitem **f, exitem **l, int n, int r)
{
  exitem *a;

  /* allocate space for test example */
  a = (exitem *) palloc(n, sizeof(exitem));
  *f = a; *l = &a[n-1];

  /* make test instance */
  c = maketest(*f, *l, r);
}


void definesolution(void)
{
  register boolean **j;
  for (j = ehead; j < estack; j++) **j = 1 - **j;
}

void cleare()
{
  estack = ehead;
}

void pushe(item *i)
{
  *estack = i->x;
  estack++;
}

void pushi(istack **stack, item *f, item *l, stype ws)
{
  register istack *pos;
  pos = *stack;
  pos->f = f; pos ->l = l; pos->ws = ws;
  (*stack)++;
}

void reduce(item **f, item **l)
{
  register item *i, *j, *k;
  register itype pb, wb;
  register stype q;

  pb = br->p; wb = br->w;
  q = DET(z+1, c-wsb, pb, wb);
  i = *f; j = *l;
  if (i <= br) {
    k = fsort - 1;
    while (i <= j) {
      if (DET(-j->p, -j->w, pb, wb) < q) {
        SWAP(i, j); i++;       /* not feasible */
      } else {
        SWAP(j, k); j--; k--;  /* feasible */
      }
    }
    if (k == fsort - 1) { SWAP(*f, k); k--; } /* return at least one */
    *l = fsort - 1; *f = k + 1;
  } else {
    k = lsort + 1;
    while (i <= j) {
      if (DET(i->p, i->w, pb, wb) < q) {
        SWAP(i, j); j--;       /* not feasible */
      } else {
        SWAP(i, k); i++; k++;  /* feasible */
      }
    }
    if (k == lsort + 1) { SWAP(*l, k); k++; }  /* return at least one */
    *f = lsort + 1; *l = k - 1;
  }
}

void partsort(item *f, item *l, stype ws)
{
  register itype mp, mw;
  register item *i, *j, *m;
  register stype wi;
  int d;

  d = l - f + 1;
  if (d > 1) {
    m = f + d / 2;
    if (DET(f->p, f->w, m->p, m->w) < 0) SWAP(f, m);
    if (d > 2) {
      if (DET(m->p, m->w, l->p, l->w) < 0) {
	SWAP(m, l);
	if (DET(f->p, f->w, m->p, m->w) < 0) SWAP(f, m);
      }
    }
  }
  if (d <= 3) {
    s.f = f; s.l = l; s.ws = ws;
  } else {
    mp = m->p; mw = m->w;
    i = f; j = l; wi = ws;
    for (;;) {
      do { wi += i->w; i++; } while (DET(i->p, i->w, mp, mw) > 0);
      do {                j--; } while (DET(j->p, j->w, mp, mw) < 0);
      if (i > j) break;
      SWAP(i, j);
    }
    if (wi > c) {
      pushi(&stack2, i,   l, wi); partsort(f, i-1, ws);
    } else {
      pushi(&stack1, f, i-1, ws); partsort(i,   l, wi);
    }
  }
}

boolean sorti(istack **stack)
/* returns TRUE if expansion succeeded, FALSE if no more intervals */
{
  register istack *pos;

  if ((*stack == ihead1) || (*stack == ihead2)) return FALSE;
  (*stack)--;
  pos = *stack;
  reduce(&(pos->f), &(pos->l));
  partsort(pos->f, pos->l, pos->ws);
  if (s.f < fsort) fsort = s.f;
  if (s.l > lsort) lsort = s.l;
  return TRUE;
}

short elebranch(itype ps, itype ws, item *s, item *t)
{
  short improved;

  improved = FALSE;
  if (ws <= 0) {
    if (ps > z) {
      improved = TRUE;
      z = ps;
      cleare();
    }
    for (;;) {
      if (t > lsort) { if (!sorti(&stack2)) break; }
      if (DET(ps-(z+1), ws, t->p, t->w) < 0) break;
      if (elebranch(ps + t->p, ws + t->w, s, t+1)) {
	improved = TRUE; pushe(t);
      }
      t++;
    }
  } else {
    for (;;) {
      if (s < fsort) { if (!sorti(&stack1)) break; }
      if (DET(ps-(z+1), ws, s->p, s->w) < 0) break;
      if (elebranch(ps - s->p, ws - s->w, s-1, t)) {
	improved = TRUE; pushe(s);
      }
      s--;
    }
  }
  return improved;
}

stype heuristic(item *f, item *l)
{
  register item *i;
  register stype ps, ws;
  register itype r, z, pb, dz;

  ps = 0; ws = c;
  for (i = f; i->w <= ws; i++) {
    ws -= i->w;
    ps += i->p;
    *(i->x) = 1;
  }
  br = i; wsb = c-ws; psb = ps;

  /* determine dantzig bound, and use it as upper bound on z */
  dz = (c - wsb) * br->p / br->w;

  /* define initial solution */
  cleare();
  z = 0; if (z == dz) return z;

  /* forward greedy solution */
  r = c - wsb;
  for (i = br; i <= l; i++) {
    if ((i->w <= r) && (i->p > z)) {
      cleare(); pushe(i);
      z = i->p; if (z == dz) return z;
    }
  }

  /* backward greedy solution */
  r = (wsb + br->w) - c; pb = br->p;
  for (i = br-1; i >= f; i--) {
    if ((i->w >= r) && (pb - i->p > z)) {
      cleare(); pushe(br); pushe(i);
      z = pb - i->p; if (z == dz) return z;
    }
  }

  return z;
}

stype expknap(exitem *f, exitem *l, stype cap)
{
  register item *j;
  register exitem *i;
  item *fitem, *litem;
  int n;

  n = l-f+1;
  c = cap;
  ihead1 = (istack   *) palloc(SORTSTACK, sizeof(istack));
  ihead2 = (istack   *) palloc(SORTSTACK, sizeof(istack));
  ehead  = (boolean **) palloc(n, sizeof(boolean *));
  fitem  = (item     *) palloc(n, sizeof(item));
  litem  = fitem + n - 1;
  stack1 = ihead1; iend1 = ihead1 + SORTSTACK;
  stack2 = ihead2; iend2 = ihead2 + SORTSTACK;

  /* copy items to own array */
  for (i = f, j = fitem; i <= l; i++, j++) {
    j->p = i->p; j->w = i->w; j->x = &(i->x); i->x = 0;
  }

  /* find break item */
  partsort(fitem, litem, 0);
  fsort = s.f; lsort = s.l;

  /* solve problem */
  z = heuristic(fitem, litem);
  elebranch(0, wsb-c, br-1, br);

  /* define solution */
  definesolution();

  free(ihead1);
  free(ihead2);
  free(ehead);
  free(fitem);
  return z + psb;
}


/* ======================================================================
				main
   ====================================================================== */

void main(int argc, char *argv[])
{
  exitem *f, *l;
  stype z;
  int n, r;

  if (argc == 3) {
    n = atoi(argv[1]);
    r = atoi(argv[2]);
    printf("Expknap %d, %d, %d\n", n, r);
  } else {
    printf("Expknap\n");
    printf("n = ");
    scanf("%d", &n);
    printf("r = ");
    scanf("%d", &r);
  }

  srand(time(NULL));
  testinstance(&f, &l, n, r);

  std::vector<exitem> task;
  for (exitem* e = f; e <= l; e++)
  {
      task.push_back(*e);
  }

  // starttime();
  z = expknap(f, l, c);

  std::vector<exitem> sol;
  for (exitem* e = f; e <= l; e++)
  {
      sol.push_back(*e);
  }
  // endtime(&time);

  free(f);
}

