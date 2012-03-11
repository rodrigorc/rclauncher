/*
rclauncher: a small program to launch multimedia files using a LIRC remote control.

Copyright (C) 2011 Rodrigo Rivas Costa <rodrigorivascosta@gmail.com>
*/

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
#include <getopt.h>

#include <regex.h>
#include <wordexp.h>

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include <gdk/gdkx.h>
#include "miglib/migtk.h"
#include "miglib/mipango.h"
#include "micairo.h"
#include <gdk/gdkkeysyms.h>

#include "xml/simplexmlparse.h"

class OpenDir
{
public:
    OpenDir(DIR *dir=NULL)
        :m_dir(dir)
    {}
    OpenDir(const char *name)
    {
        m_dir = opendir(name);
    }
    OpenDir(std::string &name)
    {
        m_dir = opendir(name.c_str());
    }
    ~OpenDir()
    {
        Reset();
    }
    void Reset(DIR *dir=NULL)
    {
        if (m_dir)
            closedir(m_dir);
        m_dir = dir;
    }
    operator DIR *() const
    { return m_dir; }
private:
    DIR *m_dir;
};

std::string BaseName(const std::string &file)
{
    if (file == "/")
        return file;

    size_t slash = file.rfind('/');
    if (slash == std::string::npos)
        return "";
    else
        return file.substr(slash + 1);
}

std::string DirName(const std::string &file)
{
    if (file == "/" || file.empty())
        return "/";

    size_t slash = file.rfind('/');
    if (slash == std::string::npos)
        return "";
    else if (slash == 0)
        return "/";
    else
        return file.substr(0, slash);
}

using namespace miglib;

bool g_verbose = false;
bool g_fullscreen = true;
bool g_hideOnRun = false;
std::string g_geometry;

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
    static FileAssoc *Match(const std::vector<FileAssoc*> &assocs, const std::string &file);
    static FileAssoc *MatchGlobal(const std::string &file);
};

struct NameTrans
{
    RegEx regex;
    bool global;
    std::string to;

    NameTrans(const char *sfrom, int cflags, bool cglobal, const char *sto)
        :regex(sfrom, cflags), global(cglobal), to(sto)
    {
    }
    std::string TransformName(const std::string &name) const;

    static std::string TransformName(const std::vector<NameTrans*> &trans, const std::string &name);
    static std::string TransformNameGlobal(const std::string &name);
};

std::string NameTrans::TransformName(const std::string &name) const
{
    regmatch_t matches[10];
    std::string res;
    size_t start = 0, last = 0;
    bool prevEmpty = true;
    do
    {
        int nres = regexec(regex, name.c_str() + start, sizeof(matches) / sizeof(*matches), matches, start? REG_NOTBOL : 0);
        if (nres != REG_NOERROR)
            break;
        if (!(matches[0].rm_eo == 0 && !prevEmpty))
        { //Do not replace empty matches if the previous one was non-empty
            res += name.substr(start, matches[0].rm_so);
            size_t pos = 0;
            for (;;)
            {
                size_t pos2 = to.find('\\', pos);
                if (pos2 != std::string::npos)
                {
                    res += to.substr(pos, pos2 - pos);
                    if (pos2 + 1 < to.size())
                    {
                        char ch = to[pos2 + 1];
                        if (ch >= '0' && ch <= '9')
                        {
                            int idx = ch - '0';
                            if (matches[idx].rm_so != -1)
                                res += name.substr(start + matches[idx].rm_so, matches[idx].rm_eo - matches[idx].rm_so);
                        }
                        else
                            res += ch;
                    }
                    else
                    {
                        res += '\\';
                        break;
                    }
                }
                else
                {
                    res += to.substr(pos);
                    break;
                }
                pos = pos2 + 2;
            }
        }
        if (matches[0].rm_eo > 0)
        {
            start += matches[0].rm_eo;
            prevEmpty = false;
        }
        else
        { //If an empty match is found advance just one char or else we'll get stuck in an infinite loop
            res += name[start];
            start += 1;
            prevEmpty = true;
        }
        last = start;
    } while (global && start <= name.size());
    if (last < name.size())
        res += name.substr(last);
    return res;
}

