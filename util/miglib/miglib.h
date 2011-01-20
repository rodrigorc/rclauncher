#ifndef MIGLIB_H_INCLUDED
#define MIGLIB_H_INCLUDED

#include <glib-object.h>
#include <stdexcept>
#include <vector>

namespace miglib
{

class glib_error : public std::runtime_error
{
private:
    typedef std::runtime_error base_type;
    GError *m_error;
    static std::string make_str(GError *error, const char *str)
    {
        std::string res("GError");
        if (str)
        {
            res += "(";
            res += str;
            res += ")";
        }
        res += ": ";
        res += error->message;
        return res;
    }
public:
    glib_error(GError *&error, const char *str)
        :base_type(make_str(error, str)), m_error(error)
    {
        error = NULL;
    }
    glib_error(const glib_error &e)
        :base_type(e)
    {
        m_error = g_error_copy(e.m_error);
    }
    virtual ~glib_error() throw()
    {
        g_error_free(m_error);
    }
    GError *GetGError() const
    { 
        return m_error;
    }
    GQuark GetDomain() const
    { 
        return m_error->domain; 
    }
    gint GetCode() const
    {
        return m_error->code;
    }
    gchar *GetMessage() const
    {
        return m_error->message;
    }
};

inline void TestGError(GError *&error, const char *str = NULL)
{
    if (error != NULL)
        throw glib_error(error, str);
}

class GErrorPtr
{
private:
    GError *m_error;
public:
    GErrorPtr()
        :m_error(NULL)
    {}
    ~GErrorPtr()
    {
        Clear();
    }
    bool IsSet() const
    {
        return m_error != NULL;
    }
    GError **operator &()
    {
        return Ptr();
    }
    GError **Ptr()
    {
        g_clear_error(&m_error);
        return &m_error;
    }
    operator GError *()
    {
        return m_error;
    }
    GError *operator->()
    {
        return m_error;
    }
    void Test(const char *str = NULL)
    {
        TestGError(m_error, str);
    }
    void Clear()
    {
        g_clear_error(&m_error);
    }
};

////////////////////////////////////////////////////

template <typename T> struct GPtrTraits
{
    static void Ref(T *ptr)
    { g_object_ref(G_OBJECT(ptr)); }
    static void Unref(T *ptr)
    { g_object_unref(G_OBJECT(ptr)); }
};

struct GPtrFollow
{
    void FollowInit(void *&ptr)
    {}
    void Follow(void *&ptr)
    {}
    void Unfollow(void *&ptr)
    {}
};

template <typename T, typename Q> 
struct CheckCast
{
    static T* Cast(Q *q)
    {
        return q;
    }
};

template <typename T> 
struct GPtrCast
{
protected:
    void *m_ptr;
    GPtrCast(void *p)
        :m_ptr(p)
    {}
public:
    operator T*() const
    {
        return static_cast<T*>(m_ptr);
    }
    operator GTypeInstance *() const
    {
        return static_cast<GTypeInstance *>(m_ptr);
    }
};

template <typename T, typename F=GPtrTraits<T>, typename P=GPtrFollow >
class GPtr : public GPtrCast<T>, private P
{
private:
    typedef GPtrCast<T> cast_base;

