#include <stdint.h>

#if defined(_WIN32)
#   define PLATFORM_WIN32
#elif defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
#   define PLATFORM_UNIX
#else
#   error Unsupported platform
#endif

/**
 * Aguarda por alguns milisegundos até o próximo tick.
 * Uso exclusivo para o frontend em terminal.
 */
extern void cli_sync(void);

/**
 * Limpa e renderiza os caracteres especificados no terminal.
 * Usando 24x80 seguindo o padrão VT200.
 */
extern void cli_render(char screen[24][80]);
