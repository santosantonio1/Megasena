- Na mesma pasta deste arquivo, há o app.exe e o ganhou.exe. Caso queira testar o cenário com vencedores e não depender da sorte,
execute o ganhou.exe que tem como números sorteados {1,2,3,4,5,6,7,8,9,10}. Caso contrário, execute o app.exe que é a solução padrão do 
enunciado.

- A solução utiliza uma biblioteca gráfica externa chamada Raylib. O executável compilado está na mesma pasta deste arquivo e se chama app.

- Para compilar o programa, utilizar uma máquina com Windows, ir para a pasta build e executar o makefile. Se executar o programa na pasta
build, a opção de salvar os dados não irá funcionar, tome cuidado!

- O makefile espera um arquivo "raylib.h" na pasta include e o arquivo "librarylib.a" (criado na instalação da biblioteca gráfica)
na pasta lib.

- Link para a biblioteca: https://github.com/raysan5/raylib

- O compilador utilizado foi o mingw (g++).
