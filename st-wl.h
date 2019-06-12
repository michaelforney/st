
/* Arbitrary sizes */
#define UTF_SIZ       4
#define ESC_BUF_SIZ   (128*UTF_SIZ)

#define MOD_MASK_ANY    UINT_MAX
#define MOD_MASK_NONE   0
#define MOD_MASK_CTRL   (1<<0)
#define MOD_MASK_ALT    (1<<1)
#define MOD_MASK_SHIFT  (1<<2)
#define MOD_MASK_LOGO   (1<<3)

#define AXIS_VERTICAL   WL_POINTER_AXIS_VERTICAL_SCROLL



/* Macros */
#define LEN(a)          (sizeof(a) / sizeof(a)[0])
#define BETWEEN(x, a, b)    ((a) <= (x) && (x) <= (b))
#define MAX(a, b)       ((a) < (b) ? (b) : (a))
#define MIN(a, b)       ((a) < (b) ? (a) : (b))
#define MODBIT(x, set, bit) ((set) ? ((x) |= (bit)) : ((x) &= ~(bit)))
#define LIMIT(x, a, b)      (x) = (x) < (a) ? (a) : (x) > (b) ? (b) : (x)
#define IS_SET(flag)        ((term.mode & (flag)) != 0)



/* type declarations */
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned short ushort;

typedef uint_least32_t Rune;

typedef struct {
    xkb_keysym_t k;
    uint mask;
    char *s;
    /* three valued logic variables: 0 indifferent, 1 on, -1 off */
    signed char appkey;    /* application keypad */
    signed char appcursor; /* application cursor */
    signed char crlf;      /* crlf mode          */
} Key;

typedef struct {
    Rune u;           /* character code */
    ushort mode;      /* attribute flags */
    uint32_t fg;      /* foreground  */
    uint32_t bg;      /* background  */
} Glyph;

typedef Glyph *Line;

typedef struct {
    Glyph attr; /* current char attributes */
    int x;
    int y;
    char state;
} TCursor;

typedef union {
    int i;
    uint ui;
    float f;
    const void *v;
} Arg;

typedef struct {
    uint mod;
    xkb_keysym_t keysym;
    void (*func)(const Arg *);
    const Arg arg;
} Shortcut;

/* Internal representation of the screen */
typedef struct {
    int row;      /* nb row */
    int col;      /* nb col */
    Line *line;   /* screen */
    Line *alt;    /* alternate screen */
    int *dirty;  /* dirtyness of lines */
    TCursor c;    /* cursor */
    int top;      /* top    scroll limit */
    int bot;      /* bottom scroll limit */
    int mode;     /* terminal mode flags */
    int esc;      /* escape state flags */
    char trantbl[4]; /* charset table translation */
    int charset;  /* current charset */
    int icharset; /* selected charset for sequence */
    int numlock; /* lock numbers in keyboard */
    int *tabs;
} Term;

typedef struct {
    struct xkb_context *ctx;
    struct xkb_keymap *keymap;
    struct xkb_state *state;
    xkb_mod_index_t ctrl, alt, shift, logo;
    unsigned int mods;
} XKB;

typedef struct {
    uint b;
    uint mask;
    char *s;
} MouseShortcut;

typedef struct {
    int axis;
    int dir;
    uint mask;
    char s[ESC_BUF_SIZ];
} Axiskey;

typedef struct {
    struct wl_display *dpy;
    struct wl_compositor *cmp;
    struct wl_shm *shm;
    struct wl_seat *seat;
    struct wl_keyboard *keyboard;
    struct wl_pointer *pointer;
    struct wl_data_device_manager *datadevmanager;
    struct wl_data_device *datadev;
    struct wl_data_offer *seloffer;
    struct wl_surface *surface;
    struct wl_buffer *buffer;
    struct xdg_wm_base *wm;
    struct xdg_surface *xdgsurface;
    struct xdg_toplevel *xdgtoplevel;
    XKB xkb;
    bool configured;
    int px, py; /* pointer x and y */
    int tw, th; /* tty width and height */
    int w, h; /* window width and height */
    int ch; /* char height */
    int cw; /* char width  */
    int vis;
    char state; /* focus, redraw, visible */
    int cursor; /* cursor style */
    struct wl_callback * framecb;
} Wayland;

