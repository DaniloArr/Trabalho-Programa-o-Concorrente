# Trabalho-Programa-o-Concorrente
Especificação:

Desenvolver uma aplicação multi-thread para processar uma matriz bidimensional contendo valores inteiros numa escala de 0 a 9. A ordem da matriz deve ser fornecida pelo usuário, podendo ser quadrada ou retangular. A matriz deverá ser gerada aleatoriamente e mostrada na tela. Pode-se determinar um tamanho máximo para a matriz na ordem de 1000x1000 elementos.

A matriz deverá ser percorrida, da esquerda para a direita, de cima para baixo, e cada elemento deverá ser substituído pela média aritmética simples de seus 8 vizinhos (truncada - parte inteira). Use uma thread para processar cada linha da matriz, mas observe a seguinte restrição: a thread que processa uma linha abaixo deve esperar os vizinhos da linha acima estarem prontos (processados) para então usá-los em seu processamento, mas poderá utilizar os prontos a direita, abaixo, a esquerda já processados por ela mesma e aguardar, se necessários, os pontos acima.

Ao final do processamento, a matriz resultante deverá ser mostrada na tela.