    void *&Obj()
    {
        return cast_base::m_ptr;
    }
    void *Obj() const
    {
        return cast_base::m_ptr;
    }
    T *ObjT()
    {
        return static_cast<T*>(cast_base::m_ptr);
    }
    T *ObjT() const
    {
        return static_cast<T*>(cast_base::m_ptr);
    }

public:
	GPtr()
		:cast_base(NULL)
	{}
	explicit GPtr(T *ptr) //new reference
		:cast_base(ptr)
	{
        if (Obj()) this->FollowInit(Obj());
	}
	template <typename Q> explicit GPtr(Q *ptr) //new reference
		:cast_base(CheckCast<T,Q>::Cast(ptr))
	{
        if (Obj()) this->FollowInit(Obj());
	}
	GPtr(const GPtr &o)
        :cast_base(o.ObjT())
	{
        if (Obj())
        {
            F::Ref(ObjT());
            this->Follow(Obj());
        }
	}
	~GPtr()
	{
        if (Obj()) 
        {
            this->Unfollow(Obj());
            F::Unref(ObjT());
        }
	}
    void Reset(T *ptr = NULL) //new reference
    {
        if (Obj())
        {
            this->Unfollow(Obj());
            F::Unref(ObjT());
        }
        cast_base::m_ptr = ptr;
        if (Obj()) this->FollowInit(Obj());

    }
    template <typename Q> void Reset(Q *ptr) //new reference
    {
        T *t = CheckCast<T,Q>::Cast(ptr);
        Reset(t);
    }
	const GPtr &operator=(const GPtr &o)
	{
        if (Obj()) this->Unfollow(Obj());

        if (o.Obj()) F::Ref(o.ObjT());
        if (Obj()) F::Unref(ObjT());
		Obj()= o.Obj();

        if (Obj()) this->Follow(Obj());
		return *this;
	}
    T *operator->() const
    {
        return ObjT();
    }
	T *Ptr() const
	{
		return ObjT();
	}
    void swap(GPtr &o)
    {
        void *t = Obj();

        if (Obj()) this->Unfollow(Obj());
        if (o.Obj()) o.Unfollow(o.Obj());

        Obj()= o.Obj();
        o.Obj()= t;

        if (Obj()) this->Follow(Obj());
        if (o.Obj()) o.Follow(o.Obj());
    }
};

template<typename T, typename F, typename P> bool operator==(const GPtr<T,F,P> &a, const GPtr<T,F,P> &b)
{
    return a.Obj()== b.Obj();
}

template<typename T, typename F, typename P> bool operator!=(const GPtr<T,F,P> &a, const GPtr<T,F,P> &b)
{
    return a.Obj()!= b.Obj();
}

template<typename T, typename F, typename P> bool operator<(const GPtr<T,F,P> &a, const GPtr<T,F,P> &b)
{
    return a.Obj()< b.Obj();
}

template<typename T, typename F, typename P> void swap(GPtr<T,F,P> &a, GPtr<T,F,P> &b)
{
    a.swap(b);
}

typedef GPtr<GObject> GObjectPtr;

//////////////////////////////////////////////////////
// Strings

class GCharPtr
{
private:
    gchar *m_ptr;

    GCharPtr(const GCharPtr&); //nocopy
    void operator=(const GCharPtr&); //nocopy

public:
    explicit GCharPtr(gchar *s = NULL)
        :m_ptr(s)
    {
    }
    ~GCharPtr()
    {
        g_free(m_ptr);
    }
    gchar **Ptr()
    {
        g_free(m_ptr);
        m_ptr = NULL;
        return &m_ptr;
    }
    gchar **operator &()
    {
        return Ptr();
    }
    operator const gchar*() const
    {
        return m_ptr;
    }
    operator std::string() const
    {
        return m_ptr? m_ptr : "";
    }
    operator gchar*()
    {
        return m_ptr;
    }
    const gchar *c_str() const
    {
        return m_ptr;
    }
    gchar *c_str()
    {
        return m_ptr;
    }
    gchar *Detach()
    {
        gchar *t = m_ptr;
        m_ptr = NULL;
        return t;
    }
    void Reset(gchar *s = NULL)
    {
        g_free(m_ptr);
        m_ptr = s;
    }
};

class GStringPtr
{
private:
    GString *m_str;
public:
    GStringPtr(const gchar *s = "")
    {
        m_str = g_string_new(s);
        if (!m_str)
            throw std::bad_alloc();
    }
    GStringPtr(const gchar *s, gssize len)
    {
        m_str = g_string_new_len(s, len);
        if (!m_str)
            throw std::bad_alloc();
    }
    explicit GStringPtr(gsize sz)
    {
        m_str = g_string_sized_new(sz);
        if (!m_str)
            throw std::bad_alloc();
    }
    GStringPtr(const GStringPtr &s)
    {
        m_str = g_string_new_len(s.m_str->str, s.m_str->len);
    }
    GStringPtr(GString *p, bool attach)
    {
        if (attach)
            m_str = p;
        else
            m_str = g_string_new_len(p->str, p->len);
    }
    ~GStringPtr()
    {
        g_string_free(m_str, true);
    }
    const GStringPtr &operator=(const gchar *s)
    {
        g_string_assign(m_str, s);
        return *this;
    }
    const GStringPtr &operator=(const GStringPtr &s)
    {
        g_string_assign(m_str, s.m_str->str);
        return *this;
    }
    const GStringPtr &operator+=(const gchar *s)
    {
        g_string_append(m_str, s);
        return *this;
    }
    const GStringPtr &operator+=(const GStringPtr &s)
    {
        g_string_append_len(m_str, s.m_str->str, s.m_str->len);
        return *this;
    }
    const GStringPtr &operator+=(gchar c)
    {
        g_string_append_c(m_str, c);
        return *this;
    }
    const GStringPtr &operator+=(gunichar c)
    {
        g_string_append_unichar(m_str, c);
        return *this;
    }
    operator const gchar *() const
    {
        return m_str->str;
    }
    const gchar *c_str() const
    {
        return m_str->str;
    }
    gchar *c_str()
    {
        return m_str->str;
    }
    const GString *g_str() const
    {
        return m_str;
    }
    GString *g_str()
    {
        return m_str;
    }
    gsize size() const
    {
        return m_str->len;
    }
    gsize length() const
    {
        return m_str->len;
    }
};

inline bool operator==(const GStringPtr &a, const GStringPtr &b)
{
    return g_string_equal(a.g_str(), b.g_str()) != FALSE;
}

//////////////////////////////////////////////////////

class GListPtr
{
private:
	GList *m_list;

