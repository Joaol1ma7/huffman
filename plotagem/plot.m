Criar imagem dados = readtable('dados.csv');

plot(dados.tamanho, dados.media_comparacoes_lista, '-o');
hold on
plot(dados.tamanho, dados.media_comparacoes_arvore, '-x');
xlabel('Tamanho da estrutura');
ylabel('Média de comparações');
legend('Lista', 'Árvore');
grid on
title('Comparação de comparações - Lista vs Árvore');