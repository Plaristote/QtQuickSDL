#ifndef  QSDL_GLOBAL_H
# define QSDL_GLOBAL_H

# include <QtCore/qglobal.h>

# if defined(QSDL_LIBRARY)
#  define QSDL_EXPORT Q_DECL_EXPORT
# else
#  define QSDL_EXPORT Q_DECL_IMPORT
#endif

#endif