    GListPtr(const GListPtr&); //nocopy
    void operator=(const GListPtr&); //nocopy
public:
	explicit GListPtr(GList *list)
		:m_list(list)
	{}
	~GListPtr()
	{
		g_list_free(m_list);
	}
	operator GList *()
	{
		return m_list;
	}
	GList *operator->()
	{
		return m_list;
	}
};
//////////////////////////////////////////////////////

template<typename T> struct MiGLibFunctions
{
    template<typename R, R (T::*M)()> 
        static R Static0(gpointer p)
    {
        return (static_cast<T*>(p)->*M)();
    }
    template<typename R, typename A, R (T::*M)(A)> 
        static R Static1(A a, gpointer p)
    {
        return (static_cast<T*>(p)->*M)(a);
    }
    template<typename R, typename A, typename B, R (T::*M)(A,B)> 
        static R Static2(A a, B b, gpointer p)
    {
        return (static_cast<T*>(p)->*M)(a, b);
    }
    template<typename R, typename A, typename B, typename C, R (T::*M)(A,B,C)> 
        static R Static3(A a, B b, C c, gpointer p)
    {
        return (static_cast<T*>(p)->*M)(a, b, c);
    }
    template<typename R, typename A, typename B, typename C, typename D, R (T::*M)(A,B,C,D)> 
        static R Static4(A a, B b, C c, D d, gpointer p)
    {
        return (static_cast<T*>(p)->*M)(a, b, c, d);
    }
    template<typename R, typename A, typename B, typename C, typename D, typename E, R (T::*M)(A,B,C,D,E)> 
        static R Static5(A a, B b, C c, D d, E e, gpointer p)
    {
        return (static_cast<T*>(p)->*M)(a, b, c, d, e);
    }
    template<typename R, typename A, typename B, typename C, typename D, typename E, typename F, R (T::*M)(A,B,C,D,E,F)> 
        static R Static6(A a, B b, C c, D d, E e, F f, gpointer p)
    {
        return (static_cast<T*>(p)->*M)(a, b, c, d, e, f);
    }
    template<typename R, typename A, typename B, typename C, typename D, typename E, typename F, typename G, R (T::*M)(A,B,C,D,E,F,G)> 
        static R Static7(A a, B b, C c, D d, E e, F f, G g, gpointer p)
    {
        return (static_cast<T*>(p)->*M)(a, b, c, d, e, f, g);
    }
};

#define MIGLIB_FUNC_0(R, cls, func) \
    ((R (*)(gpointer))(&::miglib::MiGLibFunctions<cls>::Static0<R, &cls::func>))

#define MIGLIB_FUNC_1(R, A, cls, func) \
    ((R (*)(A,gpointer))(&::miglib::MiGLibFunctions<cls>::Static1<R, A, &cls::func>))

#define MIGLIB_FUNC_2(R, A, B, cls, func) \
    ((R (*)(A,B,gpointer))(&::miglib::MiGLibFunctions<cls>::Static2<R, A, B, &cls::func>))

#define MIGLIB_FUNC_3(R, A, B, C, cls, func) \
    ((R (*)(A,B,C,gpointer))(&::miglib::MiGLibFunctions<cls>::Static3<R, A, B, C, &cls::func>))

#define MIGLIB_FUNC_4(R, A, B, C, D, cls, func) \
    ((R (*)(A,B,C,D,gpointer))(&::miglib::MiGLibFunctions<cls>::Static4<R, A, B, C, D, &cls::func>))

#define MIGLIB_FUNC_5(R, A, B, C, D, E, cls, func) \
    ((R (*)(A,B,C,D,E,gpointer))(&::miglib::MiGLibFunctions<cls>::Static5<R, A, B, C, D, E, &cls::func>))

#define MIGLIB_FUNC_6(R, A, B, C, D, E, F, cls, func) \
    ((R (*)(A,B,C,D,E,F,gpointer))(&::miglib::MiGLibFunctions<cls>::Static6<R, A, B, C, D, E, F, &cls::func>))

#define MIGLIB_FUNC_7(R, A, B, C, D, E, F, G, cls, func) \
    ((R (*)(A,B,C,D,E,F,G,gpointer))(&::miglib::MiGLibFunctions<cls>::Static7<R, A, B, C, D, E, F, G, &cls::func>))
////

#define MIGLIB_FUNCTION(R, cls, func) \
    MIGLIB_FUNC_0(R, cls, func)

#define MIGLIB_SIGNAL_FUNC_0(R, cls, func) \
    GCallback(MIGLIB_FUNC_0(R, cls, func))

#define MIGLIB_SIGNAL_FUNC_1(R, A, cls, func) \
    GCallback(MIGLIB_FUNC_1(R, A, cls, func))

#define MIGLIB_SIGNAL_FUNC_2(R, A, B, cls, func) \
    GCallback(MIGLIB_FUNC_2(R, A, B, cls, func))

#define MIGLIB_SIGNAL_FUNC_3(R, A, B, C, cls, func) \
    GCallback(MIGLIB_FUNC_3(R, A, B, C, cls, func))

#define MIGLIB_SIGNAL_FUNC_4(R, A, B, C, D, cls, func) \
    GCallback(MIGLIB_FUNC_4(R, A, B, C, D, cls, func))

#define MIGLIB_SIGNAL_FUNC_5(R, A, B, C, D, E, cls, func) \
    GCallback(MIGLIB_FUNC_5(R, A, B, C, D, E, cls, func))

#define MIGLIB_SIGNAL_FUNC_6(R, A, B, C, D, E, F, cls, func) \
    GCallback(MIGLIB_FUNC_6(R, A, B, C, D, E, F, cls, func))

#define MIGLIB_SIGNAL_FUNC_7(R, A, B, C, D, E, F, G, cls, func) \
    GCallback(MIGLIB_FUNC_7(R, A, B, C, D, E, F, G, cls, func))

////////////////////////////////////
//Signals

#define MIGLIB_CONNECT_0(src, sig, R, cls, func, ptr) \
    g_signal_connect(G_OBJECT(src), sig, MIGLIB_SIGNAL_FUNC_0(R, cls, func), static_cast<cls*>(ptr))

#define MIGLIB_CONNECT_1(src, sig, R, A, cls, func, ptr) \
    g_signal_connect(G_OBJECT(src), sig, MIGLIB_SIGNAL_FUNC_1(R, A, cls, func), static_cast<cls*>(ptr))

#define MIGLIB_CONNECT_2(src, sig, R, A, B, cls, func, ptr) \
    g_signal_connect(G_OBJECT(src), sig, MIGLIB_SIGNAL_FUNC_2(R, A, B, cls, func), static_cast<cls*>(ptr))

#define MIGLIB_CONNECT_3(src, sig, R, A, B, C, cls, func, ptr) \
    g_signal_connect(G_OBJECT(src), sig, MIGLIB_SIGNAL_FUNC_3(R, A, B, C, cls, func), static_cast<cls*>(ptr))

#define MIGLIB_CONNECT_4(src, sig, R, A, B, C, D, cls, func, ptr) \
    g_signal_connect(G_OBJECT(src), sig, MIGLIB_SIGNAL_FUNC_4(R, A, B, C, D, cls, func), static_cast<cls*>(ptr))

#define MIGLIB_CONNECT_5(src, sig, R, A, B, C, D, E, cls, func, ptr) \
    g_signal_connect(G_OBJECT(src), sig, MIGLIB_SIGNAL_FUNC_5(R, A, B, C, D, E, cls, func), static_cast<cls*>(ptr))

#define MIGLIB_CONNECT_6(src, sig, R, A, B, C, D, E, F, cls, func, ptr) \
    g_signal_connect(G_OBJECT(src), sig, MIGLIB_SIGNAL_FUNC_6(R, A, B, C, D, E, F, cls, func), static_cast<cls*>(ptr))

#define MIGLIB_CONNECT_7(src, sig, R, A, B, C, D, E, F, G, cls, func, ptr) \
    g_signal_connect(G_OBJECT(src), sig, MIGLIB_SIGNAL_FUNC_7(R, A, B, C, D, E, F, G, cls, func), static_cast<cls*>(ptr))



/////////////////
//Other signals

#define MIGLIB_TIMEOUT_FUNC(cls, func) MIGLIB_FUNCTION(gboolean, cls, func)
#define MIGLIB_TIMEOUT_ADD(to, cls, func, ptr)  \
    g_timeout_add(to, MIGLIB_TIMEOUT_FUNC(cls, func), static_cast<cls*>(ptr))

#define MIGLIB_TIMEOUT_ADD_SECONDS(to, cls, func, ptr)  \
    g_timeout_add_seconds(to, MIGLIB_TIMEOUT_FUNC(cls, func), static_cast<cls*>(ptr))

#define MIGLIB_IDLE_FUNC(cls, func) MIGLIB_FUNCTION(gboolean, cls, func)
#define MIGLIB_IDLE_ADD(cls, func, ptr)  \
    g_idle_add(MIGLIB_IDLE_FUNC(cls, func), static_cast<cls*>(ptr))

#define MIGLIB_IO_WATCH_FUNC(cls, func) MIGLIB_FUNC_2(gboolean, GIOChannel*, GIOCondition, cls, func)
#define MIGLIB_IO_ADD_WATCH(io, cond, cls, func, ptr) \
    g_io_add_watch(io, static_cast<GIOCondition>(cond), MIGLIB_IO_WATCH_FUNC(cls, func), static_cast<cls*>(ptr))

#define MIGLIB_CHILD_WATCH_FUNC(cls, func) MIGLIB_FUNC_2(void, GPid, gint, cls, func)
#define MIGLIB_CHILD_WATCH_ADD(pid, cls, func, ptr) \
    g_child_watch_add(pid, MIGLIB_CHILD_WATCH_FUNC(cls, func), static_cast<cls*>(ptr))

#define MIGLIB_DESTROY_NOTIFY_FUNC(cls, func) MIGLIB_FUNC_0(void, cls, func)

#define MIGLIB_OBJECT_notify(src, prop, cls, func, ptr) \
    MIGLIB_CONNECT_2(src, "notify::" prop, void, GObject*, GParamSpec*, cls, func, ptr)



class AutoSource
{
public:
    guint AddIdle(GSourceFunc fun, gpointer ptr, gint priority=G_PRIORITY_DEFAULT_IDLE)
    {
        InfoSrcS *info = new InfoSrcS(this, fun, ptr);
        m_infos.push_back(info);
        info->id = g_idle_add_full(priority, 
                MIGLIB_IDLE_FUNC(InfoSrcS, Call), info,
                MIGLIB_DESTROY_NOTIFY_FUNC(InfoSrcS, OnDestroy));
        return info->id;
    }
    guint AddTimeout(gint timeout, GSourceFunc fun, gpointer ptr, gint priority=G_PRIORITY_DEFAULT)
    {
        InfoSrcS *info = new InfoSrcS(this, fun, ptr);
        m_infos.push_back(info);
        info->id = g_timeout_add_full(priority, timeout, 
                MIGLIB_TIMEOUT_FUNC(InfoSrcS, Call), info,
                MIGLIB_DESTROY_NOTIFY_FUNC(InfoSrcS, OnDestroy));
        return info->id;
    }
    guint AddTimeoutSeconds(gint timeout, GSourceFunc fun, gpointer ptr, gint priority=G_PRIORITY_DEFAULT)
    {
        InfoSrcS *info = new InfoSrcS(this, fun, ptr);
        m_infos.push_back(info);
        info->id = g_timeout_add_seconds_full(priority, timeout, 
                MIGLIB_TIMEOUT_FUNC(InfoSrcS, Call), info,
                MIGLIB_DESTROY_NOTIFY_FUNC(InfoSrcS, OnDestroy));
        return info->id;
    }
    guint AddIOWatch(GIOChannel *channel, GIOCondition cond, GIOFunc fun, gpointer ptr, gint priority=G_PRIORITY_DEFAULT)
    {
        InfoSrcIO *info = new InfoSrcIO(this, fun, ptr);
        m_infos.push_back(info);
        info->id = g_io_add_watch_full(channel, priority, cond, 
                MIGLIB_IO_WATCH_FUNC(InfoSrcIO, CallIO), info,
                MIGLIB_DESTROY_NOTIFY_FUNC(InfoSrcIO, OnDestroy));
        return info->id;
    }

