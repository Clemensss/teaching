# Instalação

## Download

* Primeiro abra o terminal. No ubuntu pode-se usar o atalho `CTRL + ALT + T`.
* Então entre no diretório desejado usando o comando cd. Exemplo:

   ```bash
   cd Desktop/
   ```

* Em seguida clone o repositório do projeto, através do comando git clone:

   ```bash
   git clone https://github.com/Clemensss/teaching.git
   ```

* Quando o projeto for clonado, entre na pasta do download:

   ```bash
   cd teaching/AB2.1-TP
   ```

## Compilação

* Uma vez clonado o projeto e acessada a sua pasta, é necessário navegar até o diretório onde está localizado o código-fonte:

   ```bash
   cd src/
   ```

* Para compilar, é utilizado o `gcc`, através do seguinte comando:

   ```bash
   gcc -o bin main.c
   ```

## Execução

Para a execução do programa, poderão ser utilizados os seguintes comandos:

* Para o teste com uso intenso do **processador**:

   ```bash
   ./bin cpu
   ```

* Para o teste com uso intenso da **memória** e do **processador**:

   ```bash
   ./bin cpu-mem
   ```

* Para salvar os resultados dentro de um arquivo, adicione o nome do arquivo desejado como parâmetro do comando de execução:

   ```bash
   ./bin cpu-mem testes.txt
   ```

  * Caso essa opção seja executada, os resultados deixarão de ser mostrados no terminal e estarão dispóníveis apenas no arquivo indicado.
  * Caso ainda não exista um arquivo com o nome indicado no parâmetro, o programa o criará.
