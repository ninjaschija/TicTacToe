/// @file
///
/// @author
///
/// @copyright
///
#ifndef TICTACTOECORE_GLOBAL_HPP
#define TICTACTOECORE_GLOBAL_HPP

#include <QtCore/qglobal.h>

#if defined(TICTACTOECORE_LIBRARY)
#  define TICTACTOECORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define TICTACTOECORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TICTACTOECORE_GLOBAL_HPP
