NAME=letex

cat <<EOF > ${NAME}.tex
\documentclass{article}

\usepackage{amsmath,amssymb}
\usepackage{latexsym}
\usepackage{CJK}

\begin{document}
\begin{CJK*}{UTF8}{gbsn}

\author{崔嵬}
\title{高级编译项目}

你好 \[\int_a^b f(x)dx\]

\textsl{lean}
\newline

\maketitle
谢谢！
\tableofcontents
\section{Some Interesting Words}
Well, and here begins my lovely article.
\section{Good Bye World}
\ldots{} and here it ends.

\end{CJK*}
\end{document}
EOF

rm -f ${NAME}.{aux,log,toc,pdf}
texi2pdf ${NAME}.tex
firefox ${NAME}.pdf