typedef struct {
    char str[32];
    uint32_t key;
    int len;
    bool started;
    struct timespec last;
} Repeat;

typedef struct {
    struct wld_context *ctx;
    struct wld_font_context *fontctx;
    struct wld_renderer *renderer;
    struct wld_buffer *buffer, *oldbuffer;
} WLD;

typedef struct {
    int mode;
    int type;
    int snap;
    /*
     * Selection variables:
     * nb – normalized coordinates of the beginning of the selection
     * ne – normalized coordinates of the end of the selection
     * ob – original coordinates of the beginning of the selection
     * oe – original coordinates of the end of the selection
     */
    struct {
        int x, y;
    } nb, ne, ob, oe;

    char *primary;
    struct wl_data_source *source;
    int alt;
    uint32_t tclick1, tclick2;
} Selection;




/* function declarations */
void *xmalloc(size_t);

size_t utf8decode(char *, Rune *, size_t);
size_t utf8encode(Rune, char *);
void usage(void);
void cresize(int, int);
void ttynew(void);
void ttysend(char *, size_t);
size_t ttyread(void);
void ttywrite(const char *, size_t);
void ttyresize(void);
void draw(void);
int cmdfd;
void die(const char *, ...);

int tattrset(int);
void tsetdirtattr(int);
void tsetdirt(int, int);

char *xstrdup(char *);
void tnew(int, int);

void selinit(void);
void selnormalize(void);
void selclear(void);
void selcopy(uint32_t);
int selected(int, int);

void wlsettitle(char *);
void wlresettitle(void);
void wlinit(void);
void wlresize(int, int);
void wlloadcols(void);
int wlsetcolorname(int, const char *);
void wlloadfonts(char *, double);
void wlunloadfonts(void);
void wldrawcursor(void);
void wldraws(char *, Glyph, int, int, int, int);
void framedone(void *, struct wl_callback *, uint32_t);

void datadevoffer(void *, struct wl_data_device *,
        struct wl_data_offer *);
void datadeventer(void *, struct wl_data_device *, uint32_t,
        struct wl_surface *, wl_fixed_t, wl_fixed_t, struct wl_data_offer *);
void datadevleave(void *, struct wl_data_device *);
void datadevmotion(void *, struct wl_data_device *, uint32_t,
        wl_fixed_t x, wl_fixed_t y);
void datadevdrop(void *, struct wl_data_device *);
void datadevselection(void *, struct wl_data_device *,
        struct wl_data_offer *);
void dataofferoffer(void *, struct wl_data_offer *, const char *);
void datasrctarget(void *, struct wl_data_source *, const char *);
void datasrcsend(void *, struct wl_data_source *, const char *, int32_t);
void datasrccancelled(void *, struct wl_data_source *);



/* global variables */
enum glyph_attribute {
    ATTR_NULL       = 0,
    ATTR_BOLD       = 1 << 0,
    ATTR_FAINT      = 1 << 1,
    ATTR_ITALIC     = 1 << 2,
    ATTR_UNDERLINE  = 1 << 3,
    ATTR_BLINK      = 1 << 4,
    ATTR_REVERSE    = 1 << 5,
    ATTR_INVISIBLE  = 1 << 6,
    ATTR_STRUCK     = 1 << 7,
    ATTR_WRAP       = 1 << 8,
    ATTR_WIDE       = 1 << 9,
    ATTR_WDUMMY     = 1 << 10,
    ATTR_BOLD_FAINT = ATTR_BOLD | ATTR_FAINT,
};