struct DirEntry
{
    //dispName is the name shown to the user
    //fileName is a lister-specific text to be used by Lister::ActualFile() to build the real file name
    std::string dispName, fileName;
    bool isDir;
    FileAssoc *assoc;
    DirEntry(const std::string &disp, const std::string &file, FileAssoc *fa, bool d)
        :dispName(disp), fileName(file), isDir(d), assoc(fa)
    {
        //assert((fa == NULL) == isDir); //assoc is NULL iff !isDir
    }
    bool operator < (const DirEntry &o) const
    {
        //First of all the directories
        if (isDir && !o.isDir)
            return true;
        if (!isDir && o.isDir)
            return false;
        //And the top directory is always "..", if present.
        bool p1 = dispName == "..", p2 = o.dispName == "..";
        if (p1 && !p2)
            return true;
        if (!p1 && p2)
            return false;
        //Then use collation order
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
    {
        for (size_t i = 0; i < m_assocs.size(); ++i)
            delete m_assocs[i];
        for (size_t i = 0; i < m_nameTrans.size(); ++i)
            delete m_nameTrans[i];
    }
    void AddAssoc(FileAssoc *fa)
    {
        m_assocs.push_back(fa);
    }
    void AddDefaultAssoc()
    {
        m_assocs.push_back(NULL);
    }
    void AddNameTransform(NameTrans *nt)
    {
        m_nameTrans.push_back(nt);
    }
    int Id()
    { return m_id; }

    virtual std::string Title() =0;
    virtual std::string Root() =0;
    virtual void ChangePath(const DirEntry &entry) =0;
    virtual void ChangePath(const std::string &path) =0;
    virtual bool Back(std::string &prev) =0; //returns false if going back from root directory
    virtual void ListDir(std::vector<DirEntry> &files) =0;
    virtual std::string ActualFile(const DirEntry &entry) =0;

    virtual FileAssoc *Match(const std::string &file)
    {
        return FileAssoc::Match(m_assocs, file);
    }

    std::string TransformName(const std::string &name) const
    {
        if (!m_nameTrans.empty())
            return NameTrans::TransformName(m_nameTrans, name);
        else
            return NameTrans::TransformNameGlobal(name);
    }

private:
    int m_id;
    //Smart hack: A NULL value in the m_assocs vector means to search into MatchGlobal recursively.
    std::vector<FileAssoc*> m_assocs;
    std::vector<NameTrans*> m_nameTrans;
    
};

class FileLister : public Lister
{
public:
    FileLister(int id, const std::string &title, const std::string &root)
        :Lister(id), m_title(title), m_root(root), m_cwd("/")
    {}

    std::string Title()
    {
        std::string res = m_title;
        if (!res.empty())
           res += ": ";
        res += m_cwd;
        return res;
    }
    std::string Root()
    { return m_root; }
    virtual void ChangePath(const DirEntry &entry);
    virtual void ChangePath(const std::string &path);
    virtual bool Back(std::string &prev);
    virtual void ListDir(std::vector<DirEntry> &files);
    virtual std::string ActualFile(const DirEntry &entry)
    {
        std::string fullPath = m_cwd;
        if (fullPath != "/")
            fullPath += "/";
        fullPath += entry.fileName;
        return m_root + fullPath;
    }
private:
    std::string m_title, m_root;
    std::string m_cwd; //relative to m_root
};

FileLister g_defaultLister(0, "", "/");

void FileLister::ChangePath(const DirEntry &entry)
{
    if (m_cwd == "/")
        m_cwd.clear(); //to avoid the double /
    m_cwd = m_cwd + "/" + entry.fileName;
}
void FileLister::ChangePath(const std::string &path)
{
    m_cwd = path;
}

bool FileLister::Back(std::string &prev)
{
    if (m_cwd == "/")
        return false;
    prev = BaseName(m_cwd);
    m_cwd = DirName(m_cwd);
    return true;
}

void FileLister::ListDir(std::vector<DirEntry> &files)
{
    std::string realPath = m_root + m_cwd;
    if (realPath != "/" && realPath != "//")
        files.push_back(DirEntry("..", "..", NULL, true));

    OpenDir dir(realPath);
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
            FileAssoc *assoc = Match(name);
            if (assoc)
            {
                std::string disp = TransformName(name);
                files.push_back(DirEntry(disp, name, assoc, false));
            }
        }
    }

    std::sort(files.begin(), files.end());
}

