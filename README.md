# Come-Come

## Who doesn't like a little retro game?

This project born, in our classroom of *Data Structure and Algorithms*, in the 2017.1 semester of **FUBA** (*Federal University of Bahia*). We were in our second period at University.

The teacher asked for a little **Pac-Man** game, using knowledge gained through his subject. But after some days (so-so a month) and class problems, he decided to cancel this homework project.

As we were happy with the project initial idea we decided to keep the project, like a fun hobbie, during the semester.

## The name

**Pac-Man** loves to eat some fruits and ghosts during the game, he just does it. So we decided to call him ***Come-Come*** (like *eat-eat* in english) cause he just eat and eat and eat...

This was the name we used to hear when child, and we love it.

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
```

### Compilando

Estando na pasta principal do projeto, rode:

```
cmake --build build
```

De forma alternativa, você pode utilizar `make` ou outro comando a depender do tipo de projeto alvo utilizado por você no CMake.

