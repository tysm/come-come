# Come-Come

## Compilando

### Requisitos

 + Um compilador de C
    - No Windows recomenda-se o [MinGW-w64](https://sourceforge.net/projects/mingw-w64/).
    - No Linux recomenda-se o GCC.
 + O build system [CMake](https://cmake.org/).

### Gerando os arquivos de build

Estando na pasta principal do projeto, rode:

```
mkdir build
cd build
cmake ..
**mkdir maps**
**cd..**
**xcopy maps "build\\maps" /E**
```
`
-**Comandos em negrito devem ser efetuados a cada pull feito**
`

### Compilando

Estando na pasta principal do projeto, rode:

```
cmake --build build
```

De forma alternativa, você pode utilizar `make` ou outro comando a depender do tipo de projeto alvo utilizado por você no CMake.

