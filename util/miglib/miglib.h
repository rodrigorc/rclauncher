#ifndef MIGLIB_H_INCLUDED
#define MIGLIB_H_INCLUDED

#include <glib-object.h>
#include <stdexcept>
#include <vector>

//Define the macro below to enable the GObject type checking
//for GPtr instantiations. Usually you won't need this, as the 
//library checks the types at complile time.
//#define MIGLIB_ENABLE_CHECK_CAST

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
        error = NULL; //Stealed!
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
        Clear();
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
        //g_clear_error(&m_error);
        if (m_error)
        {
            g_error_free(m_error);
            m_error = NULL;
        }
    }
};

////////////////////////////////////////////////////

template <typename T> 
struct GPtrCast
{
protected:
    gpointer m_ptr;

    //The following are non-virtual overridables
    void Init()
    {}
    void Follow()
    {}
    void Unfollow()
    {}
public:
    operator T*() const
    {
        return static_cast<T*>(m_ptr);
    }
};

template <typename T, typename Q> 
struct CheckCast
{
    //This should check the cast only if MIGLIB_ENABLE_CHECK_CAST is defined
    static T* Cast(Q *q)
    {
        return q;
    }
    //This should always check the cast
    static T* CastChecked(Q *q)
    {
        return q;
    }
};

template<typename T> struct BorrowWrapper
{
    T *ptr;
    explicit BorrowWrapper(T *p)
        :ptr(p)
    {}
};

template<typename T> BorrowWrapper<T> Borrow(T *p)
{
    return BorrowWrapper<T>(p);
}

template <typename T, typename F, typename P=GPtrCast<T> >
class GPtr : public P
{
private:
    typedef P base;
    typedef F traits_type;

    T *Obj()
    {
        return static_cast<T*>(this->m_ptr);
    }
    T *Obj() const
    {
        return static_cast<T*>(this->m_ptr);
    }
    T *SetObj(T *ptr)
    {
        this->m_ptr = ptr;
        return ptr;
    }

public:
    typedef T original_type;
    GPtr()
    {
        SetObj(NULL);
    }
    explicit GPtr(T *ptr) //new reference
    {
        if (SetObj(ptr))
        {
            this->Init();
            this->Follow();
        }
    }
    template <typename Q> explicit GPtr(Q *ptr) //new reference
    {
        if (SetObj(CheckCast<T,Q>::CastChecked(ptr)))
        {
            this->Init();
            this->Follow();
        }
    }
    explicit GPtr(BorrowWrapper<T> ptr) //borrowed reference
    {
        if (SetObj(ptr.ptr))
        {
            traits_type::Ref(Obj());
            this->Follow();
        }
    }
    template <typename Q> explicit GPtr(BorrowWrapper<Q> ptr) //borrowed reference
    {
        if (SetObj(CheckCast<T,Q>::CastChecked(ptr.ptr)))
        {
            traits_type::Ref(Obj());
            this->Follow();
        }
    }
    GPtr(const GPtr &o)
    {
        if (SetObj(o.Obj()))
        {
            traits_type::Ref(Obj());
            this->Follow();
        }
    }
    ~GPtr()
    {
        if (Obj()) 
        {
            this->Unfollow();
            traits_type::Unref(Obj());
        }
    }
    void Reset(T *ptr = NULL) //new reference
    {
        T *prev = Obj();
        if (prev)
            this->Unfollow();
        if (SetObj(ptr))
        {
            this->Init();
            this->Follow();
        }
        if (prev)
            traits_type::Unref(prev);
    }
    template <typename Q> void Reset(Q *ptr) //new reference
    {
        T *t = CheckCast<T,Q>::CastChecked(ptr);
        Reset(t);
    }
    void Reset(BorrowWrapper<T> ptr) //borrowed reference
    {
        T *prev = Obj();
        if (prev)
            this->Unfollow();
        if (SetObj(ptr.ptr))
        {
            traits_type::Ref(Obj());
            this->Follow();
        }
        if (prev)
            traits_type::Unref(prev);
    }
    template <typename Q> void Reset(BorrowWrapper<Q> ptr) //borrowed reference
    {
        T *t = CheckCast<T,Q>::CastChecked(ptr.ptr);
        Reset(BorrowWrapper<T>(t));
    }
    const GPtr &operator=(const GPtr &o)
    {
        if (Obj()) this->Unfollow();

        if (o.Obj()) traits_type::Ref(o.Obj());
        if (Obj()) traits_type::Unref(Obj());
        if (SetObj(o.Obj()))
            this->Follow();
        return *this;
    }
    T *operator->() const
    {
        return Obj();
    }
    T *Ptr() const
    {
        return Obj();
    }
    void swap(GPtr &o)
    {
        T *prev = Obj();

        if (Obj()) this->Unfollow();
        if (o.Obj()) o.Unfollow();

        if (SetObj(o.Obj()))
            this->Follow();
        if (o.SetObj(prev))
            o.Follow();
    }
};