    guint AddChildWatch(GPid pid, GChildWatchFunc fun, gpointer ptr, gint priority=G_PRIORITY_DEFAULT)
    {
        InfoSrcChild *info = new InfoSrcChild(this, fun, ptr);
        m_infos.push_back(info);
        info->id = g_child_watch_add_full(priority, pid, 
                MIGLIB_CHILD_WATCH_FUNC(InfoSrcChild, CallChild), info,
                MIGLIB_DESTROY_NOTIFY_FUNC(InfoSrcChild, OnDestroy));
        return info->id;
    }

    AutoSource()
    {}
    void Reset()
    {
        std::vector<InfoSrc*> infos;
        infos.swap(m_infos);
        for (size_t i=0; i<infos.size(); ++i)
            g_source_remove(infos[i]->id);
    }
    ~AutoSource()
    {
        Reset();
    }
private:
    AutoSource(const AutoSource&); //nocopy
    void operator=(const AutoSource&); //nocopy
    struct InfoSrc
    {
        guint id;
    protected:
        AutoSource *obj;

        InfoSrc(AutoSource *a)
            :id(0), obj(a)
        {
        }
        void OnDestroy()
        { //subclasses should redefine this, or would not compile, don't know why...
            for (size_t i=0; i < obj->m_infos.size(); ++i)
            {
                if (obj->m_infos[i] == this)
                {
                    obj->m_infos[i] = obj->m_infos.back();
                    obj->m_infos.pop_back();
                    break;                    
                }
            }
            //subclasses should do "delete this", it is not here to avoid the virtual destructor
        }
    };
    std::vector<InfoSrc*> m_infos;

