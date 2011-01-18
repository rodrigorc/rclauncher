#include "config.h"
#include <lirc/lirc_client.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>

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

#include "xml/tinyxml.h"

using namespace miglib;

struct ILircClient
{
    virtual void OnLircCommand(const char *cmd) =0;
};

class LircClient
{
public:
    LircClient(ILircClient *cli);
    ~LircClient();
private:
    int m_fd;
    lirc_config *m_cfg;
    GIOChannelPtr m_io;
    ILircClient *m_cli;
    gboolean OnIo(GIOChannel *io, GIOCondition cond);
};

LircClient::LircClient(ILircClient *cli)
    :m_fd(-1), m_cfg(NULL), m_cli(cli)
{
    m_fd = lirc_init("remote_browser", 1);

    m_io.Reset( g_io_channel_unix_new(m_fd) );
    g_io_channel_set_raw_nonblock(m_io, NULL);

    MIGLIB_IO_ADD_WATCH(m_io, G_IO_IN, LircClient, OnIo, this);

    lirc_readconfig("remote_browser.lirc", &m_cfg, NULL);
}

LircClient::~LircClient()
{
    if (m_cfg)
        lirc_freeconfig(m_cfg);
    lirc_deinit();
}

gboolean LircClient::OnIo(GIOChannel *io, GIOCondition cond)
{
    char *code, *cmd;
    while (lirc_nextcode(&code) == 0 && code != NULL)
    {
        //std::cout << "Code: " << code << std::endl;
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

    FileAssoc(const char *re, int cflags)
        :regex(re, cflags)
    {
    }
};

std::vector<FileAssoc*> g_assocs;

FileAssoc *MatchFile(const std::string &file)
{
    for (size_t i = 0; i < g_assocs.size(); ++i)
    {
        FileAssoc *assoc = g_assocs[i];
        if (regexec(assoc->regex, file.c_str(), 0, NULL, 0) == REG_NOERROR)
            return assoc;
    }
    return NULL;
}


struct DirEntry
{
    std::string name;
    bool isDir;
    DirEntry(const std::string &n, bool d)
        :name(n), isDir(d)
    {}
    bool operator < (const DirEntry &o) const
    {
        if (isDir && !o.isDir)
            return true;
        if (!isDir && o.isDir)
            return false;
        bool p1 = name == "..", p2 = o.name == "..";
        if (p1 && !p2)
            return true;
        if (!p1 && p2)
            return false;
        return name < o.name;
    }
};

void list_dir(const std::string &path, std::vector<DirEntry> &files)
{
    files.clear();
    if (path != "/")
        files.push_back(DirEntry("..", true));

    DIR *dir = opendir(path.c_str());
    if (!dir)
        return;

    while (dirent *entry = readdir(dir))
    {
        enum { T_Other, T_Dir, T_File } type = T_Other;
#ifdef _DIRENT_HAVE_D_TYPE
        if (entry->d_type != DT_UNKNOWN)
        {
            type = entry->d_type == DT_DIR? T_Dir : 
                   entry->d_type == DT_REG? T_File : 
                   T_Other;
        }
        else
#endif
        {
            struct stat st;
            if (stat((path + "/" + entry->d_name).c_str(), &st) == 0)
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
            files.push_back(DirEntry(name, true));
        }
        else if (type == T_File)
        {
            if (MatchFile(name))
                files.push_back(DirEntry(name, false));
        }
    }

    closedir(dir);

    std::sort(files.begin(), files.end());
}

struct Favorite
{
    int id;
    std::string name, path;
};
std::vector<Favorite> g_favorites;

class MainWnd : private ILircClient
{
public:
    MainWnd();

private:
    GtkWindowPtr m_wnd;
    GtkDrawingAreaPtr m_draw;
    LircClient m_lirc;
    std::string m_cwd;
    int m_lineSel, m_firstLine;
    std::vector<DirEntry> m_files;
    GPid m_childPid;
    std::string m_childText;
    int m_favoriteIdx;

    void OnDestroy(GtkObject *w)
    {
        gtk_main_quit();
    }
    gboolean OnDrawExpose(GtkWidget *w, GdkEventExpose *e);
    gboolean OnDrawKey(GtkWidget *w, GdkEventKey *e);
    void Move(bool down);
    void Select(bool onlyDir);
    void Back();
    void ChangePath(const std::string &path, bool findFav = true);
    bool ChangeFavorite(int nfav);
    void Open(const std::string &path);
    void OnChildWatch(GPid pid, gint status);

    void Redraw();

    //ILircClient
    virtual void OnLircCommand(const char *cmd);
};


MainWnd::MainWnd()
    :m_lirc(this), m_childPid(0), m_favoriteIdx(-1)
{
    m_wnd.Reset( gtk_window_new(GTK_WINDOW_TOPLEVEL) );
    MIGTK_OBJECT_destroy(m_wnd, MainWnd, OnDestroy, this);

    m_draw.Reset( gtk_drawing_area_new() );
    MIGTK_WIDGET_expose_event(m_draw, MainWnd, OnDrawExpose, this);
    gtk_widget_set_can_focus(m_draw, TRUE);
    MIGTK_WIDGET_key_press_event(m_draw, MainWnd, OnDrawKey, this);
    GdkColor black = {0, 0x0000, 0x0000, 0x0000};
    gtk_widget_modify_bg(m_draw, GTK_STATE_NORMAL, &black);
    gtk_container_add(m_wnd, m_draw);

    gtk_window_fullscreen(m_wnd);
    gtk_widget_show_all(m_wnd);

    if (!ChangeFavorite(1))
        ChangePath(".");
}

gboolean MainWnd::OnDrawKey(GtkWidget *w, GdkEventKey *e)
{
    if (m_childPid != 0)
        return TRUE;

    switch (e->keyval)
    {
    case GDK_KEY_Up:
    case GDK_KEY_KP_Up:
        Move(false);
        break;
    case GDK_KEY_Down:
    case GDK_KEY_KP_Down:
        Move(true);
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

void MainWnd::Move(bool down)
{
    if (down)
    {
        ++m_lineSel;
        if (m_lineSel >= static_cast<int>(m_files.size()))
            m_lineSel = m_files.size() - 1;
    }
    else
    {
        --m_lineSel;
        if (m_lineSel < 0)
            m_lineSel = 0;
    }
    Redraw();
}

void MainWnd::Select(bool onlyDir)
{
    if (m_lineSel < 0 || m_lineSel >= static_cast<int>(m_files.size()))
        return;
    const DirEntry &entry = m_files[m_lineSel];
    if (entry.isDir)
    {
        if (entry.name == "..")
        {
            Back();
        }
        else
        {
            if (m_cwd == "/")
                m_cwd.clear(); //to avoid the double /
            ChangePath(m_cwd + "/" + entry.name);
        }
    }
    else if (!onlyDir)
    {
        Open(entry.name);
    }
}

void MainWnd::Back()
{
    size_t slash = m_cwd.rfind('/');
    if (slash != std::string::npos)
    {
        std::string leaf = m_cwd.substr(slash + 1);
        ChangePath(m_cwd.substr(0, slash));
        for (size_t i = 0; i < m_files.size(); ++i)
        {
            if (m_files[i].name == leaf)
            {
                m_lineSel = i;
                break;
            }
        }
    }
    else
        ChangePath("/");
}

void MainWnd::ChangePath(const std::string &path, bool findFav /*=true*/)
{
    m_cwd = path;
    if (m_cwd.empty())
        m_cwd = "/";

    while (m_cwd.size() > 1 && m_cwd[m_cwd.size() - 1] == '/')
        m_cwd = m_cwd.substr(0, m_cwd.size() - 1);

    list_dir(m_cwd, m_files);
    m_lineSel = m_firstLine = 0;

    if (findFav)
    {
        m_favoriteIdx = -1;
        for (size_t i = 0; i < g_favorites.size(); ++i)
        {
            if (g_favorites[i].path == path.substr(0, g_favorites[i].path.size()))
            {
                m_favoriteIdx = i;
                break;
            }
        }
    }
    Redraw();
}

bool MainWnd::ChangeFavorite(int nfav)
{
    size_t i;
    for (i = 0; i < g_favorites.size(); ++i)
    {
        if (g_favorites[i].id == nfav)
            break;
    }
    if (i == g_favorites.size())
        return false;

    Favorite &fav = g_favorites[i];

    m_favoriteIdx = i;
    ChangePath(fav.path, false);
    return true;
}

void MainWnd::Open(const std::string &path)
{
    std::string fullPath = m_cwd + "/" + path;
    //std::cout << "Run " << fullPath << std::endl;

    FileAssoc *assoc = MatchFile(path);
    if (!assoc)
    {
        //std::cout << "No assoc!"<< std::endl;
        return;
    }

    //std::cout << "Assoc: "<< assoc->args[0] << std::endl;

    std::vector<const char *> args;
    for (size_t i = 0; i < assoc->args.size(); ++i)
    {
        const std::string &arg = assoc->args[i];
        if (arg == "$1")
            args.push_back(fullPath.c_str());
        else
            args.push_back(arg.c_str());
    }
    args.push_back(NULL);
    if (gdk_spawn_on_screen(gdk_screen_get_default(), NULL, const_cast<char**>(args.data()), NULL, 
                GSpawnFlags(G_SPAWN_SEARCH_PATH | G_SPAWN_DO_NOT_REAP_CHILD), 
                NULL, NULL, &m_childPid, NULL))
    {
        MIGLIB_CHILD_WATCH_ADD(m_childPid, MainWnd, OnChildWatch, this);
        m_childText = path;
        Redraw();
    }
}

void MainWnd::OnChildWatch(GPid pid, gint status)
{
    //std::cout << "Finish " << pid << ": " << status << std::endl;
    if (m_childPid == pid)
    {
        m_childPid = 0;
        m_childText.clear();
        Redraw();
    }
    g_spawn_close_pid(pid);
}

gboolean MainWnd::OnDrawExpose(GtkWidget *w, GdkEventExpose *e)
{
    GtkAllocation size;
    gtk_widget_get_allocation(w, &size);

    //std::cout << "Draw " << size.width << " - " << size.height << std::endl;

    CairoPtr cr(gdk_cairo_create(w->window));
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
    cairo_set_line_width(cr, 2);

    PangoLayoutPtr layout(pango_cairo_create_layout(cr));
    pango_layout_set_ellipsize(layout, PANGO_ELLIPSIZE_MIDDLE);

    PangoFontDescriptionPtr font(pango_font_description_new());
    pango_font_description_set_family(font, "Ubuntu");
    pango_font_description_set_size(font, 24 * PANGO_SCALE);

    PangoFontDescriptionPtr fontTitle(pango_font_description_new());
    pango_font_description_set_family(fontTitle, "Ubuntu");
    pango_font_description_set_size(fontTitle, 40 * PANGO_SCALE);
    pango_font_description_set_weight(fontTitle, PANGO_WEIGHT_BOLD);
    
    pango_layout_set_text(layout, "M", 1);
    PangoRectangle baseRect;

    pango_layout_set_font_description(layout, fontTitle);
    pango_layout_get_extents(layout, NULL, &baseRect);
    double titleH = double(baseRect.height) / PANGO_SCALE;

    pango_layout_set_font_description(layout, font);
    pango_layout_get_extents(layout, NULL, &baseRect);
    double lineH = double(baseRect.height) / PANGO_SCALE;

    double marginX1 = 20, marginX2 = 20;
    double marginY1 = titleH + 10, marginY2 = 20;
    double scrollW = 20;

    double szH = size.height - (marginY1 + marginY2), szW = size.width - (marginX1 + marginX2 + scrollW);
    int nLines = int(szH / lineH);
    szH = nLines * lineH;
    marginY2 = size.height - (marginY1 + szH);

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_translate(cr, marginX1, marginY1);

    cairo_matrix_t matrix;
    cairo_get_matrix(cr, &matrix);

    const double DELTA_X = 32;
    pango_layout_set_height(layout, 0);

    if (m_lineSel < m_firstLine)
        m_firstLine = m_lineSel;
    if (m_lineSel >= m_firstLine + nLines)
        m_firstLine = m_lineSel - nLines + 1;

    if (static_cast<int>(m_files.size()) > nLines)
    {
        double thumbY = m_firstLine * szH / m_files.size();
        double thumbH = nLines * szH / m_files.size();
        cairo_set_source_rgb(cr, 0.75, 0.75, 0.75);
        cairo_rectangle(cr, szW, thumbY, scrollW, thumbH);
        cairo_fill(cr);
        cairo_set_source_rgb(cr, 1, 1, 1);
    }
    else
    {
        cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
        cairo_rectangle(cr, szW, 0, scrollW, szH);
        cairo_fill(cr);
        cairo_set_source_rgb(cr, 1, 1, 1);
    }
    cairo_rectangle(cr, 0, 0, szW, szH);
    cairo_rectangle(cr, szW, 0, scrollW, szH);
    cairo_stroke(cr);

    //std::cout << "S: " << m_lineSel << "  F: " << m_firstLine << "    N: " << nLines << std::endl;
    
    for (size_t nLine = m_firstLine; nLine < m_files.size() && static_cast<int>(nLine) < m_firstLine + nLines; ++nLine)
    {
        const DirEntry &entry = m_files[nLine];

        pango_layout_set_text(layout, entry.name.data(), entry.name.size());
        if (static_cast<int>(nLine) == m_lineSel)
        {
            cairo_set_source_rgb(cr, 1, 1, 1);
            cairo_rectangle(cr, 0, 0, szW, lineH);
            cairo_fill(cr);
            cairo_set_source_rgb(cr, 0, 0, 0);
        }
        if (entry.isDir)
        {
            pango_layout_set_width(layout, (szW - DELTA_X) * PANGO_SCALE);
            //a small ugly folder
            cairo_move_to(cr, 6, (lineH - 18) / 2);
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
            cairo_set_line_width(cr, 2);
            cairo_translate(cr, DELTA_X, 0);
        }
        else
        {
            pango_layout_set_width(layout, (szW - DELTA_X / 4)* PANGO_SCALE);
            cairo_translate(cr, DELTA_X / 4, 0);
        }
        pango_cairo_show_layout(cr, layout);
        if (entry.isDir)
            cairo_translate(cr, -DELTA_X, 0);
        else
            cairo_translate(cr, -DELTA_X / 4, 0);
        if (static_cast<int>(nLine) == m_lineSel)
            cairo_set_source_rgb(cr, 1, 1, 1);

        cairo_translate(cr, 0, lineH);
    }

    pango_layout_set_font_description(layout, fontTitle);
    pango_layout_set_width(layout, (szW + scrollW) * PANGO_SCALE);

    std::string title;
    if (m_favoriteIdx >= 0 && m_favoriteIdx < static_cast<int>(g_favorites.size()))
    {
        const Favorite &fav = g_favorites[m_favoriteIdx];
        if (m_cwd.size() >= fav.path.size())
        {
            title = fav.name + ": " + m_cwd.substr(fav.path.size());
            if (m_cwd.size() == fav.path.size())
                title += "/";
        }
        else
            title = m_cwd;
    }
    else
        title = m_cwd;
    pango_layout_set_text(layout, title.data(), title.size());

    cairo_set_matrix(cr, &matrix);
    cairo_translate(cr, 0, -titleH - 4);
    pango_cairo_show_layout(cr, layout);


    //*******************************
    if (m_childPid != 0)
    {
        double marginChildW = 25, marginChildH = 25;
        pango_layout_set_text(layout, m_childText.data(), m_childText.size());
        /*pango_layout_set_width(layout, -1);
        pango_layout_get_extents(layout, NULL, &baseRect);

        double childH = double(baseRect.height) / PANGO_SCALE + 2 * marginChildH;
        double childW = double(baseRect.width) / PANGO_SCALE + 2 * marginChildW;
        if (childW > size.width)
        {
            marginChildW = (size.width - childW + 2 * marginChildW) / 2;
            if (marginChildW < 0)
                marginChildW = 0;
            childW = size.width;
        }

        pango_layout_set_width(layout, childW * PANGO_SCALE);
        */
        pango_layout_set_width(layout, size.width * PANGO_SCALE);
        pango_layout_set_height(layout, -1);
        pango_layout_set_ellipsize(layout, PANGO_ELLIPSIZE_NONE);
        pango_layout_set_wrap(layout, PANGO_WRAP_WORD_CHAR);
        pango_layout_get_extents(layout, NULL, &baseRect);
        double childH = double(baseRect.height) / PANGO_SCALE + 2 * marginChildH;
        double childW = double(baseRect.width) / PANGO_SCALE + 2 * marginChildW;
        if (childW > size.width)
        {
            childW = size.width;
            pango_layout_set_width(layout, (childW - 2 * marginChildW) * PANGO_SCALE);
        }

        cairo_set_matrix(cr, &matrix);
        cairo_translate(cr, (size.width - childW) / 2 - marginX1, (size.height - childH) / 2 - marginY1);
        cairo_rectangle(cr, 0, 0, childW, childH);
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_fill_preserve(cr);
        cairo_set_source_rgb(cr, 1, 1, 1);
        cairo_stroke(cr);
        cairo_translate(cr, marginChildW, marginChildH);
        pango_cairo_show_layout(cr, layout);
    }
    
    return TRUE;
}

void MainWnd::Redraw()
{
    gtk_widget_queue_draw(m_draw);
}

void MainWnd::OnLircCommand(const char *cmd)
{
    if (m_childPid != 0)
        return;

    if (strcmp(cmd, "up") == 0)
        Move(false);
    else if (strcmp(cmd, "down") == 0)
        Move(true);
    else if (strcmp(cmd, "ok") == 0)
        Select(false);
    else if (strcmp(cmd, "right") == 0)
        Select(true);
    else if (strcmp(cmd, "left") == 0)
        Back();
    else if (strlen(cmd) > 4 && memcmp(cmd, "fav ", 4) == 0)
    {
        int nfav = atoi(cmd + 4);
        ChangeFavorite(nfav);
    }
    else
        std::cout << "Unknown cmd: " << cmd << std::endl;
}

int main(int argc, char **argv)
{
    TiXmlDocument xml;
    if (xml.LoadFile("remote_browser.xml"))
    {
        TiXmlElement *config = xml.FirstChildElement("config"); 
        TiXmlElement *favorites = config->FirstChildElement("favorites");
        if (favorites)
        {
            for (TiXmlElement *fav = favorites->FirstChildElement("favorite"); fav; fav = fav->NextSiblingElement("favorite"))
            {
                const char *id = fav->Attribute("id");
                const char *name = fav->Attribute("name");
                const char *path = fav->Attribute("path");
                if (!id || !name || !path)
                    continue;
                Favorite f;
                f.id = atoi(id);
                if (f.id == 0)
                    continue;
                f.name = name;
                f.path = path;
                g_favorites.push_back(f);
            }
        }
        TiXmlElement *assoc = config->FirstChildElement("file_assoc");
        if (assoc)
        {
            for (TiXmlElement *pat = assoc->FirstChildElement("pattern"); pat; pat = pat->NextSiblingElement("pattern"))
            {
                FileAssoc *assoc = NULL;
                try
                {
                    const char *match = pat->Attribute("match");
                    const char *args = pat->Attribute("command");
                    if (!match)
                        continue;

                    assoc = new FileAssoc(match, REG_EXTENDED | REG_ICASE | REG_NOSUB);
                    bool isFileArg = false;
                    if (args)
                    {
                        wordexp_t words;
                        wordexp(args, &words, 0);
                        for (size_t i = 0; i < words.we_wordc; ++i)
                        {
                            const char *txt = words.we_wordv[i];
                            assoc->args.push_back(txt);
                            if (assoc->args.back().find("$1") != std::string::npos)
                                isFileArg = true;
                        }
                        wordfree(&words);
                    }

                    if (!isFileArg)
                        assoc->args.push_back("$1");
                    g_assocs.push_back(assoc);
                }
                catch (...)
                {
                    delete assoc;
                }
            }
        }
    }

    gtk_init(&argc, &argv);

    MainWnd mainWnd;

    gtk_main();

    for (size_t i = 0; i < g_assocs.size(); ++i)
        delete g_assocs[i];

    return 0;
}
