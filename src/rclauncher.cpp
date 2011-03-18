#include "config.h"
#include <lirc/lirc_client.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdint.h>

#include <regex.h>
#include <wordexp.h>

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "miglib/migtk.h"
#include "miglib/migtkconn.h"
#include "miglib/mipango.h"
#include "micairo.h"
#include <gdk/gdkkeysyms.h>

#include "xml/simplexmlparse.h"

using namespace miglib;

bool g_verbose = false;
bool g_fullscreen = true;


struct ILircClient
{
    virtual void OnLircCommand(const char *cmd) =0;
};

class LircClient
{
public:
    LircClient(const std::string &confFile, ILircClient *cli);
    ~LircClient();
private:
    int m_fd;
    lirc_config *m_cfg;
    GIOChannelPtr m_io;
    ILircClient *m_cli;
    gboolean OnIo(GIOChannel *io, GIOCondition cond);
};

LircClient::LircClient(const std::string &confFile, ILircClient *cli)
    :m_fd(-1), m_cfg(NULL), m_cli(cli)
{
    m_fd = lirc_init(const_cast<char*>("rclauncher"), 0);
    if (m_fd != -1)
    { //Errors are silently ignored
        m_io.Reset( g_io_channel_unix_new(m_fd) );
        g_io_channel_set_raw_nonblock(m_io, NULL);
        MIGLIB_IO_ADD_WATCH(m_io, G_IO_IN, LircClient, OnIo, this);
        lirc_readconfig(confFile.empty()? NULL : const_cast<char*>(confFile.c_str()), &m_cfg, NULL);
    }
}

LircClient::~LircClient()
{
    if (m_cfg)
        lirc_freeconfig(m_cfg);
    if (m_fd != -1)
        lirc_deinit();
}

gboolean LircClient::OnIo(GIOChannel *io, GIOCondition cond)
{
    char *code, *cmd;
    while (lirc_nextcode(&code) == 0 && code != NULL)
    {
        if (g_verbose)
            std::cout << "Code: " << code << std::endl;
        while (lirc_code2char(m_cfg, code, &cmd) == 0 && cmd != NULL)
        {
            m_cli->OnLircCommand(cmd);
        }
	free(code);
    }
    return TRUE;
}

class RegEx
{
public:
    RegEx(const char *re, int cflags)
    {
        int res = regcomp(&m_re, re, cflags);
        if (res != 0)
            Throw(res, re);
    }
    ~RegEx()
    {
        regfree(&m_re);
    }
    operator const regex_t*() const
    { return &m_re; }
private:
    regex_t m_re;
    void Throw(int res, const std::string &txt);
};

void RegEx::Throw(int res, const std::string &txt)
{
    size_t len = regerror(res, &m_re, 0, 0);
    if (len <= 0)
        throw std::runtime_error("Unknown regex error");
    std::vector<char> msg(len);
    regerror(res, &m_re, msg.data(), msg.size());
    throw std::runtime_error(msg.data());
}

struct FileAssoc
{
    RegEx regex;
    std::vector<std::string> args;
    bool isKillable;

    FileAssoc(const char *re, int cflags)
        :regex(re, cflags), isKillable(false)
    {
    }
    static FileAssoc *Match(const std::string &file);
};


struct DirEntry
{
    std::string dispName, fileName;
    bool isDir;
    FileAssoc *assoc;
    DirEntry(const std::string &disp, const std::string &file, FileAssoc *fa, bool d)
        :dispName(disp), fileName(file), isDir(d), assoc(fa)
    {}
    bool operator < (const DirEntry &o) const
    {
        if (isDir && !o.isDir)
            return true;
        if (!isDir && o.isDir)
            return false;
        bool p1 = dispName == "..", p2 = o.dispName == "..";
        if (p1 && !p2)
            return true;
        if (!p1 && p2)
            return false;
        int r = strcoll(dispName.c_str(), o.dispName.c_str());
        return r < 0;
    }
};

class Lister
{
public:
    Lister(int id)
        :m_id(id)
    {}
    virtual ~Lister()
    {}

    int Id()
    { return m_id; }
    virtual std::string Title(const std::string &cwd) =0;
    virtual std::string Root() =0;
    virtual void ListDir(const std::string &path, std::vector<DirEntry> &files) =0;
    virtual std::string ActualFile(const std::string &cwd, const DirEntry &entry) =0;
    virtual FileAssoc *Match(const DirEntry &entry) =0;
private:
    int m_id;
};

class FileLister : public Lister
{
public:
    FileLister(int id, const std::string &title, const std::string &root)
        :Lister(id), m_title(title), m_root(root)
    {}

    std::string Title(const std::string &cwd)
    {
        std::string res = m_title;
        if (!res.empty())
           res += ": ";
        res += cwd;
        return res;
    }
    std::string Root()
    { return m_root; }
    void ListDir(const std::string &path, std::vector<DirEntry> &files);
    virtual std::string ActualFile(const std::string &cwd, const DirEntry &entry)
    {
        std::string fullPath = cwd;
        if (fullPath != "/")
            fullPath += "/";
        fullPath += entry.fileName;
        return m_root + fullPath;
    }
    virtual FileAssoc *Match(const DirEntry &entry)
    { return entry.assoc; }
private:
    std::string m_title, m_root;
};