class OpenedFileLister : public Lister
{
public:
    OpenedFileLister(int id, const std::string &title)
        :Lister(id), m_title(title)
    {}
    virtual std::string Title() 
    { return m_title; }
    virtual std::string Root()
    { return ""; }
    virtual bool Back(std::string &prev)
    { return false; }
    virtual void ChangePath(const DirEntry &entry)
    {}
    virtual void ChangePath(const std::string &path)
    {}
    virtual void ListDir(std::vector<DirEntry> &files);
    virtual std::string ActualFile(const DirEntry &entry)
    { return entry.fileName; }
private:
    std::string m_title;
};

void OpenedFileLister::ListDir(std::vector<DirEntry> &files)
{
    OpenDir proc("/proc");
    if (!proc)
        return;
    while (dirent *eproc = readdir(proc))
    {
        char *end;
        (void)(strtol(eproc->d_name, &end, 10) == 0); //to avoid the warn_unused_result
        if (*end) //not an integer
            continue;
        std::string path = "/proc/";
        path += eproc->d_name;
        path += "/fd";

        OpenDir fd(path);
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
                const char *end = strchr(slash, ' '); //sometimes ' (deleted)' is added to the filename
                if (!end)
                    end = slash + strlen(slash);
                std::string base(slash, end);
                FileAssoc *assoc = Match(base);
                if (assoc)
                    files.push_back(DirEntry(base, s, assoc, false));
            }
        }
    }
}

class AmuleLister : public Lister
{
public:
    AmuleLister(int id, const std::string &root)
        :Lister(id), m_root(root)
    {}
    virtual std::string Title() 
    { return "aMule"; }
    virtual std::string Root()
    { return ""; }
    virtual bool Back(std::string &prev)
    { return false; }
    virtual void ChangePath(const DirEntry &entry)
    {}
    virtual void ChangePath(const std::string &path)
    {}
    virtual void ListDir(std::vector<DirEntry> &files);
    virtual std::string ActualFile(const DirEntry &entry)
    { return entry.fileName; }
private:
    std::string m_root;
};

void AmuleLister::ListDir(std::vector<DirEntry> &files)
{
    OpenDir dir(m_root);
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
                        FileAssoc *assoc = Match(name);
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
    std::sort(files.begin(), files.end());
}

struct GraphicOptions
{
    std::string descFont, descFontTitle, descFontQueue;
    struct Color
    {
        double r, g, b;

        void set_source(cairo_t *cr)
        {
            cairo_set_source_rgb(cr, r, g, b);
        }
    };
    Color colorFg, colorFgQ, colorBg, colorScroll;

    GraphicOptions()
    {
        descFont = "Sans 24";
        descFontTitle = "Sans Bold 40";
        descFontQueue = "Sans 16";
        colorFg.r = colorFg.g = colorFg.b = 1;
        colorFgQ.r = colorFgQ.g = 1; colorFgQ.b = 0.5;
        colorBg.r = colorBg.g = colorBg.b = 0;
        colorScroll.r = colorScroll.g = colorScroll.b = 0.75;
    }
};

struct Options
{
    GraphicOptions gr;
    std::vector<FileAssoc*> assocs;
    std::vector<NameTrans*> nameTrans;
    std::vector<Lister*> favorites;

    ~Options()
    {
        for (size_t i = 0; i < assocs.size(); ++i)
            delete assocs[i];
        for (size_t i = 0; i < nameTrans.size(); ++i)
            delete nameTrans[i];
        for (size_t i = 0; i < favorites.size(); ++i)
            delete favorites[i];
    }
} g_options;

