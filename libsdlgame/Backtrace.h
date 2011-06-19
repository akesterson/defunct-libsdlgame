#ifndef __BACKTRACE_H__
#define __BACKTRACE_H__

#ifdef __WIN32__
extern void __libsdlgame_sigsegvhandler(int z);
extern void __libsdlgame_show_stacktrace(void);
#endif

extern void RegisterStacktrace(void);

#endif // __BACKTRACE_H__