FileLister g_defaultLister(0, "", "/");

void FileLister::ListDir(const std::string &path, std::vector<DirEntry> &files)
{
    std::string realPath = m_root + path;
    if (realPath != "/" && realPath != "//")
        files.push_back(DirEntry("..", "..", NULL, true));

    DIR *dir = opendir(realPath.c_str());
    if (!dir)
        return;

    while (dirent *entry = readdir(dir))
    {
        enum { T_Other, T_Dir, T_File } type = T_Other;
#ifdef _DIRENT_HAVE_D_TYPE
        if (entry->d_type != DT_UNKNOWN && entry->d_type != DT_LNK)
        {
            type = entry->d_type == DT_DIR? T_Dir : 
                   entry->d_type == DT_REG? T_File : 
                   T_Other;
        }
        else
#endif
        {
            struct stat st;
            if (stat((realPath + "/" + entry->d_name).c_str(), &st) == 0)
            {
                type = S_ISDIR(st.st_mode)? T_Dir : 
                       S_ISREG(st.st_mode)? T_File : 
                       T_Other;
            }
        }

        const std::string name = entry->d_name;
        if (name.empty())
            continue;
        //if (name[0] == '.')
        //    continue; //hidden?
        if (name == "." || name == "..")
            continue;

        if (type == T_Dir)
        {
            if (name[0] == '.')
                continue; //hidden folder
            files.push_back(DirEntry(name, name, NULL, true));
        }
        else if (type == T_File)
        {
            FileAssoc *assoc = FileAssoc::Match(name);
            if (assoc)
                files.push_back(DirEntry(name, name, assoc, false));
        }
    }

    closedir(dir);

    std::sort(files.begin(), files.end());
}

class FlashLister : public Lister
{
public:
    FlashLister(int id)
        :Lister(id)
    {}
    virtual std::string Title(const std::string &) 
    { return "Flash"; }
    virtual std::string Root()
    { return ""; }
    virtual void ListDir(const std::string &path, std::vector<DirEntry> &files);
    virtual std::string ActualFile(const std::string &cwd, const DirEntry &entry)
    { return entry.fileName; }
    virtual FileAssoc *Match(const DirEntry &entry)
    { return entry.assoc; }
};

void FlashLister::ListDir(const std::string &path, std::vector<DirEntry> &files)
{
    DIR *proc = opendir("/proc");
    if (!proc)
        return;
    while (dirent *eproc = readdir(proc))
    {
        char *end;
        strtol(eproc->d_name, &end, 10);
        if (*end) //not an integer
            continue;
        std::string path = "/proc/";
        path += eproc->d_name;
        path += "/fd";

        DIR *fd = opendir(path.c_str());
        if (!fd)
            continue;
        while (dirent *efd = readdir(fd))
        {
            std::string s = path + "/" + efd->d_name;
            char target[100];
            int len = readlink(s.c_str(), target, sizeof(target) - 1);
            if (len < 0)
                continue;
            target[len] = 0;
            const char *slash = strrchr(target, '/');
            if (slash)
            {
                ++slash;
                const char *end = strchr(slash, ' ');
                if (!end)
                    end = slash + strlen(slash);
                std::string base(slash, end);
                if (base.substr(0, 6) == "FlashX")
                {
                    FileAssoc *assoc = FileAssoc::Match(base);
                    if (assoc)
                        files.push_back(DirEntry(base, s, assoc, false));
                }
            }
        }
        closedir(fd);

    }
    closedir(proc);
}

class AmuleLister : public Lister
{
public:
    AmuleLister(int id, const std::string &root)
        :Lister(id), m_root(root)
    {}
    virtual std::string Title(const std::string &) 
    { return "aMule"; }
    virtual std::string Root()
    { return ""; }
    virtual void ListDir(const std::string &path, std::vector<DirEntry> &files);
    virtual std::string ActualFile(const std::string &cwd, const DirEntry &entry)
    { return entry.fileName; }
    virtual FileAssoc *Match(const DirEntry &entry)
    { return entry.assoc; } 
private:
    std::string m_root;
};

