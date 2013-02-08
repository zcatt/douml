
#include "FileIn.h"
#include "Token.h"

#include "UmlCom.h"
//Added by qt3to4:
#include "misc/mystr.h"
FileIn::FileIn(const QString & path, FILE * fp)
    : _path((const char *)path), _fp(fp), _utf8(false), _linenum(1), _length(1024)  //[rageek] ambiguous, cast *
{
    _buffer = new char[_length];

    _special_chars["amp"] = '&';
    _special_chars["lt"] = '<';
    _special_chars["gt"] = '>';
    _special_chars["quot"] = '"';
    _special_chars["apos"] = '\'';

    _special_chars["euro"] = (char) 128;
    _special_chars["oelig"] = (char) 156;
    _special_chars["Yuml"] = (char) 159;
    _special_chars["nbsp"] = (char) 160;
    _special_chars["iexcl"] = (char) 161;
    _special_chars["cent"] = (char) 162;
    _special_chars["pound"] = (char) 163;
    _special_chars["curren"] = (char) 164;
    _special_chars["ye"] = (char) 165;
    _special_chars["brvbar"] = (char) 166;
    _special_chars["sect"] = (char) 167;
    _special_chars["uml"] = (char) 168;
    _special_chars["copy;  "] = (char) 169;
    _special_chars["ordf"] = (char) 170;
    _special_chars["laquo"] = (char) 171;
    _special_chars["not"] = (char) 172;
    _special_chars["shy"] = (char) 173;
    _special_chars["reg"] = (char) 174;
    _special_chars["masr"] = (char) 175;
    _special_chars["deg"] = (char) 176;
    _special_chars["plusmn"] = (char) 177;
    _special_chars["sup2"] = (char) 178;
    _special_chars["sup3"] = (char) 179;
    _special_chars["acute"] = (char) 180;
    _special_chars["micro"] = (char) 181;
    _special_chars["para"] = (char) 182;
    _special_chars["middot"] = (char) 183;
    _special_chars["cedil"] = (char) 184;
    _special_chars["sup1"] = (char) 185;
    _special_chars["ordm"] = (char) 186;
    _special_chars["raquo"] = (char) 187;
    _special_chars["frac14"] = (char) 188;
    _special_chars["frac12"] = (char) 189;
    _special_chars["frac34"] = (char) 190;
    _special_chars["iquest"] = (char) 191;
    _special_chars["Agrave"] = (char) 192;
    _special_chars["Aacute"] = (char) 193;
    _special_chars["Acirc"] = (char) 194;
    _special_chars["Atilde"] = (char) 195;
    _special_chars["Auml"] = (char) 196;
    _special_chars["Aring"] = (char) 197;
    _special_chars["Aelig"] = (char) 198;
    _special_chars["Ccedil"] = (char) 199;
    _special_chars["Egrave"] = (char) 200;
    _special_chars["Eacute"] = (char) 201;
    _special_chars["Ecirc"] = (char) 202;
    _special_chars["Euml"] = (char) 203;
    _special_chars["Igrave"] = (char) 204;
    _special_chars["Iacute"] = (char) 205;
    _special_chars["Icirc"] = (char) 206;
    _special_chars["Iuml"] = (char) 207;
    _special_chars["eth"] = (char) 208;
    _special_chars["Ntilde"] = (char) 209;
    _special_chars["Ograve"] = (char) 210;
    _special_chars["Oacute"] = (char) 211;
    _special_chars["Ocirc"] = (char) 212;
    _special_chars["Otilde"] = (char) 213;
    _special_chars["Ouml"] = (char) 214;
    _special_chars["times"] = (char) 215;
    _special_chars["Oslash"] = (char) 216;
    _special_chars["Ugrave"] = (char) 217;
    _special_chars["Uacute"] = (char) 218;
    _special_chars["Ucirc"] = (char) 219;
    _special_chars["Uuml"] = (char) 220;
    _special_chars["Yacute"] = (char) 221;
    _special_chars["thorn"] = (char) 222;
    _special_chars["szlig"] = (char) 223;
    _special_chars["agrave"] = (char) 224;
    _special_chars["aacute"] = (char) 225;
    _special_chars["acirc"] = (char) 226;
    _special_chars["atilde"] = (char) 227;
    _special_chars["auml"] = (char) 228;
    _special_chars["aring"] = (char) 229;
    _special_chars["aelig"] = (char) 230;
    _special_chars["ccedil"] = (char) 231;
    _special_chars["egrave"] = (char) 232;
    _special_chars["eacute"] = (char) 233;
    _special_chars["ecirc"] = (char) 234;
    _special_chars["euml"] = (char) 235;
    _special_chars["igrave"] = (char) 236;
    _special_chars["iacute"] = (char) 237;
    _special_chars["icirc"] = (char) 238;
    _special_chars["iuml"] = (char) 239;
    _special_chars["eth"] = (char) 240;
    _special_chars["ntilde"] = (char) 241;
    _special_chars["ograve"] = (char) 242;
    _special_chars["oacute"] = (char) 243;
    _special_chars["ocirc"] = (char) 244;
    _special_chars["otilde"] = (char) 245;
    _special_chars["ouml"] = (char) 246;
    _special_chars["divide"] = (char) 247;
    _special_chars["oslash"] = (char) 248;
    _special_chars["ugrave"] = (char) 249;
    _special_chars["uacute"] = (char) 250;
    _special_chars["ucirc"] = (char) 251;
    _special_chars["uuml"] = (char) 252;
    _special_chars["yacute"] = (char) 253;
    _special_chars["thorn"] = (char) 254;
    _special_chars["yuml"] = (char) 255;
}