    struct InfoSrcS : public InfoSrc
    {
        GSourceFunc fun;
        gpointer data;

        InfoSrcS(AutoSource *a, GSourceFunc f, gpointer d)
            :InfoSrc(a), fun(f), data(d)
        {
        }
        void OnDestroy()
        {
            InfoSrc::OnDestroy();
            delete this;
        }
        gboolean Call()
        {
            return fun(data);
        }
    };
    struct InfoSrcIO : public InfoSrc
    {
        GIOFunc fun;
        gpointer data;

        InfoSrcIO(AutoSource *a, GIOFunc f, gpointer d)
            :InfoSrc(a), fun(f), data(d)
        {
        }
        void OnDestroy()
        {
            InfoSrc::OnDestroy();
            delete this;
        }
        gboolean CallIO(GIOChannel *source, GIOCondition cond)
        {
            return fun(source, cond, data);
        }
    };
    struct InfoSrcChild : public InfoSrc
    {
        GChildWatchFunc fun;
        gpointer data;

        InfoSrcChild(AutoSource *a, GChildWatchFunc f, gpointer d)
            :InfoSrc(a), fun(f), data(d)
        {
        }
        void OnDestroy()
        {
            InfoSrc::OnDestroy();
            delete this;
        }
        void CallChild(GPid pid, gint status)
        {
            fun(pid, status, data);
        }
    };
};

//////////////////////////////////////////////
//GIOChannels (not a GObject!!!)
template<> struct GPtrTraits<GIOChannel>
{
    static void Ref(GIOChannel *ptr)
    { g_io_channel_ref(ptr); }
    static void Unref(GIOChannel *ptr)
    { g_io_channel_unref(ptr); }
};

typedef GPtr<GIOChannel> GIOChannelPtr;

inline void g_io_channel_set_raw_nonblock(GIOChannel *io, GError **error = NULL)
{
    if (g_io_channel_set_encoding(io, NULL, error) == G_IO_STATUS_ERROR)
        return;
    g_io_channel_set_buffered(io, FALSE);
    GIOFlags ioflags = GIOFlags(g_io_channel_get_flags(io));
    if (g_io_channel_set_flags(io, GIOFlags(ioflags | G_IO_FLAG_NONBLOCK), error) == G_IO_STATUS_ERROR)
        return;
}

} //namespace miglib