enum term_mode {
    MODE_WRAP        = 1 << 0,
    MODE_INSERT      = 1 << 1,
    MODE_APPKEYPAD   = 1 << 2,
    MODE_ALTSCREEN   = 1 << 3,
    MODE_CRLF        = 1 << 4,
    MODE_MOUSEBTN    = 1 << 5,
    MODE_MOUSEMOTION = 1 << 6,
    MODE_REVERSE     = 1 << 7,
    MODE_KBDLOCK     = 1 << 8,
    MODE_HIDE        = 1 << 9,
    MODE_ECHO        = 1 << 10,
    MODE_APPCURSOR   = 1 << 11,
    MODE_MOUSESGR    = 1 << 12,
    MODE_8BIT        = 1 << 13,
    MODE_BLINK       = 1 << 14,
    MODE_FBLINK      = 1 << 15,
    MODE_FOCUS       = 1 << 16,
    MODE_MOUSEX10    = 1 << 17,
    MODE_MOUSEMANY   = 1 << 18,
    MODE_BRCKTPASTE  = 1 << 19,
    MODE_PRINT       = 1 << 20,
    MODE_UTF8        = 1 << 21,
    MODE_SIXEL       = 1 << 22,
    MODE_MOUSE       = MODE_MOUSEBTN|MODE_MOUSEMOTION|MODE_MOUSEX10\
                      |MODE_MOUSEMANY,
};

enum selection_mode {
    SEL_IDLE = 0,
    SEL_EMPTY = 1,
    SEL_READY = 2
};

enum selection_type {
    SEL_REGULAR = 1,
    SEL_RECTANGULAR = 2
};

enum selection_snap {
    SNAP_WORD = 1,
    SNAP_LINE = 2
};

enum window_state {
    WIN_VISIBLE = 1,
    WIN_FOCUSED = 2
};

pid_t pid;
WLD wld;
Wayland wl;
Term term;
Repeat repeat;
Selection sel;

bool needdraw;

char **opt_cmd;
char *opt_class;
char *opt_embed;
char *opt_font;
char *opt_io;
char *opt_line;
char *opt_name;
char *opt_title;

char *usedfont;
double usedfontsize;
double defaultfontsize;



/* config.h globals */
extern char font[];
extern int borderpx;
extern float cwscale;
extern float chscale;
extern unsigned int doubleclicktimeout;
extern unsigned int tripleclicktimeout;
extern unsigned int keyrepeatdelay;
extern unsigned int keyrepeatinterval;
extern int allowaltscreen;
extern unsigned int xfps;
extern unsigned int actionfps;
extern unsigned int cursorthickness;
extern unsigned int blinktimeout;
extern char termname[];
extern unsigned int tabsspaces;
extern const char *colorname[];
extern size_t colornamelen;
extern size_t maxcolornamelen;
extern unsigned int defaultfg;
extern unsigned int defaultbg;
extern unsigned int defaultcs;
extern unsigned int defaultrcs;
extern unsigned int cursorshape;
extern unsigned int cols;
extern unsigned int rows;
extern char mouseshape[];
extern unsigned int mousefg;
extern unsigned int mousebg;
extern unsigned int defaultattr;
extern MouseShortcut mshortcuts[];
extern Axiskey ashortcuts[];
extern size_t ashortcutslen;
extern size_t mshortcutslen;
extern Shortcut shortcuts[];
extern size_t shortcutslen;
extern xkb_keysym_t mappedkeys[];
extern size_t mappedkeyslen;
extern uint forceselmod;
extern uint selmasks[];
extern size_t selmaskslen;
extern char ascii_printable[];
extern uint ignoremod;
extern Key key[];
extern size_t keylen;

/* function definitions used in config.h */
void numlock(const Arg *);
void selpaste(const Arg *);
void wlzoom(const Arg *);
void wlzoomabs(const Arg *);
void wlzoomreset(const Arg *);
void printsel(const Arg *);
void printscreen(const Arg *);
void iso14755(const Arg *);
void toggleprinter(const Arg *);
void sendbreak(const Arg *);