FileIn::~FileIn()
{
    fclose(_fp);
}

Token & FileIn::read(bool any)
{
    static Token token;

    token.read(*this, any);
    return token;
}

WrapperStr FileIn::body(WrapperStr what)
{
    WrapperStr r;
    int index = 0;
    int c;

    for (;;) {
        c = fgetc(_fp);

        if (c == EOF)
            error("premature end of file");

        if (c == '<') {
            _buffer[index] = 0;
            r += _buffer;

            ungetc(c, _fp);

            Token & token = read(FALSE);

            if (!token.close(what))
                error("'&lt;/" + what + "&gt;' expected rather than '&lt;/" +
                      token.what() + "&gt;'");

            return r;
        }

        if (c == '&')
            c = read_special_char();
        else if (c == '\n')
            _linenum += 1;

        if ((index + 1) == _length) {
            _buffer[index] = 0;
            index = 0;
            r += _buffer;
        }

        _buffer[index++] = c;
    }
}

const char * FileIn::readWord(bool any, BooL & str)
{
    int c;

    // bypass comment
    for (;;) {
        c = fgetc(_fp);

        if (c == EOF)
            // doesn't return
            error("premature end of file");

        if (c == '\n')
            _linenum += 1;
        else if ((c != " ") && (c != '\t') && (c != '\r'))
            break;
    }

    switch (c) {
    case '<':
    case '>':
    case '/':
    case '=':
    case '?':
    case '-':
    case '!':
        _buffer[0] = c;
        _buffer[1] = 0;
        str = FALSE;
        return _buffer;

    case '"':
    case '\'':
        str = TRUE;
        return read_string(c);

    default:
        str = FALSE;
        return read_word(c, any);
    }


}

void FileIn::finish(WrapperStr what)
{
    for (;;) {
        Token & tk = read(TRUE);

        if (tk.close(what))
            return;
        else if (tk.close())
            error("'&lt;/" + tk.what() + "&gt;' while wait for '&lt;/" + what + "&gt;'");
        else {
            WrapperStr s = tk.xmiId();

            if (! s.isEmpty())
                BypassedIds.insert(QString(s), "");

            if (! tk.closed())
                finish(tk.what());
        }
    }
}

void FileIn::bypass(Token & tk)
{
    static Q3Dict<char> bypassed;

    WrapperStr s = tk.xmiType();

    if (s.isEmpty()) {
        QString k = QString(tk.what());

        if (bypassed[k] == 0) {
            warning("bypass &lt;" + tk.what() + "...&gt; (other cases not signaled)");
            bypassed.insert(k, "");
        }
    }
    else {
        QString k = QString(tk.what()) + " " + QString(s);

        if (bypassed[k] == 0) {
            warning("bypass &lt;" + tk.what() +
                    " xmi:type=\"" + s + "\"...&gt; (other cases not signaled)");
            bypassed.insert(k, "");
        }
    }

    s = tk.xmiId();

    if (! s.isEmpty())
        BypassedIds.insert(QString(s), "");

    if (! tk.closed())
        finish(tk.what());

}

void FileIn::bypassedId(Token & tk)
{
    WrapperStr s = tk.xmiId();

    if (! s.isEmpty())
        BypassedIds.insert(QString(s), "");

}

void FileIn::error(WrapperStr s)
{
    WrapperStr num;
    WrapperStr err = WrapperStr("error in ") + _path + " line " +
                    num.setNum(_linenum) + " : " + s + "<br>";

    UmlCom::trace(err);
    throw 0;
}

