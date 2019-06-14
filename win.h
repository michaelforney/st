/* See LICENSE for license details. */

void draw(void);
void drawregion(int, int, int, int);

// void xbell(int);
void wlclipcopy(void);
// void xhints(void);
void wlloadcols(void);
int wlsetcolorname(int, const char*);
void wlsettitle(char *);
int wlsetcursor(int);
// void xsetpointermotion(int);
void wlselpaste(void);
void wlsetsel(char*);

void wlneeddraw(void);