void AmuleLister::ListDir(const std::string &, std::vector<DirEntry> &files)
{
    DIR *dir = opendir(m_root.c_str());
    if (!dir)
        return;

    while (dirent *entry = readdir(dir))
    {
#ifdef _DIRENT_HAVE_D_TYPE
        if (entry->d_type != DT_UNKNOWN && entry->d_type != DT_LNK)
        {
            if (entry->d_type != DT_REG)
                continue;
        }
        else
#endif
        {
            struct stat st;
            if (stat((m_root + "/" + entry->d_name).c_str(), &st) < 0 ||
                    !S_ISREG(st.st_mode))
               continue;
        }

        size_t len = strlen(entry->d_name);
        if (len < 4 || strcmp(entry->d_name + len - 4, ".met") != 0)
            continue;
        std::ifstream ifs((m_root + "/" + entry->d_name).c_str(), std::ios::binary);
        //g_print("File: %s\n", entry->d_name);
        uint8_t version;
        ifs.read((char*)&version, 1);
        if (version != 0xE0)
            continue;
        ifs.ignore(4 + 16); //date + hash
        uint16_t parts;
        ifs.read((char*)&parts, 2);
        ifs.ignore(parts * 16); //hashes
        uint32_t tags;
        ifs.read((char*)&tags, 4);
        for (uint32_t i = 0; i < tags; ++i)
        {
            uint8_t type, uname;
            ifs.read((char*)&type, 1);
            if (type & 0x80)
            {
                type &= 0x7F;
                ifs.read((char*)&uname, 1);
	    }
            else
            {
                uint16_t len;
                ifs.read((char*)&len, 2);
                if (len == 1)
                    ifs.read((char*)&uname, 1);
                else
                {
                    uname = 0xFF;
                    ifs.ignore(len);
                }
            }
            switch (type)
            {
            case 1: //HASH16
                ifs.ignore(16);
                break;
            case 2: //STRING
                {
                    uint16_t slen;
                    ifs.read((char*)&slen, 2);
                    std::vector<char> s(slen + 1);
                    ifs.read(s.data(), slen);
                    //g_print("\t%d: '%s'\n", uname, s.data());
                    if (uname == 1) //filename
                    {
                        std::string name(s.data());
                        FileAssoc *assoc = FileAssoc::Match(name);
                        if (assoc)
                            files.push_back(DirEntry(name, m_root + "/" + std::string(entry->d_name, len - 4), assoc, false));
                        goto next;
                    }
                }
                break;
            case 3: //UINT32
                ifs.ignore(4);
                break;
            case 4: //FLOAT32
                ifs.ignore(4);
                break;
            case 5: //BOOL
                ifs.ignore(1);
                break;
            case 11://UINT64
                ifs.ignore(8);
                break;
            case 8: //UINT16
                ifs.ignore(2);
                break;
            case 9: //UINT8
                ifs.ignore(1);
                break;
            case 6: //BOOLARRAY
                //TODO
            case 7: //BLOB
                //TODO
            case 10://BSOB
                //TODO
            default:
                goto next;
            }
        }
next:;
    }
    closedir(dir);
    std::sort(files.begin(), files.end());
}

struct GraphicOptions
{
    std::string descFont, descFontTitle;
    struct Color
    {
        double r, g, b;

        void set_source(cairo_t *cr)
        {
            cairo_set_source_rgb(cr, r, g, b);
        }
    };
    Color colorFg, colorBg, colorScroll;

    GraphicOptions()
    {
        descFont = "Sans 24";
        descFontTitle = "Sans Bold 40";
        colorFg.r = colorFg.g = colorFg.b = 1;
        colorBg.r = colorBg.g = colorBg.b = 0;
        colorScroll.r = colorScroll.g = colorScroll.b = 0.75;
    }
};

struct Options
{
    GraphicOptions gr;
    std::vector<FileAssoc*> assocs;
    std::vector<Lister*> favorites;

    ~Options()
    {
        for (size_t i = 0; i < assocs.size(); ++i)
            delete assocs[i];
        for (size_t i = 0; i < favorites.size(); ++i)
            delete favorites[i];
    }
} g_options;

/*static*/FileAssoc *FileAssoc::Match(const std::string &file)
{
    for (size_t i = 0; i < g_options.assocs.size(); ++i)
    {
        FileAssoc *assoc = g_options.assocs[i];
        if (regexec(assoc->regex, file.c_str(), 0, NULL, 0) == REG_NOERROR)
            return assoc;
    }
    return NULL;
}

class MainWnd : private ILircClient
{
public:
    MainWnd(const std::string &lircFile);

private:
    GtkWindowPtr m_wnd;
    GtkDrawingAreaPtr m_draw;
    LircClient m_lirc;
    Lister *m_lister;
    std::string m_cwd; //relative to m_lister
    int m_lineSel, m_firstLine, m_nLines;
    std::vector<DirEntry> m_files;
    GPid m_childPid;
    std::string m_childText;
    bool m_isKillable;

    PangoFontDescriptionPtr m_font, m_fontTitle;

    void OnDestroy(GtkWidget *w)
    {
        gtk_main_quit();
    }
#if GTK_MAJOR_VERSION < 3
    gboolean OnDrawExpose(GtkWidget *w, GdkEventExpose *e);
#else
    gboolean OnDrawDraw(GtkWidget *w, cairo_t *cr);
#endif
    void OnDrawCairo(cairo_t *cr, int width, int height);
    gboolean OnDrawKey(GtkWidget *w, GdkEventKey *e);
    void Move(int inc);
    void Select(bool onlyDir);
    void Back();
    void Refresh();
    void ChangePath(const std::string &path, bool findFav = true);
    bool ChangeFavorite(int nfav);
    void Open(const DirEntry &entry);
    void OnChildWatch(GPid pid, gint status);

