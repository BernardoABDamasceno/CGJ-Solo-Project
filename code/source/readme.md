## Build através da linha de comandos:

```
cd < localizacao do projeto >/CGJ_Project

cmake -S . -B build

cd ./build

cmake --build .

cd ./Debug

(./tangram.exe ou ./tangram_tests.exe)
```

## Build através do VS Code:

Nós fizemos sempre build do projeto através do próprio IDE.

Para o fazer da mesma forma serão precisas as seguintes extensões: 
  - C/C++ Extension Pack:
    - C/C++
    - CMake
    - CMake Tools

Após ter as extensões instaladas:
- CTRL+SHIFT+P
- CMake: Configure
    - Quaisquer problemas que possam surgir vale apena dar uma olhadela na opção &rarr; CMake: Edit CMake Cache (UI)

Após a configuração do CMake, ao lado esquerdo junto dos icones deve surgir um icone do CMake, 
nessa aba pode-se definir qual o target a ser executado (tangram_tests.exe or tangram.exe) em baixo da opção "Launch" 
e então efetivamente fazer build e executar o projeto.

Durante este processo o VS Code pode pedir qual o "kit" a ser usado (ex: Visual Studio Build Tools 2022), basta selecionar o que tiver instalado.

## Notas:

### Checkpoint 1

--------------------------

O **checkpoint 1** encontra-se organizado da seguinte forma:
- src &rarr; onde se irá encontrar coisas relevantes ao projeto a ser desenvolvido:
  - glm_math &rarr; aqui pode-se encontrar um ficheiro com as operações pedidas para o checkpoint 1;
  - tools &rarr; aqui encontram-se métodos auxiliares como os métodos de geração aleatória de vetores e matrizes fornecidos;
  - main.cpp &rarr; de momento vazio pois ainda não se iniciou o tangram
- tests &rarr; pasta de testes:
  - glm_math &rarr; todos os testes do checkpoint 1 encontram-se aqui;
  - main.cpp &rarr; executa todos os testes do checkpoint 1.

---------------------------

O teste da propriedade de inversão de vez em quando parece falhar, 
mas pode-se ver pelas matrizes que não é o caso é só mesmo imprecisão que nem mesmo o THRESHOLD consegue resolver.

---------------------------

### Checkpoint 2

---------------------------

No checkpoint 2 adicionou-se a biblioteca **mgl** entregue pelo professor, mais o ficheiros: **values.hpp**, **app.hpp**, **app.cpp**;

- **values.hpp** &rarr; guarda valores necessários para desenhar o tangrama;
- **app.hpp** & **app.cpp** &rarr; são juntos o antigo **hello-2d-world.cpp** entregue pelo professor.

---------------------------

O executável **tangram_tests.exe** continua a dar display das operações feitas no **checkpoint 1**.

O executável **tangram.exe** dá display ao tagrama 2D.

---------------------------

### Checkpoint 3

No checkpoint 3 criou-se a versão 3D do tangrama.

Foi alterada a versão de C++ de 17 para 20 para termos acesso a outras funcionalidades mais recentes.

Criou-se os seguintes ficheiros:
- **valuesApp3d.hpp** &rarr; guarda valores necessários para desenhar o tangrama em 3d;
- **app3d.hpp** & **app3d.cpp** &rarr; onde se encontra a maioria da lógica para criar o tangrama;
- **mglScenegraph.hpp** & **mglScenegraph.cpp** &rarr; Scenegraph hierárquico (pedido opcionalmente);
- **mglSceneNode.hpp** & **mglSceneNode.cpp** &rarr; Scene Node para o Scene Graph;
- **mglIDrawable.hpp** &rarr; feito exclusivamente para o IDrawable;

O executável **tangram2d.exe** dá display ao tagrama 2D.
O executável **tangram3d.exe** dá display ao tagrama 3D.

Removeu-se o **tangram_tests.exe** por conflitos com a versão de C++ nova e falta de tempo para atualizar.

Foi usado o ChatGPT e GitHub Copilot para ajudar a resolver bugs, acelerar a escrita e realizar os cálculos com quaterniões.