//Similar to Borrow.
template<typename T, typename F, typename P> BorrowWrapper<T> Cast(GPtr<T,F,P> &p)
{
    return BorrowWrapper<T>(p);
}

//Operator overload for the GPtr
template<typename T, typename P, typename F> bool operator==(const GPtr<T,F,P> &a, const GPtr<T,F,P> &b)
{
    return a.Obj() == b.Obj();
}

template<typename T, typename P, typename F> bool operator!=(const GPtr<T,F,P> &a, const GPtr<T,F,P> &b)
{
    return a.Obj() != b.Obj();
}

template<typename T, typename P, typename F> bool operator<(const GPtr<T,F,P> &a, const GPtr<T,F,P> &b)
{
    return a.Obj() < b.Obj();
}

template<typename T, typename P, typename F> void swap(GPtr<T,F,P> &a, GPtr<T,F,P> &b)
{
    a.swap(b);
}

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
        if (m_ptr)
            g_free(m_ptr);
    }
    gchar **Ptr()
    {
        if (m_ptr)
        {
            g_free(m_ptr);
            m_ptr = NULL;
        }
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
        if (m_ptr)
            g_free(m_ptr);
        m_ptr = s;
    }
    void Clear()
    {
        Reset(NULL);
    }
    bool Null() const
    {
        return m_ptr == NULL;
    }
    bool Empty() const
    {
        return m_ptr == NULL || m_ptr[0] == 0;
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

class GStrVPtr
{
private:
    gchar **m_ptr;
public:
    explicit GStrVPtr(gchar **ptr=NULL)
        :m_ptr(ptr)
    {
    }
    operator gchar**() const
    {
        return m_ptr;
    }
    ~GStrVPtr()
    {
        if (m_ptr)
            g_strfreev(m_ptr);
    }
    gchar **Detach()
    {
        gchar **t = m_ptr;
        m_ptr = NULL;
        return t;
    }
    void Reset(gchar **s = NULL)
    {
        if (m_ptr)
            g_strfreev(m_ptr);
        m_ptr = s;
    }
    void Clear()
    {
        Reset(NULL);
    }
    bool Null() const
    {
        return m_ptr == NULL;
    }
    bool Empty() const
    {
        return m_ptr == NULL || m_ptr[0] == NULL;
    }
};

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
    //Int this functions, the gpointer is the last parameter
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

#define MIGLIB_OBJECT_notify_any(src, cls, func, ptr) \
    MIGLIB_CONNECT_2(src, "notify", void, GObject*, GParamSpec*, cls, func, ptr)


class AutoSourceBase
{
public:
    AutoSourceBase()
        :m_source(0)
    {
    }
    ~AutoSourceBase()
    {
        if (m_source != 0)
            g_source_remove(m_source);
    }
    void Reset()
    {
        if (m_source != 0)
        {
            g_source_remove(m_source);
            m_source = 0;
        }
    }
    int GetSource() const
    {
        return m_source;
    }
    bool IsActive() const
    {
        return m_source != 0;
    }
private:
    AutoSourceBase(const AutoSourceBase &); //nocopy
    void operator=(const AutoSourceBase &); //nocopy

protected:
    gint m_source;
    gpointer m_data;

    //This helper struct manages the case that the source is changed from the callback
    struct GuardSource
    {
        int *psource;
        int prev;
        GuardSource(AutoSourceBase *b)
            :psource(&b->m_source), prev(*psource)
        {
        }
        gboolean Check(gboolean res)
        {
            if (!res && prev == *psource)
                *psource = 0;
            return res;
        }
    };
};

class AutoTimeout : public AutoSourceBase
{
public:
    void SetTimeout(gint timeout, GSourceFunc func, gpointer data)
    {
        if (m_source != 0)
            g_source_remove(m_source);
        m_func = func;
        m_data = data;
        m_source = g_timeout_add(timeout, OnTimeout, this);
    }
    void SetTimeoutSeconds(gint timeout, GSourceFunc func, gpointer data)
    {
        if (m_source != 0)
            g_source_remove(m_source);
        m_func = func;
        m_data = data;
        m_source = g_timeout_add_seconds(timeout, OnTimeout, this);
    }
private:
    GSourceFunc m_func;
    static gboolean OnTimeout(gpointer data)
    {
        AutoTimeout *that = static_cast<AutoTimeout*>(data);
        GuardSource guard(that);
        return guard.Check(that->m_func(that->m_data));
    }
};

class AutoIdle : public AutoSourceBase
{
public:
    void SetIdle(GSourceFunc func, gpointer data)
    {
        if (m_source != 0)
            g_source_remove(m_source);
        m_func = func;
        m_data = data;
        m_source = g_idle_add(OnIdle, this);
    }
private:
    GSourceFunc m_func;
    static gboolean OnIdle(gpointer data)
    {
        AutoIdle *that = static_cast<AutoIdle*>(data);
        GuardSource guard(that);
        return guard.Check(that->m_func(that->m_data));
    }
};

class AutoIOWatch : public AutoSourceBase
{
public:
    void SetIOWatch(GIOChannel *channel, GIOCondition cond, GIOFunc func, gpointer data)
    {
        if (m_source != 0)
            g_source_remove(m_source);
        m_func = func;
        m_data = data;
        m_source = g_io_add_watch(channel, cond, OnIOWatch, this);
    }
private:
    GIOFunc m_func;
    static gboolean OnIOWatch(GIOChannel *source, GIOCondition cond, gpointer data)
    {
        AutoIOWatch *that = static_cast<AutoIOWatch*>(data);
        GuardSource guard(that);
        return guard.Check(that->m_func(source, cond, that->m_data));
    }
};

class AutoChildWatch : public AutoSourceBase
{
public:
    void SetChildWatch(GPid pid, GChildWatchFunc func, gpointer data)
    {
        if (m_source != 0)
            g_source_remove(m_source);
        m_func = func;
        m_data = data;
        m_source = g_child_watch_add(pid, OnChildWatch, this);
    }
private:
    GChildWatchFunc m_func;
    static void OnChildWatch(GPid pid, gint status, gpointer data)
    {
        AutoChildWatch *that = static_cast<AutoChildWatch*>(data);
        GuardSource guard(that);
        that->m_func(pid, status, that->m_data);
        guard.Check(FALSE);
    }
};

//////////////////////

//The standard GObject ref-counting
struct GObjectPtrTraits
{
    static void Ref(gpointer ptr)
    { g_object_ref(ptr); }
    static void Unref(gpointer ptr)
    { g_object_unref(ptr); }
};

//No-ops ref-counting
struct GNoMagicPtrTraits
{
    static void Ref(gpointer ptr)
    { }
    static void Unref(gpointer ptr)
    { }
};


template <typename T> class GIUPtrCast : public GPtrCast<T> //For GInitiallyUnowned
{
protected:
    void Init()
    {
        g_object_ref_sink(this->m_ptr);
    }
};

//Other utility functions
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

//For specializations of GPtr with non-GObject types
#define MIGLIB_INIT_REF_UNREF_PTR_EX(x, nm, init, ref, unref) namespace miglib { \
    struct nm##PtrTraits                                        \
    {                                                           \
        static void Ref(x *ptr)                                 \
        { (ref)(ptr); }                                         \
        static void Unref(x *ptr)                               \
        { (unref)(ptr); }                                       \
    };                                                          \
    class nm##PtrCast : public GPtrCast<x>                      \
    {                                                           \
    protected:                                                  \
        void Init()                                             \
        { (init)(*this); }                                      \
    };                                                          \
    typedef GPtr<x, nm##PtrTraits, nm##PtrCast> nm##Ptr;        \
}
#define MIGLIB_INIT_REF_UNREF_PTR(x, init, ref, unref) MIGLIB_INIT_REF_UNREF_PTR_EX(x, x, init, ref, unref)

