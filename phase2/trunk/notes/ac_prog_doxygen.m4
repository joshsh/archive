dnl This was taken from http://minos.phy.bnl.gov/~bviren/elbo/libnuosc++/tvmet-1.2.0/config/ac_prog_doxygen.m4
dnl It may need to be modified for Phase2.

dnl Check for doxygen to create API docs
dnl
AC_DEFUN([AC_PROG_DOXYGEN],
[
AC_ARG_ENABLE(doxygen, [  --enable-doxygen        enable documentation generation with doxygen (auto)])
AC_ARG_ENABLE(dot, [  --enable-dot            use 'dot' to generate graphs in doxygen (auto)])
AC_ARG_ENABLE(html-docs, [  --enable-html-docs      enable HTML generation with doxygen (yes)], [], [ enable_html_docs=yes])
AC_ARG_ENABLE(latex-docs, [  --enable-latex-docs     enable LaTeX documentation generation with doxygen (no)], [], [ enable_latex_docs=no])

if test "x$enable_doxygen" = xno; then
 enable_doc=no	
else 
 AC_CHECK_PROG(DOXYGEN, doxygen, doxygen)
 if test x$DOXYGEN = x; then
  if test "x$enable_doxygen" = xyes; then
   AC_MSG_ERROR([could not find doxygen])
  fi
  enable_doc=no
 else
  enable_doc=yes
   AC_CHECK_PROG(DOT, dot, dot)
 fi
fi

AM_CONDITIONAL(DOC, test x$enable_doc = xyes)

if test x$DOT = x; then
 if test "x$enable_dot" = xyes; then
  AC_MSG_ERROR([could not find dot])
 fi
 enable_dot=no
else
 enable_dot=yes
fi

AC_SUBST(enable_dot)
AC_SUBST(enable_html_docs)
AC_SUBST(enable_latex_docs)
])