    void Redraw();

    //ILircClient
    virtual void OnLircCommand(const char *cmd);
};


MainWnd::MainWnd(const std::string &lircFile)
    :m_lirc(lircFile, this), m_childPid(0), m_isKillable(false)
{
    m_lister = &g_defaultLister;

    m_wnd.Reset( gtk_window_new(GTK_WINDOW_TOPLEVEL) );
    MIGTK_WIDGET_destroy(m_wnd, MainWnd, OnDestroy, this);

    m_draw.Reset( gtk_drawing_area_new() );
#if GTK_MAJOR_VERSION < 3
    MIGTK_WIDGET_expose_event(m_draw, MainWnd, OnDrawExpose, this);
    GdkColor black = {0, 0xFFFF * g_options.gr.colorBg.r, 0xFFFF * g_options.gr.colorBg.g, 0xFFFF * g_options.gr.colorBg.b};
    gtk_widget_modify_bg(m_draw, GTK_STATE_NORMAL, &black);
#else
    MIGTK_WIDGET_draw(m_draw, MainWnd, OnDrawDraw, this);
    GdkRGBA black = {g_options.gr.colorBg.r, g_options.gr.colorBg.g, g_options.gr.colorBg.b, 1.0};
    gtk_widget_override_background_color(m_draw, GTK_STATE_FLAG_NORMAL, &black);
#endif
    gtk_widget_set_can_focus(m_draw, TRUE);
    MIGTK_WIDGET_key_press_event(m_draw, MainWnd, OnDrawKey, this);
    gtk_container_add(m_wnd, m_draw);

    if (g_fullscreen)
        gtk_window_fullscreen(m_wnd);
    gtk_widget_show_all(m_wnd);

    if (!ChangeFavorite(1))
        ChangePath("/");
}

gboolean MainWnd::OnDrawKey(GtkWidget *w, GdkEventKey *e)
{
    if (m_childPid != 0)
    {
        if (m_isKillable && m_childPid && e->keyval == GDK_KEY_Escape)
            kill(m_childPid, SIGTERM);
        return TRUE;
    }

    switch (e->keyval)
    {
    case GDK_KEY_Up:
    case GDK_KEY_KP_Up:
        Move(-1);
        break;
    case GDK_KEY_Down:
    case GDK_KEY_KP_Down:
        Move(1);
        break;
    case GDK_KEY_Page_Up:
    case GDK_KEY_KP_Page_Up:
        Move(-m_nLines);
        break;
    case GDK_KEY_Page_Down:
    case GDK_KEY_KP_Page_Down:
        Move(m_nLines);
        break;
    case GDK_KEY_Left:
    case GDK_KEY_KP_Left:
        Back();
        break;
    case GDK_KEY_Right:
    case GDK_KEY_KP_Right:
        Select(true);
        break;
    case GDK_KEY_Return:
    case GDK_KEY_KP_Enter:
        Select(false);
        break;
    case GDK_KEY_space:
        Refresh();
        break;
    case GDK_KEY_1: 
    case GDK_KEY_KP_1: 
        ChangeFavorite(1); 
        break;
    case GDK_KEY_2: 
    case GDK_KEY_KP_2: 
        ChangeFavorite(2); 
        break;
    case GDK_KEY_3: 
    case GDK_KEY_KP_3: 
        ChangeFavorite(3); 
        break;
    case GDK_KEY_4: 
    case GDK_KEY_KP_4: 
        ChangeFavorite(4); 
        break;
    case GDK_KEY_5: 
    case GDK_KEY_KP_5: 
        ChangeFavorite(5); 
        break;
    case GDK_KEY_6: 
    case GDK_KEY_KP_6: 
        ChangeFavorite(6); 
        break;
    case GDK_KEY_7: 
    case GDK_KEY_KP_7: 
        ChangeFavorite(7); 
        break;
    case GDK_KEY_8: 
    case GDK_KEY_KP_8: 
        ChangeFavorite(8); 
        break;
    case GDK_KEY_9: 
    case GDK_KEY_KP_9: 
        ChangeFavorite(9); 
        break;
    case GDK_KEY_0: 
    case GDK_KEY_KP_0: 
        ChangeFavorite(10); 
        break;
    default:
        //std::cout << "Key: " << std::hex << e->keyval << std::dec << std::endl;
        break;
    }
    return TRUE;
}

void MainWnd::Move(int inc)
{
    m_lineSel += inc;
    if (m_lineSel >= static_cast<int>(m_files.size()))
        m_lineSel = m_files.size() - 1;
    else if (m_lineSel < 0)
        m_lineSel = 0;
    Redraw();
}

void MainWnd::Select(bool onlyDir)
{
    if (m_lineSel < 0 || m_lineSel >= static_cast<int>(m_files.size()))
        return;
    const DirEntry &entry = m_files[m_lineSel];
    if (entry.isDir)
    {
        if (entry.dispName == "..")
        {
            Back();
        }
        else
        {
            if (m_cwd == "/")
                m_cwd.clear(); //to avoid the double /
            ChangePath(m_cwd + "/" + entry.fileName); //TODO: virtualyze
        }
    }
    else if (!onlyDir)
    {
        Open(entry);
    }
}

