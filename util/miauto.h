#ifndef AUTOOBJ_H_INCLUDED
#define AUTOOBJ_H_INCLUDED

namespace miutil
{

template <class AutoTraits> class AutoObject
{
public:
    typedef typename AutoTraits::type T;

    AutoObject()
        :m_obj(AutoTraits::Null())
    {
    }
    explicit AutoObject(T obj)
        :m_obj(obj)
    {
    }
    AutoObject &operator=(T obj)
    {
        Reset(obj);
        return *this;
    }
    void Reset(T obj)
    {
        if (m_obj!=obj)
        {
            Delete();
            m_obj = obj;
        }
    }
    operator T () const
    {
        return m_obj;
    }
    T Obj() const
    { 
        return m_obj;
    }
    T operator->() const
    {
        return AutoTraits::Arrow(m_obj);
    }

    T Detach()
    {
        T obj = m_obj;
        m_obj = AutoTraits::Null();
        return obj;
    }
    ~AutoObject()
    {
        Delete();
    }
    bool IsValid()
    {
        return AutoTraits::IsValid(m_obj);
    }
    void Delete()
    {
        if (AutoTraits::IsValid(m_obj))
        {
            AutoTraits::Delete(m_obj);
            m_obj = AutoTraits::Null();
        }
    }

    //AutoObject is copyable only if AutoTraits::AddRef is defined
    AutoObject(const AutoObject &o)
        :m_obj(o.m_obj)
    {
        AddRef();
    }
    AutoObject &operator=(const AutoObject &o)
    {
        if (m_obj!=o.m_obj)
        {
            Delete();
            m_obj = o.m_obj;
            AddRef();
        }
        return *this;
    }
    void AddRef()
    {
        if (AutoTraits::IsValid(m_obj))
            AutoTraits::AddRef(m_obj);
    }
private:
    T m_obj;
};

///////////////////////////////////////////////////
template <class T> struct DeletePtrTraits
{
    typedef T *type;

    static type Null()
    {
        return NULL;
    }
    static bool IsValid(type obj)
    {
        return obj!=NULL;
    }
    static void Delete(type obj)
    {
        delete obj;
    }
};

//template <class P> typedef AutoObject<DeletePtrTraits<T> > AutoPtr;
template <class P> class AutoPtr : public AutoObject<DeletePtrTraits<P> >
{
private:
    typedef AutoObject<DeletePtrTraits<P> > base_type;
public:
    AutoPtr()
    {}

    AutoPtr(typename base_type::T obj)
        :base_type(obj)
    {}
};

} //namespace miutil

#endif /* AUTOOBJ_H_INCLUDED */
