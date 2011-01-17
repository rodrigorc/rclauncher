#include "config.h"
#include <lirc/lirc_client.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>

#include <regex.h>

#include <string>
#include <vector>
#include <iostream>

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
    m_fd = lirc_init("mplayer", 1);

    m_io.Reset( g_io_channel_unix_new(m_fd) );
    g_io_channel_set_raw_nonblock(m_io, NULL);

    MIGLIB_IO_ADD_WATCH(m_io, G_IO_IN, LircClient, OnIo, this);

    lirc_readconfig(NULL, &m_cfg, NULL);
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
        std::cout << "Code: " << code << std::endl;
        while (lirc_code2char(m_cfg, code, &cmd) == 0 && cmd != NULL)
        {
            m_cli->OnLircCommand(cmd);
        }
	free(code);
    }
    return TRUE;
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
        if (name[0] == '.')
            continue; //hidden?
        if (name == "." || name == "..")
            continue;

        if (type == T_Dir)
            files.push_back(DirEntry(name, true));
        else if (type == T_File)
            files.push_back(DirEntry(name, false));
    }

    closedir(dir);

    std::sort(files.begin(), files.end());
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
    std::string cmd;
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

    void OnDestroy(GtkObject *w)
    {
        gtk_main_quit();
    }
    gboolean OnDrawExpose(GtkWidget *w, GdkEventExpose *e);
    gboolean OnDrawKey(GtkWidget *w, GdkEventKey *e);
    void Move(bool down);
    void Select();
    void ChangePath(const std::string &path);
    void Open(const std::string &path);
    void OnChildWatch(GPid pid, gint status);

    void Redraw();

    //ILircClient
    virtual void OnLircCommand(const char *cmd);
};


MainWnd::MainWnd()
    :m_lirc(this), m_childPid(0)
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

    //gtk_window_fullscreen(m_wnd);
    gtk_widget_show_all(m_wnd);

    ChangePath("/home/rodrigo");
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
    case GDK_KEY_Return:
    case GDK_KEY_KP_Enter:
        Select();
        break;
    default:
        std::cout << "Key: " << std::hex << e->keyval << std::dec << std::endl;
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

void MainWnd::Select()
{
    if (m_lineSel < 0 || m_lineSel >= static_cast<int>(m_files.size()))
        return;
    const DirEntry &entry = m_files[m_lineSel];
    if (entry.isDir)
    {
        if (entry.name == "..")
        {
            size_t slash = m_cwd.rfind('/');
            if (slash != std::string::npos)
                ChangePath(m_cwd.substr(0, slash));
            else
                ChangePath("/");
        }
        else
        {
            ChangePath(m_cwd + "/" + entry.name);
        }
    }
    else
    {
        Open(entry.name);
    }
}

void MainWnd::ChangePath(const std::string &path)
{
    m_cwd = path;
    if (m_cwd.empty())
        m_cwd = "/";

    while (m_cwd.size() > 1 && m_cwd[m_cwd.size() - 1] == '/')
        m_cwd = m_cwd.substr(0, m_cwd.size() - 1);

    list_dir(m_cwd, m_files);
    m_lineSel = m_firstLine = 0;
    Redraw();
}

void MainWnd::Open(const std::string &path)
{
    std::string fullPath = m_cwd + "/" + path;
    std::cout << "Run " << fullPath << std::endl;

    FileAssoc *assoc = MatchFile(path);
    if (!assoc)
    {
        std::cout << "No assoc!"<< std::endl;
        return;
    }

    std::cout << "Assoc: "<< assoc->cmd << std::endl;

    std::vector<const char *> args;
    args.push_back(assoc->cmd.c_str());
    for (size_t i = 0; i < assoc->args.size(); ++i)
    {
        const std::string &arg = assoc->args[i];
        if (arg == "$1")
            args.push_back(fullPath.c_str());
        else
            args.push_back(arg.c_str());
    }
    args.push_back(NULL);
    if (g_spawn_async(NULL, const_cast<char**>(args.data()), NULL, GSpawnFlags(G_SPAWN_SEARCH_PATH | G_SPAWN_DO_NOT_REAP_CHILD), NULL, NULL, &m_childPid, NULL))
    {
        MIGLIB_CHILD_WATCH_ADD(m_childPid, MainWnd, OnChildWatch, this);
        m_childText = path;
    }
}