void MainWnd::Refresh()
{
    ChangePath(m_cwd);
}

void MainWnd::Back()
{
    if (m_cwd == "/")
    {
        m_cwd = m_lister->Root();
        m_lister = &g_defaultLister;
    }
    size_t slash = m_cwd == "/"? std::string::npos : m_cwd.rfind('/');
    if (slash != std::string::npos)
    {
        std::string leaf = m_cwd.substr(slash + 1);
        ChangePath(m_cwd.substr(0, slash));
        for (size_t i = 0; i < m_files.size(); ++i)
        {
            if (m_files[i].fileName == leaf)
            {
                m_lineSel = i;
                break;
            }
        }
    }
    else
    {
        ChangePath("/");
    }
}

void MainWnd::ChangePath(const std::string &path, bool findFav /*=true*/)
{
    //g_print("ChangePath '%s' -> '%s'\n", m_cwd.c_str(), path.c_str());
    m_cwd = path;

    if (findFav)
    {
        std::string realPath = m_lister->Root() + path;
        for (size_t i = 0; i < g_options.favorites.size(); ++i)
        {
            const std::string &root = g_options.favorites[i]->Root();
            if (root.empty())
                continue;
            if (root == path.substr(0, root.size()))
            {
                m_lister = g_options.favorites[i];
                m_cwd = path.substr(root.size());
                break;
            }
        }
    }
    if (m_cwd.empty())
        m_cwd = "/";

    //remove the ending /'s
    while (m_cwd.size() > 1 && m_cwd[m_cwd.size() - 1] == '/')
        m_cwd = m_cwd.substr(0, m_cwd.size() - 1);

    m_files.clear();
    m_lister->ListDir(m_cwd, m_files);
    m_lineSel = m_firstLine = 0;
    m_nLines = 1;

    Redraw();
}

bool MainWnd::ChangeFavorite(int nfav)
{
    size_t i;
    for (i = 0; i < g_options.favorites.size(); ++i)
    {
        if (g_options.favorites[i]->Id() == nfav)
            break;
    }
    if (i == g_options.favorites.size())
        return false;

    m_lister = g_options.favorites[i];
    ChangePath("/", false);
    return true;
}

static void SetupSpawnedEnviron(gpointer data)
{
    GCharPtr display( gdk_screen_make_display_name((GdkScreen*)data) );
    g_setenv("DISPLAY", display, TRUE);
}

void MainWnd::Open(const DirEntry &entry)
{
    std::string fullPath = m_lister->ActualFile(m_cwd, entry);
    if (g_verbose)
        std::cout << "Run " << fullPath << std::endl;

    FileAssoc *assoc = m_lister->Match(entry);
    if (!assoc)
    {
        if (g_verbose)
            std::cout << "No assoc!"<< std::endl;
        return;
    }
    if (g_verbose)
        std::cout << "Assoc:";

    std::vector<const char *> args;
    for (size_t i = 0; i < assoc->args.size(); ++i)
    {
        const std::string &arg = assoc->args[i];
        if (arg == "{}")
            args.push_back(fullPath.c_str());
        else
            args.push_back(arg.c_str());
        if (g_verbose)
            std::cout << " " << args.back() ;
    }
    if (g_verbose)
        std::cout << std::endl;
    if (args.empty())
    {
        //This should not happend, but just in case...
        if (g_verbose)
            std::cout << "Empty command!" << std::endl;
        return;
    }
    gboolean res;
    args.push_back(NULL);
    res = g_spawn_async(NULL, const_cast<char**>(args.data()), NULL,
                GSpawnFlags(G_SPAWN_SEARCH_PATH | G_SPAWN_DO_NOT_REAP_CHILD), 
                SetupSpawnedEnviron, gdk_screen_get_default(), &m_childPid, NULL);
    if (res)
    {
        MIGLIB_CHILD_WATCH_ADD(m_childPid, MainWnd, OnChildWatch, this);
        m_childText = entry.dispName;
        m_isKillable = assoc->isKillable;
        Redraw();
    }
}

void MainWnd::OnChildWatch(GPid pid, gint status)
{
    if (g_verbose)
        std::cout << "Finish " << pid << ": " << status << std::endl;
    if (m_childPid == pid)
    {
        m_childPid = 0;
        m_childText.clear();
        Redraw();
    }
    g_spawn_close_pid(pid);
}

#if GTK_MAJOR_VERSION < 3
gboolean MainWnd::OnDrawExpose(GtkWidget *w, GdkEventExpose *e)
{
    GtkAllocation size;
    gtk_widget_get_allocation(w, &size);

    CairoPtr cr(gdk_cairo_create(gtk_widget_get_window(w)));
    OnDrawCairo(cr, size.width, size.height);
    return TRUE;
}
#else
gboolean MainWnd::OnDrawDraw(GtkWidget *w, cairo_t *cr)
{
    OnDrawCairo(cr, gtk_widget_get_allocated_width(w), gtk_widget_get_allocated_height(w));
    return TRUE;
}
#endif

