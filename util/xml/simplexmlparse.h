#ifndef SIMPLEXMLPARSE_H_INCLUDED
#define SIMPLEXMLPARSE_H_INCLUDED

#include "mixmlparse.h"
#include <string>
#include <vector>
#include <stdarg.h>

#ifdef __GNUC__
    #define SENTINEL __attribute__((sentinel))
#else
    #define SENTINEL 
#endif

class Simple_XML_Parser : public MiXML_Parser
{
public:
    Simple_XML_Parser()
    {
        PostReset();
    }
    typedef std::vector<std::string> attributes_t;
protected:
    enum { TAG_NONE = -2, TAG_INIT = -1 };
    void SetStateAttr(int st, ...) SENTINEL
    {
        va_list va;
        va_start(va, st);

        StateDesc *desc = Find(st, true);
        if (!desc)
            return;
        for (;;)
        {
            const char *p = va_arg(va, const char *);
            if (!p)
                break;
            desc->atts.push_back(p);
        }
        va_end(va);
    }
    void SetStateNext(int st, ...) SENTINEL
    {
        va_list va;
        va_start(va, st);

        StateDesc *desc = Find(st, true);
        if (!desc)
            return;
        for (;;)
        {
            const char *p = va_arg(va, const char *);
            if (!p)
                break;
            int next = va_arg(va, int);
            desc->next.push_back(std::make_pair(p, next));
        }
        va_end(va);
    }
    void SetStateCharData(int st, bool notify = true)
    {
        StateDesc *desc = Find(st, true);
        if (!desc)
            return;
        desc->charData = notify;
    }
    int Depth() const
    {
        return m_stack.size() - 1;
    }
    int StackState(int depth) const
    {
        int i = m_stack.size() - depth - 1;
        if (i >= 0 || i < static_cast<int>(m_stack.size()))
            return m_stack[i];
        else
            return TAG_NONE;
    }
    int State() const
    {
        return m_stack.back();
    }

//overridables
protected:
    typedef std::vector< std::pair<std::string, std::string> > extraArgs_t;
    const extraArgs_t &ExtraArgs() const
    {
        return m_extraArgs;
    }
    std::string GetExtraArg(const std::string &name, const std::string &def)
    {
        for (size_t i = 0; i < m_extraArgs.size(); ++i)
            if (m_extraArgs[i].first == name)
                return m_extraArgs[i].second;
        return def;
    }
    virtual void StartState(int state, const std::string &name, const attributes_t &atts)
    {}
    virtual void EndState(int state)
    {}
    virtual void CharDataState(int state, const char *buf, int len)
    {}

//implementation
protected:
    virtual void PostReset()
    {
        m_stack.clear();
        Push(TAG_INIT, NULL);
    }
    virtual void XML_StartElementHandler(const XML_Char *name, const XML_Char **atts)
    {
        int next = TAG_NONE;
        const StateDesc *top = Top();
        m_extraArgs.clear();
        if (top)
        {
            std::string sname(name);
            next = top->FindNext(sname);
            top = NULL;
            if (next > TAG_INIT && (top = Find(next, false)) != NULL)
            {
                attributes_t vatts(top->atts.size());
                for (size_t i = 0; atts[i]; i += 2)
                {
                    const char *aname = atts[i];
                    const char *aval = atts[i + 1];
                    for (size_t i = 0; i < top->atts.size(); ++i)
                    {
                        if (top->atts[i] == aname)
                            vatts[i] = aval;
                        else
                            m_extraArgs.push_back(std::make_pair(aname, aval));
                    }
                }
                StartState(next, sname, vatts);
            }
        }
        Push(next, top);
    }
    virtual void XML_EndElementHandler(const XML_Char *name)
    {
        int state = m_stack.back();
        if (state > TAG_INIT)
            EndState(state);
        Pop();
    }
    virtual void XML_CharacterDataHandler(const XML_Char *s, int len)
    {
        int state;
        const StateDesc *top = Top(&state);
        if (state > TAG_INIT && top && top->charData)
            CharDataState(state, s, len);
    }

private:
    struct StateDesc
    {
        std::vector< std::pair<std::string,int> > next;
        attributes_t atts;
        bool charData : 1;

        StateDesc()
            :charData(false)
        {}

        int FindNext(const std::string &s) const
        {
            for (std::vector< std::pair<std::string,int> >::const_iterator it = next.begin(); it != next.end(); ++it)
            {
                if (it->first == s)
                    return it->second;
            }
            return TAG_NONE;                
        }
    };

    StateDesc *Find(int state, bool insert)
    {
        ++state; //-1 (TAG_INIT) is mapped to m_stateMap[0], and so on

        if (state < 0)
            return NULL;
        if (state >= static_cast<int>(m_stateMap.size()))
        {
            if (!insert)
                return NULL;
            else
                m_stateMap.resize(state + 1);
        }
        return &m_stateMap[state];
    }

    const StateDesc *Top(int *st = NULL)
    {
        int state = m_stack.back();

        if (!m_top)
            m_top = Find(state, false);
        if (st)
            *st = state;
        return m_top;
    }
    void Push(int next, const StateDesc *top)
    {
        m_stack.push_back(next);
        m_top = top;
    }
    void Pop()
    {
        m_stack.pop_back();
        m_top = NULL;
    }

    std::vector<StateDesc>  m_stateMap;
    std::vector<int> m_stack;
    const StateDesc *m_top;
    extraArgs_t m_extraArgs;
};

#endif /* SIMPLEXMLPARSE_H_INCLUDED */