/*static*/FileAssoc *FileAssoc::MatchGlobal(const std::string &file)
{
    return FileAssoc::Match(g_options.assocs, file);
}
/*static*/FileAssoc *FileAssoc::Match(const std::vector<FileAssoc*> &assocs, const std::string &file)
{
    bool isGlobal = &g_options.assocs == &assocs;

    //If no associations are defined use the default
    if (!isGlobal && assocs.empty())
        return MatchGlobal(file);

    for (size_t i = 0; i < assocs.size(); ++i)
    {
        FileAssoc *assoc = assocs[i];
        if (assoc)
        {
            if (regexec(assoc->regex, file.c_str(), 0, NULL, 0) == REG_NOERROR)
                return assoc;
        }
        else if (!isGlobal)
        {
            assoc = MatchGlobal(file);
            if (assoc)
                return assoc;
        }
    }
    return NULL;
}

/*static*/std::string NameTrans::TransformName(const std::vector<NameTrans*> &trans, const std::string &name)
{
    std::string res(name);
    for (size_t i = 0; i < trans.size(); ++i)
    {
        const NameTrans &re = *trans[i];
        res = re.TransformName(res);
    }
    return res;
}

/*static*/std::string NameTrans::TransformNameGlobal(const std::string &name)
{
    return NameTrans::TransformName(g_options.nameTrans, name);
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
    int m_lineSel, m_firstLine, m_nLines;
    std::vector<DirEntry> m_files;
    std::vector<int> m_playQueue;
    GPid m_childPid;
    std::string m_childText;
    bool m_isKillable;

    AutoTimeout m_timeoutSpawned;
    gboolean OnTimeoutSpawned();

    PangoFontDescriptionPtr m_font, m_fontTitle, m_fontQueue;

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
    int PositionInQueue(int line);
    void Move(int inc);
    void Select(bool onlyDir);
    void Queue();
    void Unqueue();
    void Back();
    void Refresh();
    bool ChangeFavorite(int nfav);
    void Open(const DirEntry &entry);
    void AfterRun();
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

    gtk_widget_show_all(m_draw);
    if (!g_geometry.empty())
        gtk_window_parse_geometry(m_wnd, g_geometry.c_str());
    gtk_widget_show(m_wnd);

    XResetScreenSaver(gdk_x11_get_default_xdisplay());

    //if (!ChangeFavorite(1))
    //    ChangePath("/");
    ChangeFavorite(1);
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
    case GDK_KEY_KP_Add:
    case GDK_KEY_plus:
        Queue();
        break;
    case GDK_KEY_KP_Subtract:
    case GDK_KEY_minus:
        Unqueue();
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
        if (g_verbose)
            std::cout << "Unknown Key: 0x" << std::hex << e->keyval << std::dec << std::endl;
        break;
    }
    return TRUE;
}

int MainWnd::PositionInQueue(int line)
{
    std::vector<int>::iterator itQueue = std::find(m_playQueue.begin(), m_playQueue.end(), line);
    if (itQueue == m_playQueue.end())
        return -1;
    return itQueue - m_playQueue.begin();
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
    if (!onlyDir && !m_playQueue.empty())
    {
        AfterRun();
        return;
    }

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
            m_lister->ChangePath(entry);
            Refresh();
        }
    }
    else if (!onlyDir)
    {
        Open(entry);
    }
}

void MainWnd::Queue()
{
    if (m_lineSel < 0 || m_lineSel >= static_cast<int>(m_files.size()))
        return;
    const DirEntry &entry = m_files[m_lineSel];
    if (entry.isDir)
        return;
    if (PositionInQueue(m_lineSel) != -1)
        return;

    m_playQueue.push_back(m_lineSel);
    Redraw();
}

