/**
 * @file mupdf-global.h
 * @brief libmupdf-qt global definition
 * @author xiangxw xiangxw5689@126.com
 * @date 2012-03-28
 */
#ifndef MUPDF_GLOBAL_H
#define MUPDF_GLOBAL_H

#if defined(_WIN32)
# define MUPDF_QT_EXPORT __declspec(dllexport)
#else
# define MUPDF_QT_EXPORT
#endif

#endif // end MUPDF_GLOBAL_H
