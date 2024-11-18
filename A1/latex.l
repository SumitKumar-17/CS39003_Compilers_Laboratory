%{
#include "procltx.h"
%}

%option noyywrap

%%
"~"                                { return TILDE; }
\\[a-zA-Z]+                        { return ALPHA_CMD; }
\\[^a-zA-Z\s]                      { return NON_ALPHA_CMD; }
\\begin\{[^}]+\}                   { return BEGIN_ENV; }
\\end\{[^}]+\}                     { return END_ENV; }
\$([^$]*\n)*[^$]*\$                { return INLINE_MATH; }
\$\$([^$]*\n)*[^$]*\$\$            { return DISPLAYED_MATH; }
\\\[([^]]*\n)*[^\]]*\\\]           { return DISPLAYED_MATH; }
%.*                                { return COMMENT; }
.|\n                               { return OTHER; }