void MainWnd::OnDrawCairo(cairo_t *cr, int width, int height)
{
    //std::cout << "Draw " << width << " - " << height << std::endl;

    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
    cairo_set_line_width(cr, 2);

    PangoLayoutPtr layout(pango_cairo_create_layout(cr));
    pango_layout_set_ellipsize(layout, PANGO_ELLIPSIZE_MIDDLE);

    if (!m_font)
        m_font.Reset(pango_font_description_from_string(g_options.gr.descFont.c_str()));
    if (!m_fontTitle)
        m_fontTitle.Reset(pango_font_description_from_string(g_options.gr.descFontTitle.c_str()));
    
    pango_layout_set_text(layout, "M", 1);
    PangoRectangle baseRect;

    pango_layout_set_font_description(layout, m_fontTitle);
    pango_layout_get_extents(layout, NULL, &baseRect);

    //titleH is the height of the title area
    double titleH = double(baseRect.height) / PANGO_SCALE;

    pango_layout_set_font_description(layout, m_font);
    pango_layout_get_extents(layout, NULL, &baseRect);

    //lineH is the height of a line in the file list
    double lineH = double(baseRect.height) / PANGO_SCALE;

    //Margins to the borders of the window
    double marginX1 = 20, marginX2 = 20;
    double marginY1 = titleH + 10, marginY2 = 20;
    //scrollW is the width of the scroll bar
    double scrollW = 20;

    //szH,szW are the height and width of the file list view
    double szH = height - (marginY1 + marginY2), szW = width - (marginX1 + marginX2 + scrollW);
    m_nLines = int(szH / lineH);
    szH = m_nLines * lineH;
    marginY2 = height - (marginY1 + szH);

    g_options.gr.colorFg.set_source(cr);
    cairo_translate(cr, marginX1, marginY1);

    cairo_matrix_t matrix;
    cairo_get_matrix(cr, &matrix);

    //DELTA_X is the width reserved for the icon to the left of the file names
    const double DELTA_X = (32.0 / 37.0) * lineH;
    pango_layout_set_height(layout, 0);

    if (m_lineSel < m_firstLine)
        m_firstLine = m_lineSel;
    if (m_lineSel >= m_firstLine + m_nLines)
        m_firstLine = m_lineSel - m_nLines + 1;

    if (static_cast<int>(m_files.size()) > m_nLines)
    {
        double thumbY = m_firstLine * szH / m_files.size();
        double thumbH = m_nLines * szH / m_files.size();
        g_options.gr.colorScroll.set_source(cr);
        cairo_rectangle(cr, szW, thumbY, scrollW, thumbH);
        cairo_fill(cr);
        g_options.gr.colorFg.set_source(cr);
    }
    else
    {
        g_options.gr.colorBg.set_source(cr);
        cairo_rectangle(cr, szW, 0, scrollW, szH);
        cairo_fill(cr);
        g_options.gr.colorFg.set_source(cr);
    }
    cairo_rectangle(cr, 0, 0, szW, szH);
    cairo_rectangle(cr, szW, 0, scrollW, szH);
    cairo_stroke(cr);

    for (size_t nLine = m_firstLine; nLine < m_files.size() && static_cast<int>(nLine) < m_firstLine + m_nLines; ++nLine)
    {
        const DirEntry &entry = m_files[nLine];

        pango_layout_set_text(layout, entry.dispName.data(), entry.dispName.size());
        if (static_cast<int>(nLine) == m_lineSel)
        {
            g_options.gr.colorFg.set_source(cr);
            cairo_rectangle(cr, 0, 0, szW, lineH);
            cairo_fill(cr);
            g_options.gr.colorBg.set_source(cr);
        }
        if (entry.isDir)
        {
            //A small ugly folder. It is designed with a lineH size of 37, 
            //so scale it accordingly
            cairo_scale(cr, lineH / 37.0, lineH / 37.0);
            cairo_move_to(cr, 6, 10);
            cairo_set_line_width(cr, 4);
            cairo_rel_line_to(cr, 0, 18);
            cairo_rel_line_to(cr, 22, 0);
            cairo_rel_line_to(cr, 0, -14);
            cairo_rel_line_to(cr, -10, 0);
            cairo_rel_line_to(cr, -6, -4);
            cairo_close_path(cr);
            //cairo_rectangle(cr, 4, (lineH - 24) / 2, 24, 24);
            cairo_fill_preserve(cr);
            cairo_stroke(cr);

            //And restore the cairo state
            cairo_set_line_width(cr, 2);
            cairo_scale(cr, 37.0 / lineH, 37.0 / lineH);

            //Move forward to draw the text
            cairo_translate(cr, DELTA_X, 0);
            pango_layout_set_width(layout, (szW - DELTA_X) * PANGO_SCALE);
        }
        else
        {
            //If no icon, only a quarter of the DELTA_X space is left
            //Currently only directories have icon, so...
            cairo_translate(cr, DELTA_X / 4, 0);
            pango_layout_set_width(layout, (szW - DELTA_X / 4) * PANGO_SCALE);
        }

        //The name itself
        pango_cairo_show_layout(cr, layout);

        if (static_cast<int>(nLine) == m_lineSel)
            g_options.gr.colorFg.set_source(cr);

        //Advance to the next line
        if (entry.isDir)
            cairo_translate(cr, -DELTA_X, lineH);
        else
            cairo_translate(cr, -DELTA_X / 4, lineH);
    }

    pango_layout_set_font_description(layout, m_fontTitle);
    pango_layout_set_width(layout, (szW + scrollW) * PANGO_SCALE);

    std::string title = m_lister->Title(m_cwd);
    pango_layout_set_text(layout, title.data(), title.size());

    cairo_set_matrix(cr, &matrix);
    cairo_translate(cr, 0, -titleH - 4);
    pango_cairo_show_layout(cr, layout);

    //*******************************
    if (m_childPid != 0)
    {
        //If a child is running, overlay the name of the file on top of everything
        double marginChildW = 25, marginChildH = 25;
        pango_layout_set_text(layout, m_childText.data(), m_childText.size());

        pango_layout_set_width(layout, (width - 2 * marginChildW) * PANGO_SCALE);
        pango_layout_set_height(layout, -1);
        pango_layout_set_ellipsize(layout, PANGO_ELLIPSIZE_NONE);
        pango_layout_set_wrap(layout, PANGO_WRAP_WORD_CHAR);
        pango_layout_get_extents(layout, NULL, &baseRect);
        double childH = double(baseRect.height) / PANGO_SCALE + 2 * marginChildH;
        double childW = double(baseRect.width) / PANGO_SCALE + 2 * marginChildW;

        cairo_set_matrix(cr, &matrix);
        cairo_translate(cr, (width - childW) / 2 - marginX1, (height - childH) / 2 - marginY1);
        cairo_rectangle(cr, 0, 0, childW, childH);
        g_options.gr.colorBg.set_source(cr);
        cairo_fill_preserve(cr);
        g_options.gr.colorFg.set_source(cr);
        cairo_stroke(cr);
        cairo_translate(cr, marginChildW, marginChildH);
        pango_cairo_show_layout(cr, layout);
    }
}

