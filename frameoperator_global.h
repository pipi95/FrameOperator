﻿#ifndef FRAMERECEIVER_GLOBAL_H
#define FRAMERECEIVER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FRAMEOPERATOR_LIBRARY)
#define FRAMEOPERATORSHARED_EXPORT Q_DECL_EXPORT
#else
#define FRAMEOPERATORSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FRAMERECEIVER_GLOBAL_H