#define MIGLIB_REF_UNREF_PTR_EX(x, nm, ref, unref) namespace miglib { \
    struct nm##PtrTraits                                        \
    {                                                           \
        static void Ref(x *ptr)                                 \
        { (ref)(ptr); }                                         \
        static void Unref(x *ptr)                               \
        { (unref)(ptr); }                                       \
    };                                                          \
    typedef GPtr<x, nm##PtrTraits> nm##Ptr;                     \
}
#define MIGLIB_REF_UNREF_PTR(x, ref, unref) MIGLIB_REF_UNREF_PTR_EX(x, x, ref, unref)

#define MIGLIB_UNREF_PTR_EX(x, nm, unref) namespace miglib {    \
    struct nm##PtrTraits                                        \
    {                                                           \
        static void Unref(x *ptr)                               \
        { (unref)(ptr); }                                       \
    };                                                          \
    typedef GPtr<x, nm##PtrTraits> nm##Ptr;                     \
}
#define MIGLIB_UNREF_PTR(x, unref) MIGLIB_UNREF_PTR_EX(x, x, unref)

MIGLIB_REF_UNREF_PTR(GIOChannel, g_io_channel_ref, g_io_channel_unref)
MIGLIB_REF_UNREF_PTR(GMainLoop, g_main_loop_ref, g_main_loop_unref)
MIGLIB_REF_UNREF_PTR(GMainContext, g_main_context_ref, g_main_context_unref)
MIGLIB_INIT_REF_UNREF_PTR(GVariant, g_variant_ref_sink, g_variant_ref, g_variant_unref)
MIGLIB_UNREF_PTR(GVariantIter, g_variant_iter_free)
MIGLIB_REF_UNREF_PTR(GVariantBuilder, g_variant_builder_ref, g_variant_builder_unref)