void MainWnd::Unqueue()
{
    if (m_lineSel < 0 || m_lineSel >= static_cast<int>(m_files.size()))
        return;
    const DirEntry &entry = m_files[m_lineSel];
    if (entry.isDir)
        return;
    int pos = PositionInQueue(m_lineSel);
    if (pos == -1)
        return;

    m_playQueue.erase(m_playQueue.begin() + pos);
    Redraw();
}

void MainWnd::Refresh()
{
    m_files.clear();
    m_playQueue.clear();
    m_lister->ListDir(m_files);
    m_lineSel = m_firstLine = 0;
    m_nLines = 1;

    Redraw();
}

void MainWnd::Back()
{
    std::string base;
    if (!m_lister->Back(base))
    {
        std::string cwd = m_lister->Root();
        base = BaseName(cwd);
        cwd = DirName(cwd);
        m_lister = &g_defaultLister;
        m_lister->ChangePath(cwd);
    }
    Refresh();

    for (size_t i = 0; i < m_files.size(); ++i)
    {
        if (m_files[i].fileName == base)
        {
            m_lineSel = i;
            break;
        }
    }
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
    m_lister->ChangePath("/");
    Refresh();

    return true;
}

static void SetupSpawnedEnviron(gpointer data)
{
    GCharPtr display( gdk_screen_make_display_name((GdkScreen*)data) );
    g_setenv("DISPLAY", display, TRUE);
}

void MainWnd::Open(const DirEntry &entry)
{
    std::string fullPath = m_lister->ActualFile(entry);
    if (g_verbose)
        std::cout << "Run " << fullPath << std::endl;

    FileAssoc *assoc = entry.assoc;
    if (!assoc)
    {
        if (g_verbose)
            std::cout << "No assoc!"<< std::endl;
        AfterRun();
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
        AfterRun();
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

        if (g_hideOnRun)
            m_timeoutSpawned.SetTimeout(15000, MIGLIB_TIMEOUT_FUNC(MainWnd, OnTimeoutSpawned), this);
    }
    else
    {
        AfterRun();
    }
        
}

gboolean MainWnd::OnTimeoutSpawned()
{
    gtk_widget_hide(m_wnd);
    return FALSE;
}

void MainWnd::OnChildWatch(GPid pid, gint status)
{
    if (g_verbose)
        std::cout << "Finish " << pid << ": " << status << std::endl;
    if (m_childPid == pid)
    {
        m_childPid = 0;
        m_childText.clear();
        if (g_hideOnRun)
        {
            m_timeoutSpawned.Reset();
            gtk_widget_show(m_wnd);
        }
        Redraw();
    }
    g_spawn_close_pid(pid);
    AfterRun();
}

