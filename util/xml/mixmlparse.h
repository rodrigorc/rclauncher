#ifndef MIXMLPARSE_H_INCLUDED
#define MIXMLPARSE_H_INCLUDED

#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <expat.h>
#include <errno.h>

class xml_error : public std::runtime_error
{
public:
    xml_error(const std::string &e)
        :std::runtime_error(e)
    {}
    xml_error(XML_Parser parser)
        :std::runtime_error(make_str(parser))
    {}
private:
    std::string make_str(XML_Parser parser)
    {
        XML_Error error = XML_GetErrorCode(parser);
        std::ostringstream os;
        os << "XML_Parser: ";
        os << XML_GetCurrentLineNumber(parser) << ":" << XML_GetCurrentColumnNumber(parser) << ": " << XML_ErrorString(error);
        return os.str();
    }

};

class MiXML_Parser
{
public:
    void TestXMLError(XML_Status status)
    {
        if (status != XML_STATUS_ERROR)
            return;

        throw xml_error(m_parser);
    }
    explicit MiXML_Parser(const XML_Char *encoding=0)
    {
        m_parser = XML_ParserCreate(encoding);
        InitPriv();
    }
    explicit MiXML_Parser(const XML_Char *encoding, XML_Char namespaceSeparator)
    {
        m_parser = XML_ParserCreateNS(encoding, namespaceSeparator);
        InitPriv();
    }
    virtual ~MiXML_Parser()
    {
        XML_ParserFree(m_parser);
    }
    void Reset(const XML_Char *encoding=0)
    {
        if (XML_ParserReset(m_parser, encoding)==XML_FALSE)
            TestXMLError(XML_STATUS_ERROR);
        InitPriv();
        PostReset();
    }

    operator XML_Parser() const
    { return m_parser; }

    XML_Status Parse(std::istream &is, int bufLen = 1024*16)
    {
        while (is)
        {
            void *ptr = XML_GetBuffer(m_parser, bufLen);
            if (!ptr)
                TestXMLError(XML_STATUS_ERROR);

            is.read(static_cast<char*>(ptr), bufLen);
            int bytes = is.gcount();
            bool end = is.eof();

            XML_Status res = XML_ParseBuffer(m_parser, bytes, end);
            TestXMLError(res);

            if (end || res==XML_STATUS_SUSPENDED)
                return res;
        }
        return XML_STATUS_ERROR;
    }
    XML_Status Parse(const void *ptr, int len, bool isFinal=true)
    {
        XML_Status res = XML_Parse(m_parser, static_cast<const char *>(ptr), len, isFinal);
        TestXMLError(res);
        return res;
    }
    XML_Status Parse(const char *str, bool isFinal=true)
    {
        return Parse(str, strlen(str), isFinal);
    }
    XML_Status Parse(const std::string &str, bool isFinal=true)
    {
        return Parse(str.c_str(), str.size(), isFinal);
    }
    XML_Status ParseFile(const char *str, int bufLen = 1024*16)
    {
        std::ifstream ifs(str);
        if (!ifs)
            throw xml_error(std::string(str) + ": " + strerror(errno));
        return Parse(ifs, bufLen);
    }
    XML_Status ParseFile(const std::string &str, int bufLen = 1024*16)
    {
        return ParseFile(str.c_str(), bufLen);
    }
protected:
    virtual void PostReset()
    {}

    virtual void XML_StartElementHandler(const XML_Char *name, const XML_Char **atts)
    {}
    virtual void XML_EndElementHandler(const XML_Char *name)
    {}
    virtual void XML_CharacterDataHandler(const XML_Char *s, int len)
    {}

private:
    MiXML_Parser(MiXML_Parser &); // nocopy
    void operator=(MiXML_Parser &); // nocopy

    XML_Parser m_parser;

    static void XMLCALL St_StartElementHandler(void *userData,
                                   const XML_Char *name,
                                   const XML_Char **atts)
    { GetPtr(userData)->XML_StartElementHandler(name, atts); }
    static void XMLCALL St_EndElementHandler(void *userData,
                                   const XML_Char *name)
    { GetPtr(userData)->XML_EndElementHandler(name); }
    static void XMLCALL St_CharacterDataHandler(void *userData,
                                    const XML_Char *s,
                                    int len)
    { GetPtr(userData)->XML_CharacterDataHandler(s, len); }

    void InitPriv()
    {
        if (!m_parser)
            throw xml_error("Unknown XML_Parser error");

        XML_SetUserData(m_parser, this);

        XML_SetElementHandler(m_parser, St_StartElementHandler, St_EndElementHandler);
        XML_SetCharacterDataHandler(m_parser, St_CharacterDataHandler);
    }
    static MiXML_Parser *GetPtr(void *p)
    {
        return static_cast<MiXML_Parser *>(p);
    }
};


#endif //MIXMLPARSE_H_INCLUDED