//For the GObject hierarchy
#define MIGLIB_DECL_EX(x, nm) namespace miglib { typedef GPtr< x, GObjectPtrTraits > nm##Ptr; \
    typedef GPtr< x, GNoMagicPtrTraits > nm##PtrNM; }
#define MIGLIB_DECL(x) MIGLIB_DECL_EX(x,x)

#define MIGLIB_TYPE_CAST_MEMBER(sub)                    \
        operator sub*() const                           \
        { return static_cast<sub*>(this->m_ptr); }

#ifdef MIGLIB_ENABLE_CHECK_CAST
#define MIGLIB_CHECK_CAST(sub, type, q) (G_TYPE_CHECK_INSTANCE_CAST ((q), type, sub))
#else
#define MIGLIB_CHECK_CAST(sub, type, q) reinterpret_cast<sub*>(q)
#endif

#define MIGLIB_SUBCLASS_EX_BEGIN(sub, cls, type)         \
namespace miglib {                                      \
    template <typename Q> struct CheckCast<sub,Q>       \
    {                                                   \
        static sub *Cast(Q *q)                          \
        { return MIGLIB_CHECK_CAST(sub, type, q); }      \
        static sub *CastChecked(Q *q)                   \
        { return (G_TYPE_CHECK_INSTANCE_CAST ((q), type, sub)); } \
    };                                                  \
    template <> struct GPtrCast<sub> : GPtrCast<cls>    \
    {                                                   \
        MIGLIB_TYPE_CAST_MEMBER(sub)

#define MIGLIB_SUBCLASS_EX_END()                        \
    };                                                  \
}