void MainWnd::AfterRun()
{
    if (!m_playQueue.empty())
    {
        int q = m_playQueue[0];
        m_playQueue.erase(m_playQueue.begin());
        Open(m_files[q]);
    }
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

    PangoLayoutPtr layoutQueue(pango_cairo_create_layout(cr));
    
    if (!m_font)
        m_font.Reset(pango_font_description_from_string(g_options.gr.descFont.c_str()));
    if (!m_fontTitle)
        m_fontTitle.Reset(pango_font_description_from_string(g_options.gr.descFontTitle.c_str()));
    if (!m_fontQueue)
        m_fontQueue.Reset(pango_font_description_from_string(g_options.gr.descFontQueue.c_str()));
    
    pango_layout_set_text(layout, "M", 1);
    PangoRectangle baseRect;

    pango_layout_set_font_description(layout, m_fontTitle);
    pango_layout_get_extents(layout, NULL, &baseRect);

    //titleH is the height of the title area
    double titleH = double(baseRect.height) / PANGO_SCALE;

    pango_layout_set_font_description(layout, m_font);
    pango_layout_get_extents(layout, NULL, &baseRect);

    pango_layout_set_font_description(layoutQueue, m_fontQueue);
    pango_layout_set_alignment(layoutQueue, PANGO_ALIGN_CENTER);

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

    GraphicOptions::Color &fg = m_playQueue.empty()? g_options.gr.colorFg : g_options.gr.colorFgQ;
    fg.set_source(cr);
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
        fg.set_source(cr);
    }
    else
    {
        g_options.gr.colorBg.set_source(cr);
        cairo_rectangle(cr, szW, 0, scrollW, szH);
        cairo_fill(cr);
        fg.set_source(cr);
    }
    cairo_rectangle(cr, 0, 0, szW, szH);
    cairo_rectangle(cr, szW, 0, scrollW, szH);
    cairo_stroke(cr);

    for (size_t nLine = m_firstLine; nLine < m_files.size() && static_cast<int>(nLine) < m_firstLine + m_nLines; ++nLine)
    {
        const DirEntry &entry = m_files[nLine];

        if (static_cast<int>(nLine) == m_lineSel)
        {
            fg.set_source(cr);
            cairo_rectangle(cr, 0, 0, szW, lineH);
            cairo_fill(cr);
            g_options.gr.colorBg.set_source(cr);
        }

        double extraMargin = 0;
        int idQueue = PositionInQueue(nLine);
        if (idQueue != -1)
        {
            cairo_rectangle(cr, 2, 2, DELTA_X - 4, lineH - 4);
            cairo_stroke(cr);
            extraMargin += DELTA_X;
            std::ostringstream os;
            os << (idQueue + 1);
            std::string sn = os.str();
            pango_layout_set_text(layoutQueue, sn.data(), sn.size());
            pango_layout_set_width(layoutQueue, DELTA_X);
            cairo_translate(cr, DELTA_X/2, 0);
            pango_cairo_show_layout(cr, layoutQueue);
            cairo_translate(cr, -DELTA_X/2, 0);
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

            extraMargin += DELTA_X;
        }
        else
        {
            //If no icon, only a quarter of the DELTA_X space is left
            //Currently only directories have icon, so...
            extraMargin += DELTA_X / 4;
        }
        //Move forward to draw the text
        cairo_translate(cr, extraMargin, 0);
        pango_layout_set_text(layout, entry.dispName.data(), entry.dispName.size());
        pango_layout_set_width(layout, (szW - extraMargin) * PANGO_SCALE);

        //The name itself
        pango_cairo_show_layout(cr, layout);

        if (static_cast<int>(nLine) == m_lineSel)
            fg.set_source(cr);

        //Advance to the next line
        cairo_translate(cr, -extraMargin, lineH);
    }

    pango_layout_set_font_description(layout, m_fontTitle);
    pango_layout_set_width(layout, (szW + scrollW) * PANGO_SCALE);

    std::string title = m_lister->Title();
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
        fg.set_source(cr);
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
        if (strcmp(cmd, "kill") == 0)
        {
            m_playQueue.clear();
            if (m_isKillable)
            {
                kill(m_childPid, SIGTERM);
            }
        }
        return;
    }

    bool resetScreenSaver = true;

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
    else if (strcmp(cmd, "queue") == 0)
        Queue();
    else if (strcmp(cmd, "unqueue") == 0)
        Unqueue();
    else if (strlen(cmd) > 4 && memcmp(cmd, "fav ", 4) == 0)
    {
        int nfav = atoi(cmd + 4);
        ChangeFavorite(nfav);
    }
    else if (strcmp(cmd, "quit") == 0)
    {
        gtk_main_quit();
        resetScreenSaver = false;
    }
    else
    {
        if (g_verbose)
            std::cout << "Unknown cmd: " << cmd << std::endl;
        resetScreenSaver = false;
    }
    if (resetScreenSaver)
        XResetScreenSaver(gdk_x11_get_default_xdisplay());
}

