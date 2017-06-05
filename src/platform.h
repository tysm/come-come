#include <stdint.h>

#if defined(_WIN32)
#   define PLATFORM_WIN32
#elif defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
#   define PLATFORM_UNIX
#else
#   error Unsupported platform
#endif

typedef enum key_id
{
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_ENTER,
    KEY_ESCAPE,
    KEY_MAX
} key_id_t;

/**
 * Retorna verdadeiro se a tecla especifica começou a ser apertada agora, falso caso esteja
 * sendo apertada a mais tempo ou não está apertada.
 */
extern int keyhit(key_id_t k);

/**
 * Retorna verdadeiro se a tecla especifica está apertada, falso caso contrário.
 */
extern int keyhold(key_id_t k);

/**
 * Atualiza o estado das teclas no terminal.
 */
extern void cli_update_keys(void);

/**
 * Aguarda por alguns milisegundos até o próximo tick.
 * Uso exclusivo para o frontend em terminal.
 */
extern void cli_sync(void);

/**
 * Limpa e renderiza os caracteres especificados no terminal.
 * Usando 24x80 seguindo o padrão VT200.
 */
extern void cli_render(char screen[24][80], char life[3], float points);
