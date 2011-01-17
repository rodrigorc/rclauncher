#include "config.h"
#include <lirc/lirc_client.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <string>
#include <vector>
#include <iostream>

#include "miglib/migtk.h"
#include "miglib/migtkconn.h"
#include "miglib/mipango.h"
#include "micairo.h"
#include <gdk/gdkkeysyms.h>

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
    lirc_readconfig(NULL, &m_cfg, NULL);

    m_io.Reset( g_io_channel_unix_new(m_fd) );
    g_io_channel_set_raw_nonblock(m_io, NULL);

    MIGLIB_IO_ADD_WATCH(m_io, G_IO_IN, LircClient, OnIo, this);
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

class CMainWnd : private ILircClient
{
public:
    CMainWnd();

private:
    GtkWindowPtr m_wnd;
    GtkDrawingAreaPtr m_draw;
    LircClient m_lirc;
    std::string m_cwd;
    int m_lineSel;
    std::vector<std::string> m_dirs, m_files;

    void OnDestroy(GtkObject *w)
    {
        gtk_main_quit();
    }
    gboolean OnDrawExpose(GtkWidget *w, GdkEventExpose *e);
    gboolean OnDrawKey(GtkWidget *w, GdkEventKey *e);


    void Redraw();

    //ILircClient
    virtual void OnLircCommand(const char *cmd);
};

void list_dir(const std::string &path, std::vector<std::string> &dirs, std::vector<std::string> &files)
{
    dirs.clear();
    files.clear();

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

        if (type == T_Dir)
            dirs.push_back(entry->d_name);
        else if (type == T_File)
            files.push_back(entry->d_name);
    }

    closedir(dir);

    std::sort(dirs.begin(), dirs.end());
    std::sort(files.begin(), files.end());
}

CMainWnd::CMainWnd()
    :m_lirc(this)
{
    m_wnd.Reset( gtk_window_new(GTK_WINDOW_TOPLEVEL) );
    MIGTK_OBJECT_destroy(m_wnd, CMainWnd, OnDestroy, this);

    m_draw.Reset( gtk_drawing_area_new() );
    MIGTK_WIDGET_expose_event(m_draw, CMainWnd, OnDrawExpose, this);
    gtk_widget_set_can_focus(m_draw, TRUE);
    MIGTK_WIDGET_key_press_event(m_draw, CMainWnd, OnDrawKey, this);
    GdkColor black = {0, 0x0000, 0x0000, 0x0000};
    gtk_widget_modify_bg(m_draw, GTK_STATE_NORMAL, &black);
    gtk_container_add(m_wnd, m_draw);

    gtk_window_fullscreen(m_wnd);
    gtk_widget_show_all(m_wnd);

    m_cwd = "/windows/r";
    list_dir(m_cwd, m_dirs, m_files);
    m_lineSel = 0;
}

gboolean CMainWnd::OnDrawKey(GtkWidget *w, GdkEventKey *e)
{
    std::cout << "Key: " << e->keyval << std::endl;
    switch (e->keyval)
    {
    case GDK_KEY_Up:
        Move(false);
        break;
    case GDK_KEY_Down:
        Move(true);
        break;
    }
    return TRUE;
}

void CMainWnd::Move(bool down)
{
    if (down)
    {
        ++m_lineSel;
        if (m_lineSel > m_dirs.size() + m_files.size())
            m_lineSel = m_dirs.size() + m_files.size();
    }
    else
    {
        --m_lineSel;
        if (m_lineSel < 0)
            m_lineSel = 0;
    }
    Redraw();
}

gboolean CMainWnd::OnDrawExpose(GtkWidget *w, GdkEventExpose *e)
{
    GtkAllocation size;
    gtk_widget_get_allocation(w, &size);

    std::cout << "Draw " << size.width << " - " << size.height << std::endl;

    CairoPtr cr(gdk_cairo_create(w->window));
    PangoLayoutPtr layout(pango_cairo_create_layout(cr));
    PangoFontDescriptionPtr font(pango_font_description_new());

    cairo_set_source_rgb(cr, 1, 1, 1);

    pango_font_description_set_family(font, "Ubuntu");
    pango_font_description_set_size(font, 16 * PANGO_SCALE);
    pango_layout_set_font_description(layout, font);
    pango_layout_set_ellipsize(layout, PANGO_ELLIPSIZE_MIDDLE);
    pango_layout_set_width(layout, size.width * PANGO_SCALE);
    pango_layout_set_height(layout, 0);

    pango_layout_set_text(layout, "M", 1);
    PangoRectangle baseRect;
    pango_layout_get_extents(layout, NULL, &baseRect);
    double lineH = double(baseRect.height) / PANGO_SCALE;

    double marginX1 = 50, marginX2 = 50;
    double marginY1 = 50, marginY2 = 50;

    double szH = size.height - (marginY1 + marginY2), szW = size.width - (marginX1 + marginX2);
    int nLines = int(szH / lineH);
    szH = nLines * lineH;
    marginY2 = size.height - (marginY1 + szH);

    cairo_rectangle(cr, marginX1, marginY1, szW, szH);
    cairo_stroke(cr);

    cairo_translate(cr, marginX1, marginY1);

    int nLine = 0;
    for (size_t i = 0; i < m_dirs.size() + m_files.size(); ++i)
    {
        bool isDir = i < m_dirs.size();
        const std::string &n = isDir ? m_dirs[i] : m_files[i - m_dirs.size()];

        pango_layout_set_text(layout, n.data(), n.size());
        if (nLine == m_lineSel)
        {
            cairo_set_source_rgb(cr, 1, 1, 1);
            cairo_rectangle(cr, 0, 0, szW, lineH);
            cairo_fill(cr);
            cairo_set_source_rgb(cr, 0, 0, 0);
        }
        cairo_translate(cr, 4, 0);
        pango_cairo_show_layout(cr, layout);
        cairo_translate(cr, -4, 0);
        if (nLine == m_lineSel)
            cairo_set_source_rgb(cr, 1, 1, 1);

        cairo_translate(cr, 0, lineH);
        if (++nLine >= nLines)
            break;
    }
    return TRUE;
}

void CMainWnd::Redraw()
{
    gtk_widget_queue_draw(m_draw);
}

void CMainWnd::OnLircCommand(const char *cmd)
{
    std::cout << "Cmd: " << cmd << std::endl;
}

int main(int argc, char **argv)
{
    /*
    std::vector<std::string> ds, fs;
    list_dir(".", ds, fs);
    return 0;
    */

    gtk_init(&argc, &argv);

    CMainWnd mainWnd;

    gtk_main();
    return 0;
}
