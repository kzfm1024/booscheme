//
// input_port.cpp
//

#include <cctype>
#include <stdexcept>
#include "boo.h"
#include "boo_types.h"
#include "input_port.h"

namespace boo
{
    input_port::input_port(std::istream& is) : 
        m_is_pushed_token(false),
        m_is_pushed_char(false),
        m_pushed_token(),
        m_pushed_char(-1),
        m_in(is),
        m_buf()
    {
    }

    object* input_port::read_character()
    {
        if (m_is_pushed_char)
        {
            m_is_pushed_char = false;
            if (m_pushed_char == -1) { return EOF(); }
            else                     { return new character(m_pushed_char); }
        }
        else
        {
            char ch;
            m_in.get(ch);
            if      (m_in.bad()) { return error("IO stream corrupted"); }
            else if (m_in.eof()) { return EOF();                        }
            else                 { return new character(ch);            }
        }
    }

    object* input_port::peek_character()
    {
        int ch = peek_char();
        if (ch == -1) { return EOF(); }
        else          { return new character(ch); }
    }

    int input_port::peek_char()
    {
        if (m_is_pushed_char)
        {
            return m_pushed_char;
        }
        else
        {
            char ch;
            m_in.get(ch);
            if (m_in.bad()) { error("IO stream corrupted"); return -1; }
            return push_char(m_in.eof() ? -1 : ch);
        }
    }

    int input_port::push_char(int ch)
    {
        m_is_pushed_char = true;
        return m_pushed_char = ch;
    }

    int input_port::pop_char()
    {
        m_is_pushed_char = false;
        return m_pushed_char;
    }

    object* input_port::next_token()
    {
        char ch;
        std::string msg;

        if (m_is_pushed_token)
        {
            m_is_pushed_token = false;
            return m_pushed_token;
        }
        else if (m_is_pushed_char)
        {
            ch = static_cast<char>(pop_char());
        }
        else
        {
            m_in.get(ch);
        }

        // Skip whitespace
        while (isspace(ch)) m_in.get(ch);

        if (m_in.bad()) return error("IO stream corrupted");
        if (m_in.eof()) return EOF();

        switch (ch)
        {
        case '(' : return symbol::get("(");
        case ')' : return symbol::get(")");
        case '\'': return symbol::get("'");
        case '`' : return symbol::get("`");
        case ',':
            m_in.get(ch);
            if (ch == '@') return symbol::get(",@");
            else { push_char(static_cast<int>(ch)); return symbol::get(","); }
        case ';': // Comment: skip to end of line and then read next token
            while (!m_in.eof() && ch != '\n' && ch != '\r') m_in.get(ch);
            return next_token();
        case '"': // string_t
            m_buf.clear();
            m_in.get(ch);
            while (ch != '"' && !m_in.eof())
            {
                if (ch == '\\') m_in.get(ch);
                m_buf.append(1, ch);
                m_in.get(ch);
            }
            if (m_in.eof()) warn("EOF inside of a string.");
            return new string_t(m_buf);
		case '#':
			m_in.get(ch);
			switch (ch)
			{
			case 't': case 'T': return TRUE();
			case 'f': case 'F': return FALSE();
#if 0 // NOT YET
			case '(': // vector
				push_char('(');
				return listToVector(read());
#endif // NOT YET
			case '\\': // character
				m_in.get(ch);
				if (ch == 's' || ch == 'S' || ch == 'n' || ch == 'N')
				{
					push_char(ch);
					object* token = next_token();
					if (is_symbol(token, "space"))
					{
						return new character(' ');
					}
					else if (is_symbol(token, "newline"))
					{
						return new character('\n');
					}
					else
					{
						m_is_pushed_token = true;
						m_pushed_token = token;
						return new character(ch);
					}
				}
				else
				{
					return new character(ch);
				}
			case 'e': case 'i': case 'd': 
			case 'b': case 'o': case 'x':
				msg.assign("#");
				msg.append(1, ch);
				msg.append(" not implemented, ignored.");
				warn(msg);
				return next_token();
			default:
				msg.assign("#");
				msg.append(1, ch);
				msg.append(" not recognized, ignored.");
				return next_token();
			}
		default:
			m_buf.clear();
			char c = ch;
			do
			{
				m_buf.append(1, ch);
				m_in.get(ch);
			} while (!m_in.eof() && !isspace(ch) &&
					 ch != '(' && ch != ')' && ch != '\'' && ch != ';' &&
					 ch != '"' && ch != ',' && ch != '`');
			push_char(ch);

			// Try potential numbers, but catch any format errors.
			if (c == '.' || c == '+' || c == '-' || (c >= '0' && c <= '9'))
			{
				try
				{
					return to_number(m_buf);
				}
				catch (const std::invalid_argument& e)
				{
					; // do nothing
				}
			}

			return symbol::get(m_buf);
        }
    }

	object* input_port::read()
	{
		object* token = next_token();

		if (is_symbol(token, "("))
		{
			return read_tail();
		}
		else if (is_symbol(token, ")"))
		{
			warn("Extra ) ignored.");
			return read();
		}
		else if (is_symbol(token, "."))
		{
			warn("Extra . ignored.");
			return read();
		}
		else if (is_symbol(token, "'"))
		{
			return list(symbol::get("quote"), read());
		}
		else if (is_symbol(token, "`"))
		{
			return list(symbol::get("quasiquote"), read());
		}
		else if (is_symbol(token, ","))
		{
			return list(symbol::get("unquote"), read());
		}
		else if (is_symbol(token, ",@"))
		{
			return list(symbol::get("unquote-splicing"), read());
		}
		else
		{
			return token;
		}
	}

	object* input_port::read_tail()
	{
		object* token = next_token();
		if (is_eof(token))
		{
			error("EOF during read.");
			return EOF();
		}
		else if (is_symbol(token, ")"))
		{
			return NIL();
		}
		else if (is_symbol(token, "."))
		{
			object* result = read();
			token = next_token();
			if (!is_symbol(token, ")")) 
			{ 
				warn("Where's the ')'? Got " + stringify(token) + " after ." );
			}
			return result;
		}
		else
		{
			m_is_pushed_token = true;
			m_pushed_token = token;

			//
			// return cons(read(), read_tail());
			//
			// 引数の評価順は仕様では不定である。
			// Intel x86 でよく使われる cdecl 規約では後の引数からスタックに
			// 積まれるため read_tail() が無限ループしてしまう。
			// 
			object* x = read();
			return cons(x, read_tail());
		}
	}
}
