// HParseHeader.h

#ifndef HPARSEHEADER_H_
#define HPARSEHEADER_H_

#include <istream>          // for std::ifstream
#include <fstream>          // for std::ifstream
#include <iterator>         // for std::istreambuf_iterator
#include <iostream>         // for std::cerr

#include "HParserAST.h"     // for Calc_H::Node, Calc_H::TokenInfo
#include "HParser.h"        // for Calc_H::Parser
#include "HScanner.h"       // for Calc_H::Scanner
#include "HParserSite.h"    // for Calc_H::ParserSite

namespace Calc_H
{
    template <class Iterator>
    bool parse(shared_ptr<Sentence>& s, std::string& error,
               Iterator begin, Iterator end)
    {
        ParserSite ps;
        Scanner<Iterator, ParserSite> scanner(ps);

        std::vector<TokenValue> infos;
        scanner.scan(infos, begin, end);
        #ifdef _DEBUG
            scanner.show_tokens(infos.begin(), infos.end());
        #endif

        Parser<shared_ptr<Node>, ParserSite> parser(ps);
        std::vector<TokenValue>::iterator it, end2 = infos.end();
        if (ps.error().empty())
        {
            for (it = infos.begin(); it != end2; ++it)
            {
                #if 0
                    std::cout << scanner.token_to_string(*it) << std::endl;
                #endif
                if (parser.post(it->m_token, make_shared<TokenValue>(*it)))
                {
                    if (parser.error())
                    {
                        ps.location() = it->location();
                        ps.message(std::string("ï∂ñ@ÉGÉâÅ[: ") +
                                   scanner.token_to_string(*it));
                    }
                    break;
                }
            }
        }

        if (ps.error().empty())
        {
            shared_ptr<Node> node;
            if (parser.accept(node))
            {
                s = static_pointer_cast<Sentence, Node>(node);
                return true;
            }
        }
        else
        {
            error = ps.error();
        }

        return false;
    }

    inline bool parse_string(shared_ptr<Sentence>& sentence, const char *s,
                             std::string& error)
    {
        return parse(sentence, error, s, s + std::strlen(s));
    }

    inline bool parse_string(shared_ptr<Sentence>& sentence, const std::string& str,
                             std::string& error)
    {
        return parse(sentence, error, str.begin(), str.end());
    }

    inline bool parse_file(shared_ptr<Sentence>& sentence, const char *filename,
                           std::string& error)
    {
        std::ifstream file(filename);
        if (file.is_open())
        {
            std::istreambuf_iterator<char> begin(file), end;
            bool ok = parse(sentence, error, begin, end);
            file.close();
            return ok;
        }
        return false;
    }
} // namespace Calc_H

#endif  // ndef HPARSEHEADER_H_
