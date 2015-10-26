dnl **************************************************************************
dnl * Initialize
dnl **************************************************************************
AC_INIT([[[mementos]]],[[[1.1]]],[ransford@cs.umass.edu])

AC_ARG_WITH(llvm,
	AS_HELP_STRING([--with-llvm=PATH], [specify path to LLVM]))
AS_IF([test -n "$with_llvm"],
	[AC_PATH_PROG([LLVMCONFIG], [llvm-config], [], [${with_llvm}/bin])],
	[AC_PATH_PROG([LLVMCONFIG], [llvm-config])])
AS_IF([test -z "$LLVMCONFIG"],
	[AC_MSG_ERROR([llvm-config not found in your PATH; quitting.])])

AC_ARG_VAR([LLVMPREFIX], [Location of LLVM parent directory])
LLVMPREFIX=`$LLVMCONFIG --prefix`

AC_ARG_VAR([LLVM_BIN], [Location of LLVM binaries])
LLVM_BIN=`$LLVMCONFIG --bindir`

AC_ARG_VAR([LLVM_LIB], [Location of LLVM libraries])
LLVM_LIB=`$LLVMCONFIG --libdir`

AC_PATH_PROG([CLANG], [clang], [], [${LLVM_BIN}])
AC_PATH_PROG([LLVM_OPT], [opt], [], [${LLVM_BIN}])
AC_PATH_PROG([LLVM_LLC], [llc], [], [${LLVM_BIN}])
AC_PATH_PROG([LLVM_LINK], [llvm-link], [], [${LLVM_BIN}])

AC_ARG_VAR([GCC], [Location of gcc])
AC_ARG_WITH(gcc,
	AS_HELP_STRING([--with-gcc=PATH], [specify path to gcc]))
AS_IF([test -n "$with_gcc"],
	[AC_PATH_PROG([GCC], [msp430-elf-gcc], [], [${with_gcc}/bin])],
	[AC_PATH_PROG([GCC], [msp430-gcc])])
AS_IF([test -z "$GCC"],
	[AC_MSG_ERROR([gcc not found in your PATH; quitting.])])
GCC_HEADERS=`echo | "$GCC" -v -x c -E - 2>&1 | awk '/#include <\.\.\.>/{flag=1;next}/End of search list/{flag=0}flag' | awk '{sub(/^ +/, ""); printf " -I%s", $0}'`
GCC_PATH="$with_gcc"

AC_ARG_VAR([MSPSIM], [Location of mspsim])
AC_ARG_WITH(mspsim,
	AS_HELP_STRING([--with-mspsim=PATH], [specify path to mspsim]))
AS_IF([test -n "$with_mspsim"],
	[MSPSIM="$with_mspsim"],
	[MSPSIM="/opt/mementos/src/mspsim"])

dnl Verify that the source directory is valid
AC_CONFIG_SRCDIR(["Makefile.common.in"])

dnl Configure a common Makefile
AC_CONFIG_FILES(Makefile.common)

AC_SUBST(prefix)
AC_SUBST(LLVMPREFIX)
AC_SUBST(LLVM_BIN)
AC_SUBST(LLVM_LIB)
AC_SUBST(GCC)
AC_SUBST(GCC_PATH)
AC_SUBST(GCC_HEADERS)
AC_SUBST(MSPSIM)

dnl This must be last
AC_OUTPUT
