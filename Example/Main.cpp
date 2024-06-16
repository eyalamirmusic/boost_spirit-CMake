#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/phoenix/operator.hpp>

#include <iostream>
#include <fstream>
#include <string>

#include "example.hpp"

using namespace boost::spirit;
using boost::phoenix::val;

template <typename Lexer>
struct example5_base_tokens : lex::lexer<Lexer>
{
protected:
    example5_base_tokens() {}

public:
    void init_token_definitions()
    {
        identifier = "[a-zA-Z_][a-zA-Z0-9_]*";
        constant = "[0-9]+";
        ifToken = "if";
        whileToken = "while";

        this->self += lex::token_def<>('(') | ')' | '{' | '}' | '=' | ';' | constant;
        this->self += ifToken | whileToken | identifier;
        this->self("WS") =
            lex::token_def<>("[ \\t\\n]+") | "\\/\\*[^*]*\\*+([^/*][^*]*\\*+)*\\/";
    }

    lex::token_def<lex::omit> ifToken {};
    lex::token_def<lex::omit> whileToken {};
    lex::token_def<std::string> identifier {};
    lex::token_def<unsigned int> constant {};
};

template <typename Iterator, typename Lexer>
struct example5_base_grammar : qi::grammar<Iterator, qi::in_state_skipper<Lexer>>
{
    template <typename TokenDef>
    example5_base_grammar(TokenDef const& tok)
        : example5_base_grammar::base_type(program)
    {
        using boost::spirit::_val;

        program = +block;

        block = '{' >> *statement >> '}';

        statement = assignment | if_stmt | while_stmt;

        assignment =
            (tok.identifier >> '=' >> expression
             >> ';')[std::cout << val("assignment statement to: ") << _1 << "\n"];

        if_stmt = (tok.ifToken >> '(' >> expression >> ')'
                   >> block)[std::cout << val("if expression: ") << _1 << "\n"];

        while_stmt =
            (tok.whileToken >> '(' >> expression >> ')'
             >> block)[std::cout << val("while expression: ") << _1 << "\n"];

        expression = tok.identifier[_val = _1] | tok.constant[_val = _1];
    }

    using skipper_type = qi::in_state_skipper<Lexer>;
    using Rule = qi::rule<Iterator, skipper_type>;

    Rule program {};
    Rule block {};
    Rule statement {};
    Rule assignment {};
    Rule if_stmt {};
    Rule while_stmt {};

    using expression_type = boost::variant<unsigned int, std::string>;
    qi::rule<Iterator, expression_type(), skipper_type> expression {};
};

template <typename Lexer>
struct example5_tokens : example5_base_tokens<Lexer>
{
    typedef example5_base_tokens<Lexer> base_type;

    example5_tokens()
    {
        elseToken = "else";
        this->self = elseToken;
        this->base_type::init_token_definitions();
    }

    lex::token_def<lex::omit> elseToken {};
};

template <typename Iterator, typename Lexer>
struct example5_grammar : example5_base_grammar<Iterator, Lexer>
{
    template <typename TokenDef>
    example5_grammar(TokenDef const& tok)
        : example5_base_grammar<Iterator, Lexer>(tok)
    {
        this->if_stmt = this->if_stmt.copy() >> -(tok.elseToken >> this->block);
    }
};

int main()
{
    typedef std::string::iterator base_iterator_type;

    using token_type =
        lex::lexertl::token<base_iterator_type,
                            boost::mpl::vector<unsigned int, std::string>>;

    using lexer_type = lex::lexertl::lexer<token_type>;
    using example5_tokens = example5_tokens<lexer_type>;
    using iterator_type = example5_tokens::iterator_type;

    using example5_grammar =
        example5_grammar<iterator_type, example5_tokens::lexer_def>;


    example5_tokens tokens;
    example5_grammar calc(tokens);

    std::string str(read_from_file("example5.input"));

    std::string::iterator it = str.begin();
    iterator_type iter = tokens.begin(it, str.end());
    iterator_type end = tokens.end();

    std::string ws("WS");
    bool r = qi::phrase_parse(iter, end, calc, qi::in_state(ws)[tokens.self]);

    if (r && iter == end)
    {
        std::cout << "-------------------------\n";
        std::cout << "Parsing succeeded\n";
        std::cout << "-------------------------\n";
    }
    else
    {
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "-------------------------\n";
    }

    std::cout << "Bye... :-) \n\n";
    return 0;
}