#define MIGLIB_DECL_EX(x, nm) namespace miglib { typedef GPtr< x > nm##Ptr; }
#define MIGLIB_DECL(x) MIGLIB_DECL_EX(x,x)

#define MIGLIB_TYPE_CAST_MEMBER(sub)                    \
        operator sub*() const                           \
        { return static_cast<sub*>(this->m_ptr); }

#define MIGLIB_SUBCLASS_EX_BEGIN(sub, cls, chk)         \
namespace miglib {                                      \
    template <typename Q> struct CheckCast<sub,Q>       \
    {                                                   \
        static sub *Cast(Q *q)                          \
        { return chk(q); }                              \
    };                                                  \
    template <> struct GPtrCast<sub> : GPtrCast<cls>    \
    {                                                   \
        typedef GPtrCast<cls> base_type;                \
        GPtrCast<sub>(void *p)                          \
            :base_type(p)                               \
        {}                                              \
        MIGLIB_TYPE_CAST_MEMBER(sub)

#define MIGLIB_SUBCLASS_EX_END()                        \
    };                                                  \
}

#define MIGLIB_SUBCLASS_EX(sub, cls, chk)   \
    MIGLIB_SUBCLASS_EX_BEGIN(sub, cls, chk) \
    MIGLIB_SUBCLASS_EX_END()