class RCParser : public Simple_XML_Parser
{
private:
    enum State { TAG_CONFIG, TAG_FAVORITES, TAG_FILE_ASSOC, TAG_NAME, TAG_GRAPHICS, TAG_FONT, TAG_COLOR, TAG_FAVORITE, TAG_PATTERN, TAG_DEFAULT, TAG_NAME_TRANSFORM };
    Lister *m_curLister;
public:
    RCParser()
        :m_curLister(NULL)
    {
        SetStateNext(TAG_INIT, "config", TAG_CONFIG, NULL);
        {
            SetStateNext(TAG_CONFIG, "graphics", TAG_GRAPHICS, "favorites", TAG_FAVORITES, 
                    "file_assoc", TAG_FILE_ASSOC, "name", TAG_NAME, NULL);
            {
                SetStateNext(TAG_GRAPHICS, "font", TAG_FONT, "color", TAG_COLOR, NULL);
                SetStateNext(TAG_FAVORITES, "favorite", TAG_FAVORITE, NULL);
                {
                    SetStateNext(TAG_FAVORITE, 
                            "file_assoc", TAG_FILE_ASSOC, "name", TAG_NAME, NULL);
                }
                SetStateNext(TAG_FILE_ASSOC, "pattern", TAG_PATTERN, "extension", TAG_PATTERN, "default", TAG_DEFAULT, NULL);
                SetStateNext(TAG_NAME, "transform", TAG_NAME_TRANSFORM, NULL);
            }
        }
        SetStateAttr(TAG_FONT, "name", "desc", NULL);
        SetStateAttr(TAG_COLOR, "name", "r", "g", "b", NULL);
        SetStateAttr(TAG_FAVORITE, "num", "title", "path", "module", NULL);
        SetStateAttr(TAG_PATTERN, "match", "ext", "command", "killable", NULL);
        SetStateAttr(TAG_NAME_TRANSFORM, "regex", "to", "flags", NULL);
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
        case TAG_DEFAULT:
            if (m_curLister)
                m_curLister->AddDefaultAssoc();
            break;
        case TAG_NAME_TRANSFORM:
            ParseNameTransform(atts);
            break;
        }
    }
    virtual void EndState(int state)
    {
        switch (state)
        {
        case TAG_FAVORITE:
            m_curLister = NULL;
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
        else if (name == "queue")
            g_options.gr.descFontQueue = desc;
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
        else if (name == "queue")
            g_options.gr.colorFgQ = color;
    }
    void ParseFavorite(const attributes_t &atts)
    {
        const std::string &num = atts[0], &name = atts[1], &path = atts[2], &module = atts[3];
        int id = atoi(num.c_str());
        if (id != 0)
        {
            m_curLister = NULL;
            if (module.empty() || module == "file")
                m_curLister = new FileLister(id, name, path);
            else if (module == "amule")
                m_curLister = new AmuleLister(id, path);
            else if (module == "openedfiles")
                m_curLister = new OpenedFileLister(id, GetExtraArg("title", "Opened"));
            if (m_curLister)
                g_options.favorites.push_back(m_curLister);
            else
            {
                if (g_verbose)
                    std::cout << "Unknown module '" << module << "'" << std::endl;
            }
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
            if (m_curLister)
                m_curLister->AddAssoc(assoc);
            else
                g_options.assocs.push_back(assoc);
        }
        catch (...)
        {
            delete assoc;
        }
    }
    void ParseNameTransform(const attributes_t &atts)
    {
        NameTrans *nt = NULL;
        try
        {
            const std::string &regex = atts[0], &to = atts[1], &flags = atts[2];
            int cflags = 0;
            if (flags.find('n') == std::string::npos)
                cflags = REG_EXTENDED; //default
            if (flags.find('i') != std::string::npos)
                cflags |= REG_ICASE;
            bool global = flags.find('g') != std::string::npos;

            nt = new NameTrans(regex.c_str(), cflags, global, to.c_str());
            if (m_curLister)
                m_curLister->AddNameTransform(nt);
            else
                g_options.nameTrans.push_back(nt);
        }
        catch (...)
        {
            delete nt;
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
    << "\t-d  Hide rclauncher while running the a parogram.\n"
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
        struct option longopts[] =
        {
            {"geometry", required_argument, NULL, 'g'},
            {NULL}
        };
        while ((op = getopt_long_only(argc, argv, "hc:l:vfd", longopts, NULL)) != -1)
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
            case 'd':
                g_hideOnRun = true;
                break;
            case 'g':
                g_geometry = optarg;
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