void MainWnd::Redraw()
{
    gtk_widget_queue_draw(m_draw);
}

void MainWnd::OnLircCommand(const char *cmd)
{
    if (m_childPid != 0)
    {
        if (m_isKillable && m_childPid && strcmp(cmd, "kill") == 0)
            kill(m_childPid, SIGTERM);
        return;
    }

    if (strcmp(cmd, "up") == 0)
        Move(-1);
    else if (strcmp(cmd, "down") == 0)
        Move(1);
    else if (strcmp(cmd, "pageup") == 0)
        Move(-m_nLines);
    else if (strcmp(cmd, "pagedown") == 0)
        Move(m_nLines);
    else if (strcmp(cmd, "ok") == 0)
        Select(false);
    else if (strcmp(cmd, "right") == 0)
        Select(true);
    else if (strcmp(cmd, "left") == 0)
        Back();
    else if (strcmp(cmd, "refresh") == 0)
        Refresh();
    else if (strlen(cmd) > 4 && memcmp(cmd, "fav ", 4) == 0)
    {
        int nfav = atoi(cmd + 4);
        ChangeFavorite(nfav);
    }
    else if (strcmp(cmd, "quit") == 0)
        gtk_main_quit();
    else
        if (g_verbose)
            std::cout << "Unknown cmd: " << cmd << std::endl;
}

