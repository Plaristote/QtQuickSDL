#ifndef QSDLCONTEXT_H
#define QSDLCONTEXT_H

#include "QSDL_global.h"
#include <QSharedPointer>

class QSDL_EXPORT QSDLContext
{
public:
  QSDLContext();
  ~QSDLContext();

  static QSharedPointer<QSDLContext> require();
};

#endif // QSDLCONTEXT_H