void MainWnd::OnChildWatch(GPid pid, gint status)
{
    std::cout << "Finish " << pid << ": " << status << std::endl;
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
    PangoLayoutPtr layout(pango_cairo_create_layout(cr));
    pango_layout_set_ellipsize(layout, PANGO_ELLIPSIZE_MIDDLE);

    PangoFontDescriptionPtr font(pango_font_description_new());
    pango_font_description_set_family(font, "Ubuntu");
    pango_font_description_set_size(font, 12 * PANGO_SCALE);

    PangoFontDescriptionPtr fontTitle(pango_font_description_new());
    pango_font_description_set_family(fontTitle, "Ubuntu");
    pango_font_description_set_size(fontTitle, 24 * PANGO_SCALE);
    
    pango_layout_set_text(layout, "M", 1);
    PangoRectangle baseRect;

    pango_layout_set_font_description(layout, fontTitle);
    pango_layout_get_extents(layout, NULL, &baseRect);
    double titleH = double(baseRect.height) / PANGO_SCALE;

    pango_layout_set_font_description(layout, font);
    pango_layout_get_extents(layout, NULL, &baseRect);
    double lineH = double(baseRect.height) / PANGO_SCALE;

    double marginX1 = 50, marginX2 = 50;
    double marginY1 = 50, marginY2 = 50;
    double scrollW = 15;

    double szH = size.height - (marginY1 + marginY2), szW = size.width - (marginX1 + marginX2 + scrollW);
    int nLines = int(szH / lineH);
    szH = nLines * lineH;
    marginY2 = size.height - (marginY1 + szH);

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_translate(cr, marginX1, marginY1);

    cairo_matrix_t matrix;
    cairo_get_matrix(cr, &matrix);

    const double DELTA_X = 20;
    pango_layout_set_width(layout, (szW - DELTA_X) * PANGO_SCALE);
    pango_layout_set_height(layout, 0);

    if (m_lineSel < m_firstLine)
        m_firstLine = m_lineSel;
    if (m_lineSel >= m_firstLine + nLines)
        m_firstLine = m_lineSel - nLines + 1;

    if (static_cast<int>(m_files.size()) > nLines)
    {
        double thumbY = m_firstLine * szH / m_files.size();
        double thumbH = nLines * szH / m_files.size();
        cairo_rectangle(cr, szW, thumbY, scrollW, thumbH);
        cairo_fill(cr);
    }
    else
    {
        cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
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
        cairo_translate(cr, DELTA_X, 0);
        pango_cairo_show_layout(cr, layout);
        cairo_translate(cr, -DELTA_X, 0);
        if (static_cast<int>(nLine) == m_lineSel)
            cairo_set_source_rgb(cr, 1, 1, 1);

        cairo_translate(cr, 0, lineH);
    }

    pango_layout_set_font_description(layout, fontTitle);
    pango_layout_set_width(layout, (szW + scrollW) * PANGO_SCALE);

    pango_layout_set_text(layout, m_cwd.data(), m_cwd.size());
    cairo_set_matrix(cr, &matrix);
    cairo_translate(cr, 0, -titleH - 4);
    pango_cairo_show_layout(cr, layout);


    //*******************************
    if (m_childPid != 0)
    {
        pango_layout_set_text(layout, m_childText.data(), m_childText.size());
        pango_layout_set_width(layout, -1);
        pango_layout_get_extents(layout, NULL, &baseRect);

        double marginChildW = 25, marginChildH = 25;
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

        cairo_set_matrix(cr, &matrix);
        cairo_translate(cr, (szW + scrollW - childW) / 2, (szH - childH) / 2);
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
    std::cout << "Cmd: " << cmd << std::endl;
}

int main(int argc, char **argv)
{
    TiXmlDocument xml;
    if (xml.LoadFile("remote_browser.rc"))
    {
        TiXmlElement *assoc = xml.FirstChildElement("file_assoc");
        if (assoc)
        {
            for (TiXmlElement *pat = assoc->FirstChildElement("pattern"); pat; pat = pat->NextSiblingElement("pattern"))
            {
                FileAssoc *assoc = NULL;
                try
                {
                    const char *match = pat->Attribute("match");
                    const char *cmd = pat->Attribute("command");
                    if (!match || !cmd)
                        continue;

                    assoc = new FileAssoc(match, REG_EXTENDED | REG_ICASE | REG_NOSUB);
                    assoc->cmd = cmd;
                    bool isFileArg = false;
                    for (TiXmlElement *arg = pat->FirstChildElement("arg"); arg; arg = arg->NextSiblingElement("arg"))
                    {
                        TiXmlText *targ = arg->FirstChild()->ToText();
                        if (!targ)
                            continue;
                        const char *txt = targ->Value();
                        assoc->args.push_back(txt);
                        if (assoc->args.back().find("$1") != std::string::npos)
                            isFileArg = true;
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
/*
    RegEx re("\\.txt$", REG_EXTENDED | REG_ICASE | REG_NOSUB);
    int r = regexec(re, argv[1], 0, NULL, 0);
    std::cout << "regexec " << r << std::endl;
    return 0;
*/
    gtk_init(&argc, &argv);

    MainWnd mainWnd;

    gtk_main();

    return 0;
}
