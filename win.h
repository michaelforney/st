/* See LICENSE for license details. */

/* Key modifiers */
#define MOD_MASK_ANY    UINT_MAX
#define MOD_MASK_NONE   0
#define MOD_MASK_CTRL   (1<<0)
#define MOD_MASK_ALT    (1<<1)
#define MOD_MASK_SHIFT  (1<<2)
#define MOD_MASK_LOGO   (1<<3)

#define AXIS_VERTICAL   WL_POINTER_AXIS_VERTICAL_SCROLL

void draw(void);
void drawregion(int, int, int, int);

// void xbell(int);
// void xclipcopy(void);
// void xclippaste(void);
// void xhints(void);
void wlloadcols(void);
int wlsetcolorname(int, const char*);
void wlsettitle(char *);
// void xsetpointermotion(int);
void wlselpaste(void);
void setsel(char*);
void zoom(const Arg *);
void zoomabs(const Arg *);
void zoomreset(const Arg *);

void wlneeddraw(void);