void FileIn::warning(WrapperStr s)
{
    WrapperStr num;
    WrapperStr warn = WrapperStr("warning in ") + _path + " line " +
                     num.setNum(_linenum) + " : " + s + "<br>";

    UmlCom::trace(warn);

}

Q3Dict<char> FileIn::BypassedIds;

const char * FileIn::read_word(int c, bool any)
{
    if (((c >= 'a') && (c <= 'z')) ||
        ((c >= '0') && (c <= '9')) ||
        (c == '_'))
        _buffer[0] = c;
    else if ((c >= 'A') && (c <= 'Z'))
        _buffer[0] = c - 'A' + 'a';
    else if (any) {
        _buffer[0] = c;
        _buffer[1] = 0;
        return _buffer;
    }
    else {
        // doesn't return
        char cs[2];

        cs[0] = c;
        cs[1] = 0;

        error("unexpected character '" + WrapperStr(cs) + "'");
    }

    int index = 1;

    for (;;) {
        c = fgetc(_fp);

        if (c == EOF)
            error("premature end of file");

        if ((c >= 'A') && (c <= 'Z'))
            c = c - 'A' + 'a';
        else if ((c != ':') && (c != '_') &&
                 ((c < 'a') || (c > 'z')) &&
                 ((c < '0') || (c > '9'))) {
            // may add 0 without size check
            _buffer[index] = 0;
            ungetc(c, _fp);
            return _buffer;
        }

        if ((index + 1) == _length) {
            // can't add c then 0
            char * b = new char[_length + 1024];

            memcpy(b, _buffer, index);
            delete [] _buffer;
            _buffer = b;
            _length += 1024;
        }

        _buffer[index++] = c;
    }
}

const char * FileIn::read_string(int marker)
{
    // " already read
    int index = 0;
    int c;

    for (;;) {
        c = fgetc(_fp);

        switch (c) {
        case EOF:
            // doesn't return
            error("premature end of file");
            break;

        case '&':
            // special char
            c = read_special_char();
            break;

        case '\n':
            _linenum += 1;
            break;

        default:
            if (c == marker) {
                // can add 0 without size check
                _buffer[index] = 0;
                return _buffer;
            }

            if (_utf8 && (((unsigned char) c) > 127))
                c = ((c & 3) << 6) + (fgetc(_fp) & 0x3f);

            break;
        }

        if ((index + 1) == _length) {
            // can't add c then 0
            char * b = new char[_length + 1024];

            memcpy(b, _buffer, index);
            delete [] _buffer;
            _buffer = b;
            _length += 1024;
        }

        _buffer[index++] = c;
    }
}

char FileIn::read_special_char()
{
    // & already read
    int c = fgetc(_fp);

    if (c == EOF)
        // doesn't return
        error("premature end of file");

    if (c == '#') {
        int c = fgetc(_fp);
        int r = 0;

        if ((c == 'x') || (c == 'X')) {
            while ((c = fgetc(_fp)) != ';') {
                if (c == EOF)
                    // doesn't return
                    error("premature end of file");

                if ((c >= '0') && (c <= '9'))
                    r = r * 16 + c - '0';
                else if ((c >= 'A') && (c <= 'F'))
                    r = r * 16 + c - 'A' + 10;
                else if ((c >= 'a') && (c <= 'f'))
                    r = r * 16 + c - 'a' + 10;
                else
                    // doesn't return
                    error("not a valid special character");
            }
        }
        else {
            while (c != ';') {
                if (c == EOF)
                    // doesn't return
                    error("premature end of file");

                if ((c >= '0') && (c <= '9'))
                    r = r * 10 + c - '0';
                else
                    // doesn't return
                    error("not a valid special character");

                c = fgetc(_fp);
            }
        }

        return r;
    }
    else {
        int index = 0;
        char s[16];

        while (c != ';') {
            if (index == sizeof(s) - 2)
                // doesn't return
                error("not a valid special character");

            s[index++] = c;
            c = fgetc(_fp);

            if (c == EOF)
                // doesn't return
                error("premature end of file");
        }

        s[index] = 0; // check on index useless

        QMap<WrapperStr, char>::ConstIterator iter = _special_chars.find(s);

        if (iter == _special_chars.end())
            // doesn't return
            error("not a valid special character");

        return iter.data();
    }
}

void FileIn::setEncoding(WrapperStr s)
{
    if (s.left(3).lower() == "utf") {
        if (s.right(1) != "8") {
            UmlCom::trace("sorry, in the UTF encoding, only UTF-8 is managed");
            throw 0;
        }
        else
            _utf8 = TRUE;
    }
}

