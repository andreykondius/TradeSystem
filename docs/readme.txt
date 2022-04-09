Для устранения ошибок типа "unresolved reference" необходимо в файлах
debug\include\c10\macros\cmake_macros.h
release\include\c10\macros\cmake_macros.h

заменить строчку 
#define C10_BUILD_SHARED_LIBS

на
#ifdef  C10_BUILD_SHARED_LIBS
    #undef C10_BUILD_SHARED_LIBS
#endif