class RCParser : public Simple_XML_Parser
{
private:
    enum State { TAG_CONFIG, TAG_FAVORITES, TAG_FILE_ASSOC, TAG_GRAPHICS, TAG_FONT, TAG_COLOR, TAG_FAVORITE, TAG_PATTERN };
public:
    RCParser()
    {
        SetStateNext(TAG_INIT, "config", TAG_CONFIG, NULL);
        {
            SetStateNext(TAG_CONFIG, "graphics", TAG_GRAPHICS, "favorites", TAG_FAVORITES, "file_assoc", TAG_FILE_ASSOC, NULL);
            {
                SetStateNext(TAG_GRAPHICS, "font", TAG_FONT, "color", TAG_COLOR, NULL);
                SetStateNext(TAG_FAVORITES, "favorite", TAG_FAVORITE, NULL);
                SetStateNext(TAG_FILE_ASSOC, "pattern", TAG_PATTERN, "extension", TAG_PATTERN, NULL);
            }
        }
        SetStateAttr(TAG_FONT, "name", "desc", NULL);
        SetStateAttr(TAG_COLOR, "name", "r", "g", "b", NULL);
        SetStateAttr(TAG_FAVORITE, "num", "name", "path", "module", NULL);
        SetStateAttr(TAG_PATTERN, "match", "ext", "command", "killable", NULL);
    }
protected:
    virtual void StartState(int state, const std::string &name, const attributes_t &atts)
    {
        switch (state)
        {
        case TAG_FONT:
            ParseFont(atts);
            break;
        case TAG_COLOR:
            ParseColor(atts);
            break;
        case TAG_FAVORITE:
            ParseFavorite(atts);
            break;
        case TAG_PATTERN:
            ParsePattern(atts, name == "pattern");
            break;
        }
    }
private:
    void ParseFont(const attributes_t &atts)
    {
        const std::string &name = atts[0], &desc = atts[1];

        if (name == "normal")
            g_options.gr.descFont = desc;
        else if (name == "title")
            g_options.gr.descFontTitle = desc;
    }
    void ParseColor(const attributes_t &atts)
    {
        const std::string &name = atts[0], &r = atts[1], &g = atts[2], &b = atts[3];

        GraphicOptions::Color color = {atof(r.c_str()), atof(g.c_str()), atof(b.c_str())};
        if (name == "fg")
            g_options.gr.colorFg = color;
        else if (name == "bg")
            g_options.gr.colorBg = color;
        else if (name == "scroll")
            g_options.gr.colorScroll = color;
    }
    void ParseFavorite(const attributes_t &atts)
    {
        const std::string &num = atts[0], &name = atts[1], &path = atts[2], &module = atts[3];
        int id = atoi(num.c_str());
        if (id != 0)
        {
            Lister *lister = NULL;
            if (module.empty() || module == "file")
                lister = new FileLister(id, name, path);
            else if (module == "amule")
                lister = new AmuleLister(id, path);
            else if (module == "flash")
                lister = new FlashLister(id);
            if (lister)
                g_options.favorites.push_back(lister);
        }
    }
    void ParsePattern(const attributes_t &atts, bool isRegex)
    {
        const std::string &match = atts[0], &ext = atts[1], &command = atts[2], &killable = atts[3];
        if ((isRegex && match.empty()) || (!isRegex && ext.empty()))
            return;
        const std::string &regex = isRegex? match : ("\\." + ext + "$");

        FileAssoc *assoc = NULL;
        try
        {
            assoc = new FileAssoc(regex.c_str(), REG_EXTENDED | REG_ICASE | REG_NOSUB);
            if (!killable.empty() && 
                    (atoi(killable.c_str()) != 0 || killable[0] == 'y' || killable[0] == 'Y'))
                assoc->isKillable = true;

            bool isFileArg = false;
            if (!command.empty())
            {
                wordexp_t words;
                if (wordexp(command.c_str(), &words, 0) == 0)
                {
                    for (size_t i = 0; i < words.we_wordc; ++i)
                    {
                        const char *txt = words.we_wordv[i];
                        assoc->args.push_back(txt);
                        if (assoc->args.back().find("{}") != std::string::npos)
                            isFileArg = true;
                    }
                    wordfree(&words);
                }
            }

            if (!isFileArg)
                assoc->args.push_back("{}");
            g_options.assocs.push_back(assoc);
        }
        catch (...)
        {
            delete assoc;
        }
    }
};

static void Help(char *argv0)
{
    std::cout
    << "RC Launcher - A file browser designed to be used with a lirc remote contoller.\n"
    << "Copyright (c) 2011: Rodrigo Rivas Costa <rodrigorivascosta@gmail.com>\n"
    << "\n"
    << "This program comes with NO WARRANTY, to the extent permitted by law. You\n"
    << "may redistribute copies of it under the terms of the GNU GPL Version 2 or\n"
    << "newer. For more information about these matters, see the file LICENSE.\n"
    << "\n";
    std::cout 
    << "Usage: " << argv0 << " [-l <lirc-file>] [-c <config-file>] [<options>...]\n"
    << "\t-l <lirc-file>\n"
    << "\t    Read lirc command definitions from this file instead of the\n"
    << "\t    default one, usually ~/.lircrc.\n"
    << "\t-c <config-file>\n"
    << "\t    Use this configuration file instead of the default one, which\n"
    << "\t    is (~/.rclauncher).\n"
    << "\t-v  Verbose: output some information to the terminal.\n"
    << "\t-f  Do not go fullscreen.\n"
    << std::endl;
}

int main(int argc, char **argv)
{
    try
    {
        gtk_init(&argc, &argv);

        const char *home = getenv("HOME");
        if (!home)
            home = ".";

        std::string configFile = std::string(home) +  "/.rclauncher";
        std::string lircFile;

        int op;
        while ((op = getopt(argc, argv, "hc:l:vf")) != -1)
        {
            switch (op)
            {
            case 'h':
            case '?':
                Help(argv[0]);
                return 1;
            case 'c':
                configFile = optarg;
                break;
            case 'l':
                lircFile = optarg;
                break;
            case 'v':
                g_verbose = true;
                break;
            case 'f':
                g_fullscreen = false;
                break;
            }
        }

        RCParser().ParseFile(configFile);

        MainWnd mainWnd(lircFile);

        gtk_main();
        return 0;
    }
    catch (std::exception &e)
    {
        std::cout << argv[0] << ": fatal error: " << e.what() << std::endl;
        return 1;
    }

}