#define MIGLIB_SUBCLASS(sub, cls, chk)    \
    MIGLIB_SUBCLASS_EX(sub, cls, chk)     \
    MIGLIB_DECL(sub)

#define MIGLIB_SUBCLASS_ITF_1(sub, cls, itf1, chk)  \
    MIGLIB_SUBCLASS_EX_BEGIN(sub, cls, chk)         \
    MIGLIB_TYPE_CAST_MEMBER(itf1)                   \
    MIGLIB_SUBCLASS_EX_END()                        \
    MIGLIB_DECL(sub)

#define MIGLIB_SUBCLASS_ITF_2(sub, cls, itf1, itf2, chk)  \
    MIGLIB_SUBCLASS_EX_BEGIN(sub, cls, chk)         \
    MIGLIB_TYPE_CAST_MEMBER(itf1)                   \
    MIGLIB_TYPE_CAST_MEMBER(itf2)                   \
    MIGLIB_SUBCLASS_EX_END()                        \
    MIGLIB_DECL(sub)

#define MIGLIB_SUBCLASS_ITF_3(sub, cls, itf1, itf2, itf3, chk)  \
    MIGLIB_SUBCLASS_EX_BEGIN(sub, cls, chk)         \
    MIGLIB_TYPE_CAST_MEMBER(itf1)                   \
    MIGLIB_TYPE_CAST_MEMBER(itf2)                   \
    MIGLIB_TYPE_CAST_MEMBER(itf3)                   \
    MIGLIB_SUBCLASS_EX_END()                        \
    MIGLIB_DECL(sub)

#endif // MIGLIB_H_INCLUDED