//For plain subclasses of GObject

#define MIGLIB_SUBCLASS_EX(sub, cls, type)   \
    MIGLIB_SUBCLASS_EX_BEGIN(sub, cls, type) \
    MIGLIB_SUBCLASS_EX_END()

#define MIGLIB_SUBCLASS(sub, cls, type)    \
    MIGLIB_SUBCLASS_EX(sub, cls, type)     \
    MIGLIB_DECL(sub)

#define MIGLIB_SUBCLASS_ITF_1(sub, cls, itf1, type)  \
    MIGLIB_SUBCLASS_EX_BEGIN(sub, cls, type)         \
    MIGLIB_TYPE_CAST_MEMBER(itf1)                   \
    MIGLIB_SUBCLASS_EX_END()                        \
    MIGLIB_DECL(sub)

#define MIGLIB_SUBCLASS_ITF_2(sub, cls, itf1, itf2, type)  \
    MIGLIB_SUBCLASS_EX_BEGIN(sub, cls, type)         \
    MIGLIB_TYPE_CAST_MEMBER(itf1)                   \
    MIGLIB_TYPE_CAST_MEMBER(itf2)                   \
    MIGLIB_SUBCLASS_EX_END()                        \
    MIGLIB_DECL(sub)

#define MIGLIB_SUBCLASS_ITF_3(sub, cls, itf1, itf2, itf3, type)  \
    MIGLIB_SUBCLASS_EX_BEGIN(sub, cls, type)         \
    MIGLIB_TYPE_CAST_MEMBER(itf1)                   \
    MIGLIB_TYPE_CAST_MEMBER(itf2)                   \
    MIGLIB_TYPE_CAST_MEMBER(itf3)                   \
    MIGLIB_SUBCLASS_EX_END()                        \
    MIGLIB_DECL(sub)


//For subclasses of GInitiallyUnowned
#define MIGIU_DECL_EX(x, nm) namespace miglib { typedef GPtr< x, GObjectPtrTraits, GIUPtrCast<x> > nm##Ptr; \
    typedef GPtr< x, GNoMagicPtrTraits > nm##PtrNM; }
#define MIGIU_DECL(x) MIGIU_DECL_EX(x,x)

#define MIGIU_SUBCLASS(sub,cls,type) MIGLIB_SUBCLASS_EX(sub,cls,type) \
    MIGIU_DECL(sub)

//The one and only GObjectPtr is here!
//GObjects are somewhat like a sublcass of GTypeInstance
MIGLIB_SUBCLASS(GObject, GTypeInstance, G_TYPE_OBJECT)

//GInitiallyUnowned is a subclass of GObject, but they use the very 
//same struct. This is unfortunate because we cannot specialize
//both the GPtr* templates for both types (they are the same).
//So we just reuse the specializations from GObject and declare a 
//new pointer type. The only drawback is that G_TYPE_OBJECT is used 
//instead of G_TYPE_INITIALLY_UNOWNED for checking.
//MIGLIB_SUBCLASS(GInitiallyUnowned, GObject, G_TYPE_INITIALLY_UNOWNED)
MIGIU_DECL(GInitiallyUnowned)

#endif // MIGLIB_H_INCLUDED

